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

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=189,176
AudioEffectFade          fade1;          //xy=380,177
AudioOutputAnalog        dac1;           //xy=552,179
AudioConnection patchCord1(sine1,dac1);
// end automatically generated code

bool isRunning = false;

const uint16_t FQ = 50;
const uint8_t decimals = 10;
float TRIAL_LENGTH = 60000000; // ms

const uint8_t CAMERA_PIN = 6;
const uint8_t CAMERA_FQ = 20; // Hz

elapsedMicros trial_t;
elapsedMillis interpulseinterval;
IntervalTimer trial_timer;

const uint8_t AMP_PIN = 5;


bool hasRun = false;

void begin(float trial_length);
void capture();

void setup() {
  fastPinMode(CAMERA_PIN, OUTPUT);
  AudioMemory(4);
  // dac1.analogReference(EXTERNAL);
  sine1.frequency(FQ);
  sine1.amplitude(.5);
}

void loop(){
 if ((!isRunning) && (!hasRun)) {
    begin(TRIAL_LENGTH);
  }
}

void begin(float trial_length) {
    TRIAL_LENGTH = trial_length;
    fastPinMode(AMP_PIN,OUTPUT); // add amplifier
    fastDigitalWrite(AMP_PIN,HIGH);
    delay(10);
    trial_t = 0;
    trial_timer.begin(capture,1000000.0/(float)CAMERA_FQ);
    isRunning = true;
    hasRun = true;
}
void endCollection() {
  trial_timer.end();
  fastDigitalWrite(AMP_PIN,LOW);
  sine1.amplitude(0);
  isRunning = false;
}

void capture() {
  if (trial_t > TRIAL_LENGTH) {
    endCollection();
 }
  fastDigitalWrite(CAMERA_PIN,HIGH);
  interpulseinterval = 0;
  while (interpulseinterval < 40) {
    ;
  }
  fastDigitalWrite(CAMERA_PIN,LOW);
}
