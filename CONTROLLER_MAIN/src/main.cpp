#include <Arduino.h>
#include <DigitalIO.h>
#include <delay_cycles_avr.h>

const int movementout = 6;
const int cam = 4;
const int auxout = 5;
const long clockrate = 16000000;
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
    Serial.readBytes(matlab_input, sizeof(matlab_input));
    trial_length_minutes = strtok(matlab_input,",");
    int trial_length_minutes_int = atoi(trial_length_minutes);
    sampling_interval_ms = strtok(NULL,",");
    int sampling_interval_ms_int = atoi(sampling_interval_ms);
    Serial.println(trial_length_minutes_int);
    Serial.println(sampling_interval_ms_int);

    long int nreps = floor(clockrate*trial_length_minutes_int*60);
    long int cycles_per_iter = floor(clockrate*sampling_interval_ms_int/1000);

    fastDigitalWrite(movementout, HIGH);
    fastDigitalWrite(auxout, HIGH);
    for (int i=0; i<nreps; i++) {
      fastDigitalWrite(cam, HIGH);
      _delay_cycles(16000); // 1 millisecond
      fastDigitalWrite(cam, LOW);
      _delay_cycles(cycles_per_iter-16000);
      //Arduino Uno has 16 Mhz clock cycles, may vary if a different board is used
      //So, to delay for 49 milliseconds do 16Mhz*0.049s (may not be 16Mhz depending on the board)
    }
    fastDigitalWrite(movementout, LOW);
    fastDigitalWrite(auxout, LOW);
  }
}
