#include <Arduino.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

/* Pin 13 has an LED connected on most Arduino boards.
Pin 11 has the LED on Teensy 2.0
Pin 6  has the LED on Teensy++ 2.0
Pin 13 has the LED on Teensy 3.0
give it a name: */

const uint8_t led = 13;     //unsigned integer 8 bit data type- designating pin, no negative pins, limited number of pins
//#define led 13                //alternate way of setting 13 to LED- uses less memory, pin number cannot be changed iin code

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}