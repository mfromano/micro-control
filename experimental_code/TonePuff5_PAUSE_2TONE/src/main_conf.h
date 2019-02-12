#include <Arduino.h>
#ifndef IS_CONFIG

#define IS_CONFIG

bool isRunning = false;

uint16_t FQ1 = 9500;
uint16_t FQ2 = 1000;

char matlabdata[200];
char delimiter = ',';
const uint8_t decimals = 10;
float TRIAL_LENGTH; // ms
uint8_t NO_TRIALS = 10;

const uint8_t PUFF_PIN = 3; // pin to use for PUFF
float PUFF_START = 12050; // ms
float PUFF_LENGTH = 100.0; //in ms
bool PUFF = false;

const uint8_t LED_PIN = 1; // pin to use for LED
bool LED = false;

float TONE1_START = 11100; // ms
float TONE1_LENGTH = 700.0;
bool TONE1 = false;

float TONE2_START = 11100; // ms
float TONE2_LENGTH = 700.0;
bool TONE2 = false;

const uint32_t CAMERA_PULSE_MIN_MICROS = 1000;

const uint8_t CAMERA_PIN = 6;
const uint8_t CAMERA_FQ = 20; // Hz
const float jitter = 1000.0/float(CAMERA_FQ)/2.0;
const bool CAMERA_ON_STATE = true;

const uint8_t AMP_PIN = 5;

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=189,176
AudioOutputAnalog        dac1;           //xy=552,179
AudioConnection patchCord1(sine1,dac1);
// end automatically generated code

volatile time_t curr_t;
volatile time_t exp_t;
elapsedMicros frame_t;
elapsedMicros experiment_t;
elapsedMicros trial_t;
IntervalTimer trial_timer;

uint16_t frame_no;
uint8_t trial_no;

typedef struct {
  uint16_t frame_in_trial = 0;
  uint32_t trial_time = 0;
  uint32_t experiment_time = 0;
  uint8_t trial_number = 0;
  bool puff_on = false;
  bool tone1_on = false;
  bool tone2_on = false;
  bool led_on  = false;

} frame_data;

void sendData(frame_data frame);
void begin(float ntrials, float trial_length);
void capture();
void updateParams();
void initializeExpParams();
void togglePUFF(time_t t);
void toggleTONE(time_t t, float TONE_START, float TONE_LENGTH, long fq, uint8_t tone_no);
void endCollection();


#endif
