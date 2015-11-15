/*
Experiment how interrupts (IRQs) can disrupt the Arduino time count.
If your program use the micros() function to compute statistics, interrupts
can falsify your results.

As the documentation states, micros() relies on interrupts to count.
So no interrupts: no more time count.
https://www.arduino.cc/en/Reference/AttachInterrupt

by: Yoan Tournade
*/
const int LED_PIN = 13;
const long BUSY_N = 100000;
int volatile hasIrq = 0;
unsigned long begintAt = 0;
unsigned long endAt = 0;

void irq() {
  begintAt = micros();
  digitalWrite(LED_PIN, HIGH);
  hasIrq = 1;
  for(long i = 0; i < BUSY_N; i++) { delayMicroseconds(1); }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(3, HIGH);
  // Run a long busy-loop in an interrupt routine.
  attachInterrupt(1, irq, FALLING);
}

void loop() {
  digitalWrite(LED_PIN, LOW);
  if(hasIrq) {
    endAt = micros();
    Serial.print("Duration of the busy-loop (during the interrupt): ");
    Serial.print(endAt - begintAt);
    Serial.println(" us");
    // Now run the same long busy-loop and compare the running time.
    begintAt = micros();
    for(long i = 0; i < BUSY_N; i++) { delayMicroseconds(1); }
    endAt = micros();
    Serial.print("Duration of the busy-loop (outside an interrupt): ");
    Serial.print(endAt - begintAt);
    Serial.println(" us");
    hasIrq = 0;
  }
}
