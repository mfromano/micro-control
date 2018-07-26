#include <Arduino.h>
#include <DigitalIO.h>
#include <delay_cycles_avr.h>

const int movementout = 6;
const int cam = 4;
const int auxout = 5;

void setup() {
  fastPinMode(movementout, OUTPUT);
  fastPinMode(cam, OUTPUT);
  fastPinMode(auxout, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if(Serial.available() > 0){ //checking for input from matlab
    Serial.readString();
    fastDigitalWrite(movementout, HIGH);
    fastDigitalWrite(auxout, HIGH);
    while (true) {
      fastDigitalWrite(cam, HIGH);
      _delay_cycles(16000);
      fastDigitalWrite(cam, LOW);
      _delay_cycles(784000);
      //Arduino Uno has 16 Mhz clock cycles, may vary if a different board is used
      //So, to delay for 49 milliseconds do 16Mhz*0.049s (may not be 16Mhz depending on the board)
    }
    fastDigitalWrite(movementout, LOW);
    fastDigitalWrite(auxout, LOW);
  }
}
