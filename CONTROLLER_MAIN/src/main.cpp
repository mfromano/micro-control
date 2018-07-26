#include <Arduino.h>
#include <DigitalIO.h>

const int movementout = 6;
const int cam = 4;
const int auxout = 5;
char *trial_length_minutes;
char *sampling_interval_ms;
char matlab_input[50];

void setup() {
  fastPinMode(movementout, OUTPUT);
  fastPinMode(cam, OUTPUT);
  fastPinMode(auxout, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if(Serial.available() > 0){ //checking for input from matlab
    delay(0.5);
    Serial.readBytes(matlab_input, sizeof(matlab_input));
    trial_length_minutes = strtok(matlab_input,",");
    float trial_length_minutes_int = atof(trial_length_minutes);
    sampling_interval_ms = strtok(NULL,",");
    float sampling_interval_ms_int = atof(sampling_interval_ms);
    long int nreps = floor(trial_length_minutes_int*60*1000/sampling_interval_ms_int);
    Serial.println(nreps);
    Serial.println(sampling_interval_ms_int);
    double start;
    double fin;
    fastDigitalWrite(movementout, HIGH);
    fastDigitalWrite(auxout, HIGH);
    for (int i=0; i<nreps; i++) {
      start = micros();
      fastDigitalWrite(cam, HIGH);
      delayMicroseconds(1000);
      fastDigitalWrite(cam, LOW);
      fin = micros();
      delay(sampling_interval_ms_int-(fin-start)/1000);
    }
    fastDigitalWrite(movementout, LOW);
    fastDigitalWrite(auxout, LOW);
  }
}
