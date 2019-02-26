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
#include <main_conf.h>


void setup() {
  fastPinMode(PUFF_PIN, OUTPUT);
  fastPinMode(CAMERA_PIN, OUTPUT);
  fastDigitalWrite(CAMERA_PIN, LOW);
  fastPinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  AudioMemory(128);
  sine1.frequency(FQ1);
  sine1.amplitude(0);
}

void loop(){
 if (!isRunning && (Serial.available() > 0)) {
    // add in Serial comprehension/parsing here once we find out what the user might want
    Serial.readBytes(matlabdata, sizeof(matlabdata)); //used from controller_main_synchronous file
    Serial.flush();
    char *ntrials_str = strtok(matlabdata,",");
    float ntrials  = atof(ntrials_str);
    char *trial_length_str = strtok(NULL,",");
    float trial_length = atof(trial_length_str);

    updateParams();

    Serial.println(String(ntrials) + ',' + String(trial_length) + ',' + String(PUFF_START) +
      ',' + String(PUFF_LENGTH) + ',' + String(TONE1_START) + ',' + String(TONE1_LENGTH) + ',' +
      String(FQ1) + ',' + String(TONE2_START) + ',' + String(TONE2_LENGTH) + ',' + String(FQ2));

    begin(ntrials, trial_length);
    fastDigitalWrite(CAMERA_PIN, !CAMERA_ON_STATE);
  }
}

void begin(float ntrials, float trial_length) {
    NO_TRIALS = uint8_t(ntrials);
    TRIAL_LENGTH = trial_length;
    fastPinMode(AMP_PIN,OUTPUT); // add amplifier
    fastDigitalWrite(AMP_PIN,HIGH);
    delay(10);
    initializeExpParams();
    char stopTrial[50];
    fastDigitalWrite(CAMERA_PIN, LOW);
    float interval_t = 1000000.0/(float)CAMERA_FQ;

    while (isRunning) {
        while (frame_t < interval_t) {
          ;
        }
        frame_t -= interval_t;
        capture();
        if (Serial.available() > 0) {
          Serial.readBytes(stopTrial,sizeof(stopTrial));
          if (strcmp(stopTrial,"STOP")) {
            Serial.flush();
            endCollection();
          }
        }
      }
    }

void updateParams() {
  char *PUFF_START_char = strtok(NULL,",");
  PUFF_START = atof(PUFF_START_char);
  char *PUFF_LENGTH_char = strtok(NULL,",");
  PUFF_LENGTH = atof(PUFF_LENGTH_char);

  char *TONE1_START_char = strtok(NULL,",");
  TONE1_START = atof(TONE1_START_char);
  char *TONE1_LENGTH_char = strtok(NULL,",");
  TONE1_LENGTH = atof(TONE1_LENGTH_char);
  char *FQ_1_char = strtok(NULL,",");
  FQ1 = strtol(FQ_1_char,NULL,0);
  char *TONE1_AMP_char = strtok(NULL,",");
  TONE1_AMP = atof(TONE1_AMP_char);

  char *TONE2_START_char = strtok(NULL,",");
  TONE2_START = atof(TONE2_START_char);
  char *TONE2_LENGTH_char = strtok(NULL,",");
  TONE2_LENGTH = atof(TONE2_LENGTH_char);
  char *FQ_2_char = strtok(NULL,",");
  FQ2 = strtol(FQ_2_char,NULL,0);
  char *TONE2_AMP_char = strtok(NULL,",");
  TONE2_AMP = atof(TONE2_AMP_char);
}

void initializeExpParams() {
  experiment_t = 0;
  trial_t = 0;
  trial_no = 0;
  frame_no = 0;
  frame_t = 0;
  isRunning = true;
  fastDigitalWrite(LED_PIN, LOW);
  fastDigitalWrite(PUFF_PIN, LOW);
  fastDigitalWrite(CAMERA_PIN, LOW);
  TONE1 = false;
  TONE2 = false;
  LED = false;
  PUFF = false;
  sine1.amplitude(0);
}

void endCollection() {
  trial_timer.end();
  fastDigitalWrite(AMP_PIN,LOW);
  fastDigitalWrite(LED_PIN, LOW);
  fastDigitalWrite(PUFF_PIN, LOW);
  fastDigitalWrite(CAMERA_PIN, !CAMERA_ON_STATE);
  TONE1 = false;
  TONE2 = false;
  LED = false;
  PUFF = false;
  sine1.amplitude(0);
  isRunning = false;
  Serial.println("END\n");
}

void capture() {
  elapsedMicros campulse_t = 0;
  fastDigitalWrite(CAMERA_PIN, CAMERA_ON_STATE);
  curr_t = trial_t;
  exp_t = experiment_t;
  if (trial_no == 0) {
    trial_no++;
    trial_t = 0;
    experiment_t = 0;
    curr_t = trial_t;
    exp_t = experiment_t;
  }
  frame_no++;


  if ((curr_t/1000.0+ jitter > TRIAL_LENGTH)) {
    trial_no++;
    if (trial_no > NO_TRIALS) {
      endCollection();
      return;
    }
    trial_t = 0;
    curr_t = trial_t;
  }

  toggleTONE(curr_t, TONE1_START, TONE1_LENGTH, FQ1, 1, TONE1_AMP);
  toggleTONE(curr_t, TONE2_START, TONE2_LENGTH, FQ2, 2, TONE2_AMP);
  togglePUFF(curr_t);

  frame_data curr_frame = {frame_no, curr_t, exp_t, trial_no, PUFF, TONE1, TONE2, LED};
  while (campulse_t < CAMERA_PULSE_MIN_MICROS){;}
  fastDigitalWrite(CAMERA_PIN, !CAMERA_ON_STATE);

  sendData(curr_frame);
}

void togglePUFF(time_t t) {
  if ((t/1000.0 +jitter > PUFF_START) && (t/1000.0 +jitter < (PUFF_START+PUFF_LENGTH))) {
    PUFF = true;
    fastDigitalWrite(PUFF_PIN, HIGH);
  } else if ((t/1000.0 + jitter > (PUFF_START + PUFF_LENGTH)) && PUFF) {
    PUFF = false;
    fastDigitalWrite(PUFF_PIN, LOW);
  }

}

void toggleTONE(time_t t, float TONE_START, float TONE_LENGTH, long fq, uint8_t tone_no, float TONE_AMP){
  // update tone
  if ((t/1000.0 + jitter > TONE_START) && (t/1000.0+jitter < (TONE_START+TONE_LENGTH))) {
    if (tone_no == 1) {
        TONE1 = true;
    }
    else {
        TONE2 = true;
    }
    LED = true;
    sine1.frequency(fq);
    sine1.amplitude(TONE_AMP);
    fastDigitalWrite(LED_PIN, HIGH);

  } else if ((t/1000.0 + jitter > (TONE_START + TONE_LENGTH)) && (TONE1 || TONE2)) {
    TONE1 = false;
    TONE2 = false;
    LED = false;
    sine1.amplitude(0);
    fastDigitalWrite(LED_PIN, LOW);

  }
}

void sendData(frame_data frame) {
  String exp_time = String(frame.experiment_time);
  String tri_time = String(frame.trial_time);
  String trial_no = String(frame.trial_number);
  String puff = String(frame.puff_on ? "1": "0");
  String tone1 = String(frame.tone1_on ? "1": "0");
  String tone2 = String(frame.tone2_on ? "1": "0");
  String led = String(frame.led_on ? "1": "0");
  Serial.println( exp_time + delimiter + tri_time + delimiter + trial_no + delimiter + puff + delimiter + tone1 + delimiter + tone2 + delimiter + led);
}
