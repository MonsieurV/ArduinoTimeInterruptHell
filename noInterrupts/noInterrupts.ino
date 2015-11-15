/*
Show that Arduino uses interrupts to count time.
Each time you disable the interrupts (through noInterrupts) or another
interrupt routine than the time count one executes, the time count is stopped
for this duration.

So if you use interrupts on your program you should acknowledge it and not rely
absolutely on the Arduino time count, for e.g. to compute time-related
statistics.
https://www.arduino.cc/en/Reference/AttachInterrupt

by: Yoan Tournade
*/

const int LED_PIN = 13;
const long BUSY_N = 1000000;
unsigned long begintAt = 0;
unsigned long endAt = 0;
int isDone = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if(!isDone) {
    // Time count with interrupts enabled.
    begintAt = millis();
    digitalWrite(LED_PIN, HIGH);
    for(long i = 0; i < BUSY_N; i++) { delayMicroseconds(1); }
    digitalWrite(LED_PIN, LOW);
    endAt = millis();
    Serial.print("Duration of the busy-loop (IRQ enabled): ");
    Serial.print(endAt - begintAt);
    Serial.println(" ms");
    // Time count with interrupts disabled.
    noInterrupts();
    begintAt = millis();
    digitalWrite(LED_PIN, HIGH);
    for(long i = 0; i < BUSY_N; i++) { delayMicroseconds(1); }
    digitalWrite(LED_PIN, LOW);
    endAt = millis();
    interrupts();
    Serial.print("Duration of the busy-loop (IRQ disabled): ");
    Serial.print(endAt - begintAt);
    Serial.println(" ms");
    isDone = 1;
  }
}
