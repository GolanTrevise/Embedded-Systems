#include <Arduino.h>

#define led 13
#define ledPin 6
#define buttonPin 12

//function prototype
void ISR_Button();

void setup()
{                
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  attachInterrupt(buttonPin, ISR_Button, FALLING);

  
}

int32_t freq;

void loop()                     
{
  int32_t val;
  
  static uint32_t prevMicros = 0;

  
  
  val = analogRead(0);
  freq = val/4;
  /*Serial.print("analog 0 is: ");
  Serial.println(val);
  Serial.print("Freq is: ");
  Serial.print(freq);
  Serial.println("Hz");*/
  analogWriteFrequency(ledPin, freq);
  analogWrite(ledPin, 123);
  //while((micros() - prevMicros) <= 100){  }
  delay(100);
  prevMicros = micros();
  
}

void ISR_Button(){
  Serial.print("Freq is: ");
  Serial.print(freq);
  Serial.println("Hz");
}

