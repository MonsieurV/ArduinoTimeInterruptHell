# Arduino IRQs and the time count hell

The Arduino boards - at least the ATmega328-based - rely on interrupts to count the time. Due to the simple interrupt management, disabling or using custom interrupts on Arduino can thus lead to confusing behavior concerning time. Bad counts can happened!

## The point on Arduino IRQs and time count

 On the ATmega328 no difference is made between interrupt routines: it has no priority support. That means that if you declare [an interrupt service routine](https://www.arduino.cc/en/Reference/AttachInterrupt), its execution will differ the execution of all the others interrupt routines, including the one responsible to count the time.

## Prove it, show me the code!

This can be simply observed in the [IRQ_micros](/IRQ_micros) and [IRQ_millis](/IRQ_millis) sketches. They use a button bound to the 3nd pin to trigger an interrupt service routine. This routine will then execute a long running task and measure its duration using respectively [micros()](https://www.arduino.cc/en/Reference/Micros) and [millis()](https://www.arduino.cc/en/Reference/Millis). We then re-run the long running task outside the interrupt and measures its duration again. You will easily observe that the time count have stopped during the interrupt and that the time really elapsed is not filled again after the interrupt.

```
Duration of the busy-loop (during the interrupt): 1228 us
Duration of the busy-loop (outside an interrupt): 132492 us
```

If you disable interrupts, this is more direct and clear: the time do not passes anymore on your Arduino, which is stopped somewhere in the past.

```
Duration of the busy-loop (IRQ enabled): 1319 ms
Duration of the busy-loop (IRQ disabled): 0 ms
```

# So what?

If you rely on Arduino time functions (`micros()`, `millis()`) to process time-sensible statistics or need a very precise time control, I would suggest to not use IRQs, or at least to acknowledge that each interrupt will stop your time count, even if it seems insensible with short interrupt routines.

You should always code very direct and fast interrupt routines.

--------------------------------

Interested by this note? Check out [our blog](http://blog.ytotech.com).
Something to say? You're welcome, [please send-me a mail](mailto:yoan@ytotech.com).

TODO: test with the [Arduino Time library](http://playground.arduino.cc/code/time).
