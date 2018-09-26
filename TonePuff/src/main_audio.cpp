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

const uint16_t FQ = 9500;
char matlabdata[50];
char delimiter = ',';
const uint8_t decimals = 10;
float TRIAL_LENGTH = 30000; // ms
uint8_t NO_TRIALS = 10;

const uint8_t PUFF_PIN = 3; // pin to use for PUFF
// const float PUFF_START = 11700; // ms
const float PUFF_START = 12050; // ms
const float PUFF_LENGTH = 100.0; //in ms
bool PUFF = false;

const uint8_t LED_PIN = 1; // pin to use for LED
bool LED = false;

const float TONE_START = 11100; // ms
// const float TONE_LENGTH = 350.0; //in ms
const float TONE_LENGTH = 700.0;
bool TONE = false;

const uint8_t CAMERA_PIN = 6;
const uint8_t CAMERA_FQ = 20; // Hz
elapsedMicros experiment_t;
elapsedMicros trial_t;
IntervalTimer trial_timer;

const uint8_t AMP_PIN = 5;

uint16_t frame_no;
uint8_t trial_no;

typedef struct {
  uint16_t frame_in_trial = 0;
  uint32_t trial_time = 0;
  uint32_t experiment_time = 0;
  uint8_t trial_number = 0;
  bool puff_on = false;
  bool tone_on = false;
  bool led_on  = false;

} frame_data;

void sendData(frame_data frame);
void begin(float ntrials, float trial_length);
void capture();

void setup() {
  fastPinMode(PUFF_PIN, OUTPUT);
  fastPinMode(CAMERA_PIN, OUTPUT);
  fastPinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  AudioMemory(128);
  // dac1.analogReference(EXTERNAL);
  sine1.frequency(FQ);
  sine1.amplitude(0);
}

void loop(){
 if (!isRunning && (Serial.available() > 0)) {
    // add in Serial comprehension/parsing here once we find out what the user might want
    Serial.readBytes(matlabdata, sizeof(matlabdata)); //used from controller_main_synchronous file
    char *ntrials_str = strtok(matlabdata,",");
    float ntrials  = atof(ntrials_str);
    char *trial_length_str = strtok(NULL,",");
    float trial_length = atof(trial_length_str);
    Serial.println(String(ntrials) + ',' + String(trial_length));
    begin(ntrials, trial_length);
  }
}

void begin(float ntrials, float trial_length) {
    NO_TRIALS = uint8_t(ntrials);
    TRIAL_LENGTH = trial_length;
    fastPinMode(AMP_PIN,OUTPUT); // add amplifier
    fastDigitalWrite(AMP_PIN,HIGH);
    delay(10);
    experiment_t = 0;
    trial_t = 0;
    trial_no = 1;
    frame_no = 0;
    trial_timer.begin(capture,1000000.0/(float)CAMERA_FQ);
    isRunning = true;
}
void endCollection() {
  trial_timer.end();
  fastDigitalWrite(AMP_PIN,LOW);
  isRunning = false;
  Serial.println("END\n");
}

void capture() {
  frame_no++;
  if (trial_t/1000.0 > TRIAL_LENGTH) {
    trial_no++;
    if (trial_no > NO_TRIALS) {
      endCollection();
      return;
    }
    trial_t = 0;
  }
  // update tone
  if ((trial_t/1000.0 > TONE_START) && (trial_t/1000.0 < (TONE_START+TONE_LENGTH))) {
    TONE = true;
    LED = true;
    sine1.amplitude(0.05);
    fastDigitalWrite(LED_PIN, HIGH);
  } else if ((trial_t/1000.0 > (TONE_START + TONE_LENGTH)) && TONE) {
    TONE = false;
    LED = false;
    sine1.amplitude(0);
    fastDigitalWrite(LED_PIN, LOW);

  }
  if ((trial_t/1000.0 > PUFF_START) && (trial_t/1000.0 < (PUFF_START+PUFF_LENGTH))) {
    PUFF = true;
    fastDigitalWrite(PUFF_PIN, HIGH);
  } else if ((trial_t/1000.0 > (PUFF_START + PUFF_LENGTH)) && PUFF) {
    PUFF = false;
    fastDigitalWrite(PUFF_PIN, LOW);
  }

  frame_data curr_frame = {frame_no, trial_t, experiment_t, trial_no, PUFF, TONE, LED};
  sendData(curr_frame);

  fastDigitalWrite(CAMERA_PIN,HIGH);
  delay(1);
  fastDigitalWrite(CAMERA_PIN,LOW);

}

void sendData(frame_data frame) {
  String exp_time = String(frame.experiment_time);
  String tri_time = String(frame.trial_time);
  String trial_no = String(frame.trial_number);
  String puff = String(frame.puff_on ? "true": "false");
  String tone = String(frame.tone_on ? "true": "false");
  String led = String(frame.led_on ? "true": "false");
  Serial.println( exp_time + delimiter + tri_time + delimiter + trial_no + delimiter + puff + delimiter + tone + delimiter + led);
}
