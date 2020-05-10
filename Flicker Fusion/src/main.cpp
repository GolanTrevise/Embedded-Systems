#include <Arduino.h>

/*
Ass. 0 Flicker Fusion
By Chris Eley
*/

// Assigning the required pins
#define ledPin 13                                       
#define buttonPin 12

// Global Variables                     
uint32_t freq;                                          // Creating the variables that require a global scope
uint8_t ledState = LOW;                                 // Setting the LED state to low prior to setup

// Function prototype for the Interrupt Service Routine
void ISR_Button();                                      

// Setup
void setup() {
  Serial.begin(115200);                                 // Initialises serial interface and sets Baud rate (115200) 
  pinMode(buttonPin, INPUT_PULLUP);                     // Assigning the button pin Input status as well as activating the internal pull-up resistor
  pinMode(ledPin, OUTPUT);                              // Assigning the LED Pin output status
  digitalWrite(ledPin, ledState);                       // Ensures the LED is off prior to beginning the program
  attachInterrupt(buttonPin, ISR_Button, RISING);       // Initiallising the Interrupt function
}

// Main Program Loop
void loop() {
  static uint32_t prevTime_us = 0;                      // Creating a local variable to store the previous time (in micro seconds)
  uint32_t aRaw;                                        // Creates a variable to store the raw value of the analogue read
  uint32_t Pby2;                                        // A local variable to store the half cycle time of the LED

  aRaw = (analogRead(0));                               // Stores the value from the potentiometer

  if (aRaw < 250) {                                     // If the raw value is low (<250) then the Period is set to 20 times the
  Pby2 = ((aRaw)*10);                                   // value of aRaw
  } 

   if (aRaw >= 250) {                                   // If the raw value is higher (>=250) then the Period is set to 60 times the
  Pby2 = (((aRaw - 250)*30) + 2500);                    // value of aRaw   
  }

  if (micros() - prevTime_us >= Pby2) {                 // Checks if the set interval has passed  
    ledState = !ledState;                               // If conditions filled alters state of LED flage
    digitalWrite(ledPin,ledState);                      // Writes the LED flag state to the LED pin.
    prevTime_us = micros();                             // Sets the previous time to the current time to allow the cycle to start again                          
  }

  freq = (1000000 / (2*Pby2));                          // Calculates and stores the frequency of the LED
}

// Interrupt service routine
void ISR_Button(){
  static uint32_t prevMillisButton = 0;                 // Creates variable for debouncing inside the ISR 
  uint32_t currMillisButton = millis();                 // Stores current time upon entering ISR
  uint32_t debounceTime_ms = 50;                        // Sets the debounce time
    if ((currMillisButton - prevMillisButton) >= debounceTime_ms){    // Debounce check
      prevMillisButton = currMillisButton;              // Syncs the previous time the button was pushed to this button push
      Serial.print("Freq is: ");                        
      Serial.print(freq);                               // Prints current frequency to serial monitor
      Serial.println(" Hz");
    }   
}