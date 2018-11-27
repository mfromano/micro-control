#include <math.h> //this was included for the sine function
#include <Arduino.h> //this was included for the Serial.readbytes command
#include <elapsedMillis.h>
#include <DigitalIO.h>
#include <IntervalTimer.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


const uint8_t PUFF_PIN = 3; // pin to use for PUFF
const uint8_t DAC_PIN = A14;
const bool EXPERIMENT_ON = 0;

elapsedMillis experimentTime;

void setup() {
  fastPinMode(PUFF_PIN, OUTPUT);
  fastPinMode(DAC_PIN, OUTPUT);
  AudioMemory(128);
  experimentTime = 0;
}

void loop(){
  while(experimentTime < 2000) {
    fastDigitalWrite(PUFF_PIN,HIGH);
    analogWrite(DAC_PIN, 2000);
    delay(50)
    fastDigitalWrite(PUFF_PIN,LOW);
    analogWrite(DAC_PIN,0);
    delay(1000);
  }

}
