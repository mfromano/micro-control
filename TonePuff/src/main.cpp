#include <math.h> //this was included for the sine function
#include <Arduino.h> //this was included for the Serial.readbytes command
#include <vector>
#include <ellapsedMillis.h>
#include <IntervalTimer.h>

char matlabdata[50];
const uint8_t decimals = 10;
const float TRIAL_LENGTH = 30000; // ms
const uint8_t NO_TRIALS = 10;

const uint8_t PUFF_PIN = 3; // pin to use for PUFF
const float PUFF_START = 12000; // ms
const float PUFF_LENGTH = 1000; //in ms
bool PUFF = FALSE;

const uint8_t TONE_PIN = 4;
const float TONE_START = 10000; // ms
const float TONE_LENGTH = 100; //in ms
bool TONE = FALSE;

const uint8_t CAMERA_PIN = 5;
const uint8_t CAMERA_FQ = 50; // Hz
ellapsedMillis experiment_t;
ellapsedMillis trial_t;
IntervalTimer trial_timer;
int frame_no;
int trial_no;

typedef struct {
  uint16_t frame_in_trial = 0;
  float trial_time;
  float experiment_time = 0;
  uint8_t trial_number = 0;
  bool puff_on = FALSE;
  bool tone_on = FALSE;

} frame_data;

void setup() {
  fastPinMode(PUFF_PIN, OUTPUT);
  fastPinMode(TONE_PIN, OUTPUT);
  fastPinMode(CAMERA_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop(){
  if (!isRunning && !Serial.read()) {
    //add in Serial comprehension/parsing here once we find out what the user might want
    Serial.readBytes(matlabdata, sizeof(matlabdata)); //used from controller_main_synchronous file
    samplefreq = strtok(matlabdata,",");
    float samplefreq = atof(samplefreq);
    loopfreq = strtok(NULL,",");
    float loopfreq = atof(loopfreq);
    begin();
  }
}

void begin() {
    experiment_t = 0;
    trial_t = 0;
    trial_no = 1;
    frame_no = 0;
    trial_timer.begin(capture,1000000.0/(float)CAMERA_FQ);
}
void endCollection() {
  trial_timer.end();
}

void capture() {
  frame_no++;
  if (trial_t > TRIAL_LENGTH) {
    trial_no++;
    trial_t = 0;
  }
  // update tone
  if ((trial_time > TONE_START) && (trial_time < (TONE_START+TONE_LENGTH))) {
    TONE = TRUE;
    fastDigitalWrite(TONE_PIN,HIGH);
  } else if ((trial_time > (TONE_START + TONE_LENGTH)) && tone_on) {
    TONE = FALSE;
    fastDigitalWrite(TONE_PIN, LOW);
  }
  if ((trial_time > PUFF_START) && (trial_time < (PUFF_START+PUFF_LENGTH)) {
    PUFF = TRUE;
    fastDigitalWrite(PUFF_PIN, HIGH);
  } else if ((trial_time > (PUFF_START + PUFF_LENGTH)) && puff_on) {
    PUFF = FALSE;
    fastDigitalWrite(PUFF_PIN, LOW);
  }

  fastDigitalWrite(CAMERA_PIN,HIGH);
  delay(1);
  fastDigitalWrite(CAMERA_PIN,LOW);

  // create output struct
  frame_data curr_frame = {frame_no, trial_t, experiment_t, trial_no, PUFF,
    TONE};
  sendData(curr_frame);
}

void sendData(frame_data frame) {
  String exp_time = String(frame.experiment_time, decimals);
  String tri_time = String(frame.trial_time, decimals);
  String trial_no = String(frame.trial_number);
  String puff = String(frame.puff_on);
  String tone = String(frame.tone_on);
  Serial.println( exp_time + tri_time + trial_no + puff + delimeter + tone);
}
