#include <Arduino.h>
#include <DigitalIO.h>
#include <delay_cycles_avr.h>

const int ttlpulseout = 2;
const int cam = 4;



void setup() {
  fastPinMode(ttlpulseout, OUTPUT);
  fastPinMode(cam, OUTPUT);
}
void loop() {
  fastDigitalWrite(ttlpulseout, HIGH);
  while (true) {
    fastDigitalWrite(cam, HIGH);
    _delay_cycles(16000);
    fastDigitalWrite(cam, LOW);
    _delay_cycles(784000);
    //Arduino Uno has 16 Mhz clock cycles, may vary if a different board is used
  }
  fastDigitalWrite(ttlpulseout, LOW);
}
