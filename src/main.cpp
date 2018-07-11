#include <Arduino.h>
#include "main.h"
#include "Timer.h"

static uint8_t CAMERA_PIN = 1;
static uint8_t MOTION_PIN = 2;
Timer t;

void setup() {
  Serial.begin(230200);
  pinMode(MOTION_PIN,OUTPUT);
  pinMode(CAMERA_PIN,OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:

    t.update();
}
