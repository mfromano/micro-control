#include <Arduino.h>
#include "main.h"
#include "Timer.h"

CAMERA_PIN = 1;


void setup() {
  Serial.begin(230200);
  pinMode(MOTION_PIN,OUTPUT);
  pinMode(CAMERA_PIN,OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:

    t.update();
}
