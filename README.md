# Arduino IRQs and the time count hell

The Arduino boards - at least the ATmega328P-based - rely on interrupts to count the time. Due to the simple interrupt management used by default, disabling or using custom interrupts on Arduino can lead to confusing behavior concerning time. Bad counts can happened!

## The point on Arduino IRQs and time count

 On the Arduino no difference is made between interrupt routines: it has no priority support. That means that if you declare [an interrupt service routine](https://www.arduino.cc/en/Reference/AttachInterrupt), its execution will differ the execution of all the others interrupt routines, including the one responsible to count the time. If your ISR is too long, other external interrupts may have been "lost" during its execution.

### Prove it: show me the code

This can be simply observed in the [IRQ_micros](/IRQ_micros) and [IRQ_millis](/IRQ_millis) sketches. They use a button bound to the 3nd pin to trigger an interrupt service routine. This routine will then execute a long running task and measure its duration using respectively [micros()](https://www.arduino.cc/en/Reference/Micros) and [millis()](https://www.arduino.cc/en/Reference/Millis). We then re-run the long running task outside the interrupt and measures its duration again. You will easily observe that the time count have stopped during the interrupt and that the time really elapsed is not filled again after the interrupt.

```
Duration of the busy-loop (during the interrupt): 1228 us
Duration of the busy-loop (outside an interrupt): 132492 us
```

If you disable interrupts (see the [noInterrupts](/noInterrupts) sketch), this is more direct and clear: the time do not passes anymore on your Arduino, which is stopped somewhere in the past.

```
Duration of the busy-loop (IRQ enabled): 1319 ms
Duration of the busy-loop (IRQ disabled): 0 ms
```

## Enable interrupts to be... interrupted

If you have a long-running interrupt service routine (ISR) and both want to preserve it and still have time flowing, you can choose to make your IRS interruptible. For that, simply enable interrupts again when you enter your ISR.

The logical behind it is that for each interrupt, the microcontroller disable globally the interrupts before calling the ISR. This is a measure for code safety, so you don't have interrupts interrupting interrupt service routine and potentially recursive calls.

### Code evidence

Check the [IRQ_on_IRQ_micros](/IRQ_on_IRQ_micros) sketch for an example.

If we trigger one time the button, our ISR can run normally and is only interrupted by the time count interrupts.

```
Duration of the busy-loop (during the interrupt): 660256 us
Duration of the busy-loop (outside an interrupt): 660704 us
```

But this time nothing preserve our ISR to be interrupted by itself: if we tap multiple times on the button (here 10 consecutive times) the ISR is first triggered then interrupted by itself as many times (here 9 times). We have a kind of stack of ISRs.

```
Duration of the busy-loop (during the interrupt): 5510788 us
Duration of the busy-loop (outside an interrupt): 660712 us
```

# So what?

If you rely on Arduino time functions (`micros()`, `millis()`) to process time-sensible statistics or need a very precise time control, I would suggest to be very cautious in your use of IRQs. You should always code very direct and fast interrupt service routines.

If you need to make advanced use of interrupts, you may dig [the Arduino documentation](http://playground.arduino.cc/Code/Interrupts) and [AVR](http://www.atmel.com/devices/ATMEGA328P.aspx) datasheets. Nick Gammon have done [an absolutely clear and complete article on interrupts on Arduino](http://gammon.com.au/interrupts). EngBlaze have also done two great [blog posts on interrupts](http://www.engblaze.com/we-interrupt-this-program-to-bring-you-a-tutorial-on-arduino-interrupts/).

Finally check the GreyGnome [EnableInterrupt library](https://github.com/GreyGnome/EnableInterrupt) for a very complete resource, with documentation, code and code samples of an advanced use of interrupts and `EnableInterrupt` function.

--------------------------------

Interested by this note? You may check out [our blog](http://blog.ytotech.com).
Something to say? You're welcome, [please send-me an email](mailto:yoan@ytotech.com).

TODO: test with the [Arduino Time library](http://playground.arduino.cc/code/time).
