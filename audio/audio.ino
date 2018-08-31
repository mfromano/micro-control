#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=189,176
AudioEffectFade          fade1;          //xy=380,177
AudioOutputAnalog        dac1;           //xy=552,179
AudioConnection patchCord1(sine1,dac1);
//
const uint8_t TONE_LENGTH = 350; //ms
const uint8_t START_PIN = 4;
const float FQ = 9500; //Hz
const uint16_t FADE_MS = 500; // ms


void setup() {
  Serial.begin(9600);
  AudioMemory(20);
  dac1.analogReference(EXTERNAL);
  pinMode(START_PIN,INPUT_PULLDOWN);
  sine1.frequency(FQ);
  sine1.amplitude(0);
}

void loop() {
  if (digitalRead(START_PIN) == 1) {
    sine1.amplitude(1);
    while (digitalRead(START_PIN)) {
      ;
    }
    sine1.amplitude(0);
  }
}
