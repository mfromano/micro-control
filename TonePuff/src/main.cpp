#include <math.h> //this was included for the sine function
#include <ctime.h>
#include <Arduino.h> //this was included for the Serial.readbytes command
#include <vector>
char matlabdata[50];


void setup() {
  samplefreq = 0;
  loopfreq = 0;
  static double pi = atan(1)*4;
  std::vector<int> v;
  fastPinMode(3,OUTPUT);
}

/*Corresponding matlab code (needs to be put into the MATLAB file as new GUI text box)
obj = serial(deviceCOMPort,'BaudRate',115200)
f = fopen(obj);
fwrite(f,sprintf('%s,%s',samplefrequency,loopfrequency));
*/

void loop(){
  clock_t t
  Serial.readBytes(matlabdata, sizeof(matlabdata)); //used from controller_main_synchronous file
  samplefreq = strtok(matlabdata,",");
  float samplefreq = atof(samplefreq);
  loopfreq = strtok(NULL,",");
  float loopfreq = atof(loopfreq);
  intervals = samplefreq*2*pi;
  t = clock();
  while (((float)(clock()-t))/CLOCKS_PER_SEC) > 0 && ((float)(clock()-t))/CLOCKS_PER_SEC) <= (2*pi)) {
    sinewave = sin(t*loopfreq);
    point = loopfreq/(2*pi*intervals); //tells how many points due to the sampling frequency
    count = 1;
    if (((float)(clock()-t))/CLOCKS_PER_SEC) == (point*count)) {
      v.push_back(count) = sin(t*loopfreq); //this way, vector v will have all the values of the sine function corresponding to the sampling frequency
      tone(3, v.push_back(count)); //this should have this particular tone play until the next tone comes along
      count++
    }
  }
}
