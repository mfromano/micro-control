#include <Arduino.h>
#include <DigitalIO.h>

const int movementmode = 6;
const int cam = 4;
const int movement_trigger = 5;
char *trial_length_minutes;
char *sampling_interval_ms;
char matlab_input[50];

void setup() {
  fastPinMode(movementmode, OUTPUT);
  fastPinMode(cam, OUTPUT);
  fastPinMode(movement_trigger, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if(Serial.available() > 0){ //checking for input from matlab
    delay(0.5);
    // read input from MATLAB function Userpopup
    Serial.readBytes(matlab_input, sizeof(matlab_input));

    //Parse input
    trial_length_minutes = strtok(matlab_input,",");
    float trial_length_minutes_int = atof(trial_length_minutes);
    sampling_interval_ms = strtok(NULL,",");
    float sampling_interval_ms_int = atof(sampling_interval_ms);
    long int nreps = floor(trial_length_minutes_int*60*1000/sampling_interval_ms_int);

    //Print these functions to MATLAB
    Serial.println(nreps);
    Serial.println(sampling_interval_ms_int);
    double start;
    double fin;
    fastDigitalWrite(movementmode, HIGH);
    delay(0.2);
    double experiment_start = micros();
    for (int i=0; i<nreps; i++) {
      start = micros();
      fastDigitalWrite(cam, HIGH);
      fastDigitalWrite(movement_trigger, HIGH);
      delayMicroseconds(1000);
      fastDigitalWrite(cam, LOW);
      fastDigitalWrite(movement_trigger, LOW);
      fin = micros();
      Serial.println((fin-experiment_start)/1000000, 10);
      delay(sampling_interval_ms_int-(fin-start)/1000);
    }
    fastDigitalWrite(movementmode, LOW);
  }
}
