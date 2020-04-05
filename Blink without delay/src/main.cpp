#include <Arduino.h>

/*
Blink with out delay
Turns on an LED on for one second, then off for one second repeatedly.
Without using Delay (a blocking function)
*/

const uint32_t interval = 2000;     //Sets LED on/off time   
const uint8_t ledPin = 11;          //unsigned integer 8 bit data type- designating pin, no negative pins, limited number of pins
uint8_t ledState = LOW;             //initialise variable
uint8_t buttonState = 0;            //initialise variable

void setup() {
  Serial.begin(115200);             //initialises serial interface and sets Baud rate (115200) 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,ledState);
}

void loop() {
  static uint32_t prevTime_ms = 0;                  //static qualifier allows scope of variable to be reduced while not resetting
  uint32_t currTime_ms = millis();                  //checks current system time
  uint32_t startIf_uc = micros();
  
  if (currTime_ms - prevTime_ms >= interval) {      //checks if the set interval has passed  
    
    if (ledState == LOW)                              //toggles ledState                            
    {ledState = HIGH;} 
    else { ledState = LOW; }
    //ledState = ~ledState;                           //"bitwise invert" works to toggle but isn't ideal
    //ledState = !ledState;                           //1st alternate method of toggling ledState

    Serial.print("Previous Time = "); Serial.print(prevTime_ms);
    Serial.print("ms Current Time = "); Serial.print(currTime_ms);
    Serial.print("ms Delta = "); Serial.print(currTime_ms - prevTime_ms);
    Serial.print("ms ledState = "); Serial.println(ledState);    
    prevTime_ms = currTime_ms;                        //saves the last time the state was changed  

    digitalWrite(ledPin,ledState);                    //changes the led State (assuming the interval has passed)
    //delay(10);
    Serial.print("If statement runtime = "); Serial.print(micros() - startIf_uc); Serial.println("us");
  }

}