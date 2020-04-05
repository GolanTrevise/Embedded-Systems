#include <Arduino.h>

/*
Flicker Fusion 2020
Chris Eley
*/

#define LEDPin 13
#define ButtonPin 12

const uint32_t debouncetime_ms = 150;

uint8_t LEDState = false;
uint8_t CurrButtonState = LOW;
uint8_t PrevButtonState = LOW;
uint32_t ButtonCount = 0;


void setup() {                                                         
  Serial.begin(115200);             //initialises serial interface and sets Baud rate (115200) 
  pinMode(LEDPin, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);
}

void loop() {
  static uint32_t PrevTime_ms = 0;
  uint32_t        CurrTime_ms = millis();

  CurrButtonState = !digitalRead(ButtonPin);
  
  //detect rising edge and debounce time
  if((CurrButtonState == HIGH) && (PrevButtonState == LOW) && (CurrTime_ms - PrevTime_ms > debouncetime_ms)) {
    //toggle LED
    LEDState = !LEDState; 
    digitalWrite(LEDPin, LEDState);
    Serial.print("LEDState =");
    Serial.println(LEDState);
    //ButtonCount = (ButtonCount + 1);         //either add to buttoncount this way or use ++ButtonCount
    Serial.print("ButtonCount =");
    Serial.println(++ButtonCount);
    PrevTime_ms = CurrTime_ms;
  }
  
  PrevButtonState = CurrButtonState;
  

}