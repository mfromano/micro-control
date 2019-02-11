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

#endif
