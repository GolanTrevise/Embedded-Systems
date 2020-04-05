/*
Use button with external interrupt to switch LED on/off
20200406 Chris Eley
*/

#include <Arduino.h>

#define buttonPin 12
#define ledPin 13

const uint32_t printTime_ms = 100;

//function prototype
void ISR_Button();

//boolean ButtonPressed = false;
volatile boolean LED_State = false;

volatile uint32_t pressCounter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(buttonPin, ISR_Button, RISING);
}

void loop() {
  static uint32_t prevTime_ms = 0;
  uint32_t currTime_ms = millis();

  digitalWrite(ledPin, LED_State);

    if((currTime_ms - prevTime_ms) >= printTime_ms){
      Serial.println(pressCounter);
      prevTime_ms = currTime_ms;
      }
}

void ISR_Button(){
  LED_State = !LED_State;
  pressCounter++;
}