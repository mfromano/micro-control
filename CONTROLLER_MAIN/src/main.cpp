#include <Arduino.h>
#include <DigitalIO.h>
#include <time.h>

const int ttlpulseout = 2;
const int cam = 4;



void setup() {
  fastPinMode(ttlpulseout, OUTPUT);
  fastPinMode(cam, OUTPUT);
}
void loop() {
  clock_t t;
  t = clock();
  fastDigitalWrite(ttlpulseout, HIGH);
  while ((float)t/CLOCKS_PER_SEC <= 500) {
    t = clock();
    while ((((float)t/CLOCKS_PER_SEC)*1000) - int((((float)t/CLOCKS_PER_SEC)*1000)) > 0){
    }
    fastDigitalWrite(cam, HIGH);
    delay(1);
    fastDigitalWrite(cam, LOW);
    delay(49);
  }
  fastDigitalWrite(ttlpulseout, LOW);
}
