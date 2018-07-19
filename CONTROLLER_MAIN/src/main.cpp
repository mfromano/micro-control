#include <Arduino.h>
#include <DigitalIO.h>
#include <time.h>

const int ttlpulseout = 2;
const int cam = 4;
int cycle;

void setup() {
  fastPinMode(ttlpulseout, OUTPUT);
  fastPinMode(cam, OUTPUT);

}
void loop() {
  clock_t t;
  fastDigitalWrite(ttlpulseout, HIGH);
  while (clock() <= 500) {
    t = clock();
    if (((float)t)/CLOCKS_PER_SEC % millis() ~= 0) {

    }
    fastDigitalWrite(cam, HIGH);
    delayMicroseconds(500);
    fastDigitalWrite(cam, LOW);
    delayMicroseconds(500);
  }
  fastDigitalWrite(ttlpulseout, LOW);
}
