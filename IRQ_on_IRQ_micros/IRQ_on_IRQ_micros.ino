/*
If you have a long-running interrupt service routine (ISR), you may want to
enable it to be itself interrupted.
Note this may be avoided in the first place (making your ISR fast and
ininturruptable), because it can lead to strange behavior, like recursive
call of the same ISR.

A great resource on Arduino interrupts:
https://github.com/GreyGnome/EnableInterrupt/wiki/Usage

by: Yoan Tournade
*/
const int LED_PIN = 13;
const long BUSY_N = 500000;
int volatile hasIrq = 0;
unsigned long begintAt = 0;
unsigned long endAt = 0;

void irq() {
  digitalWrite(LED_PIN, HIGH);
  hasIrq = 1;
  interrupts();
  begintAt = micros();
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
