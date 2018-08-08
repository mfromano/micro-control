
#include <Arduino.h>
#include <DigitalIO.h>

const int movementmode = 6;
const int ttl = 4;
char *trial_length_minutes;
char *sampling_interval_ms;
char matlab_input[50];
volatile double delay_time;
double ms_time;
double micro_time;
void startExperiment(long int nreps, float sampling_interval_ms_int);

void setup() {
  fastPinMode(movementmode, OUTPUT);
  fastPinMode(ttl, OUTPUT);
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
    fastDigitalWrite(movementmode, HIGH);
    fastDigitalWrite(ttl, LOW);
    delay(500);
    startExperiment(nreps, sampling_interval_ms_int);
    fastDigitalWrite(movementmode, LOW);
  }
}

void startExperiment(long int nreps,float sampling_interval_ms_int) {
  double start;
  double fin;
  double experiment_start = micros();
  delay(sampling_interval_ms_int);
  start = micros();
  for (int i=0; i<nreps; i++) {
    fin = micros();
    Serial.println(((float)(fin-experiment_start))/1000000.0, 10);
    fastDigitalWrite(ttl, HIGH);
    delayMicroseconds(1000);
    fastDigitalWrite(ttl, LOW);
    delay_time = sampling_interval_ms_int-(micros()-start)/1000.0;
    delay(floor(delay_time));
    delayMicroseconds(1000.0*(delay_time-floor(delay_time)));
    start = micros();
  }
}
