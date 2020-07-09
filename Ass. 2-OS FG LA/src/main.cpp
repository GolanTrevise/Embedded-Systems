/* Multifunction Device

By Chris Eley 2020 */
//Including all required libraries
#include <Arduino.h>                                                                              
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

 //Storing the screen parameters to allow easy recall later
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);                         

//Initialising the differing Machine states
enum states {startScreen, menuScreen, oScope, funcGen, logicAn, triWave, squWave, sinWave};       

enum states mode;

// Initialising global variables and defining pins
volatile bool buttonPressed = 0;
uint32_t debouncedelay_ms = 80;
int32_t i = 0;
uint32_t DACInput = 0;
uint16_t incomingByte = 0;

#define potPin A0
#define buttonPin 10
#define DACPin A14
#define ADCPin A1

// Bitmap array for sinewave image
const unsigned char Sinewaveimage [] PROGMEM = {
	// 'Sinewave, 100x45px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x04, 0x00, 0x02, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x40, 0x00, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 
	0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x00, 0x00, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x00, 0x00, 0x20, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x20, 
	0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x40, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x40, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x00, 0x80, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 
	0x80, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x80, 0x00, 0x00, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
	0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 
	0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x40, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x08, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00, 0x41, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00
};

// Function prototypes
void start();
void menu();
void scope();
void fgmenu();
void tri();
void squ();
void sin();
void analyse();
void ISRButtonPressed();

// Initialising serial communications, I2C communications, and stating pin modes
void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  pinMode(buttonPin, INPUT_PULLUP); // setting internal pull up resistors
  attachInterrupt(buttonPin, ISRButtonPressed, FALLING);  // interrupt for button
}

// void loop handles the Finite State Machine state navigation
void loop()
{
  switch (mode)
  {
  // Checks serial buffer for incoming messages
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
  }

  // Start screen state. Prompts user to move to menu screen
  case startScreen:
    
    if((buttonPressed == (true)) or (incomingByte == 65))
    {
      incomingByte = 0;
      buttonPressed = false;
      mode = menuScreen;
    }
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    start();
    break;

  // menu screen, allows user to select desired function
  case menuScreen:
    
    if (((buttonPressed == true) & (analogRead(potPin) <= 350)) or (incomingByte == 66))
    {
      incomingByte = 0; 
      i = 0;
      buttonPressed = false;
      mode = oScope;
    }
    else if (((buttonPressed == true) & (analogRead(potPin) > 350) & (analogRead(potPin) <= 700)) or (incomingByte == 67))
    { 
      buttonPressed = false;
      mode = funcGen;
    }
    else if (((buttonPressed == true) & (analogRead(potPin) > 700)) or (incomingByte == 71))
    {
      incomingByte = 0;
      buttonPressed = false;
      mode = logicAn;
    }
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    menu();
    

    break;

  // Oscilloscope function, displays visual representation of sampled voltage 
  case oScope:
    
    if((buttonPressed == true) or (incomingByte == 65))
    {
      incomingByte = 0;
      buttonPressed = false;
      mode = menuScreen;
    }
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    scope();
    break;  

  // Function generator function, allows user to select desired output signal, or return to menu
  case funcGen:
    
    if (((buttonPressed == true) & (analogRead(potPin) <= 250)) or (incomingByte == 68))
    {
      incomingByte = 0; 
      i = 0;
      buttonPressed = false;
      mode = triWave;
    }
    else if (((buttonPressed == true) & (analogRead(potPin) > 250) & (analogRead(potPin) <= 500)) or (incomingByte == 69))
    {
      incomingByte = 0; 
      i = 0;
      buttonPressed = false;
      mode = squWave;
    }
    else if (((buttonPressed == true) & (analogRead(potPin) > 500) & (analogRead(potPin) <= 750)) or (incomingByte == 70))
    {
      incomingByte = 0;
      i = 0;
      buttonPressed = false;
      mode = sinWave;
    }
    else if (((buttonPressed == true) & (analogRead(potPin) > 750)) or (incomingByte == 65))
    {
      incomingByte = 0;
      buttonPressed = false;
      mode = menuScreen;
    }
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    fgmenu();
    break;

  // Generated waves, produce desired waveform, display an artistic impression of waveform on screen
  case triWave:

    if ((buttonPressed == true) or (incomingByte == 67))
    {
      incomingByte = 0;
      buttonPressed = false;
      mode = funcGen;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Triangle Wave");
    display.drawFastHLine(0,8,78,WHITE);
    display.setCursor(0,10);
    display.println("Sample from pin 14");
    display.drawLine(10,60,40,20,WHITE);
    display.drawLine(40,20,70,60,WHITE);
    display.drawLine(70,60,100,20,WHITE);
    display.display();
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    tri();
    break;

  case squWave:

    if((buttonPressed == true) or (incomingByte == 67))
    {
      buttonPressed = false;
      mode = funcGen;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Square Wave");
    display.drawFastHLine(0,8,66,WHITE);
    display.setCursor(0,10);
    display.println("Sample from pin 14");
    display.drawFastHLine(10,60,30,WHITE);
    display.drawFastVLine(40,20,40,WHITE);
    display.drawFastHLine(40,20,30,WHITE);
    display.drawFastVLine(70,20,40,WHITE);
    display.drawFastHLine(70,60,30,WHITE);
    display.drawFastVLine(100,20,40,WHITE);
    display.display();
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    squ();
    break;
    
  case sinWave:
    if((buttonPressed == true) or (incomingByte == 67))
    {
      incomingByte = 0;
      buttonPressed = false;
      mode = funcGen;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Sine Wave");
    display.drawFastHLine(0,8,54,WHITE);
    display.setCursor(0,10);
    display.println("Sample from pin 14");
    display.drawBitmap(10,17,Sinewaveimage, 100, 45, WHITE);
    display.display();
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    sin();
    break;
    
   case logicAn:
    if((buttonPressed == true) or (incomingByte == 65))
    {
      incomingByte = 0;
      buttonPressed = false;
      mode = menuScreen;
    }
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
    analyse();
    break;

  default:
    break;
  }
}


void start(){
  display.clearDisplay();
  display.fillRoundRect(0, 0, 127, 43, 5, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(INVERSE);
  display.setCursor(4,10);
  display.print("Welcome to C. Eley's");
  display.setCursor(4,25);
  display.print("Multifunction Device");
  display.drawRoundRect(2, 2, 123, 39, 3, BLACK);
  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print("Press button to go to");
  display.setCursor(19, 53);
  display.print("the Main Menu");
  display.display();
}
void menu(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("Main Menu");
  display.drawFastHLine(10,16,106,WHITE);
  display.setTextSize(1);
  display.setCursor(20 , 21);
  display.println("Oscilloscope");
  display.setCursor(20 , 33);
  display.println("Function Generator");
  display.setCursor(20 , 45);
  display.println("Logic Analyser");
  if (analogRead(potPin) <= 350){ 
      display.drawRect(0,20,19,30,BLACK);
      display.fillCircle(10,24,5,WHITE);
    }
  else if ((analogRead(potPin) > 350) & (analogRead(potPin) <= 700)){ 
      display.drawRect(0,20,19,30,BLACK);
      display.fillCircle(10,36,5,WHITE);
    }
  else if (analogRead(potPin) > 700){
      display.drawRect(0,20,19,30,BLACK);
      display.fillCircle(10,48,5,WHITE);
    }
  display.display();
}

void scope(){
  
  static uint32_t prevTime_ms = 0;
  static int32_t currDrawVal = 0;
  static int32_t prevDrawVal = 0;
  static int32_t volts_mV = 0;
  uint32_t sampleRate_ms = 20;
  int32_t potVal = analogRead(potPin);

  if (i == 0){
    display.clearDisplay();
  }

  if (millis() - prevTime_ms >= sampleRate_ms ){
  currDrawVal = map(potVal, 0, 1023, 62, 7);
  volts_mV = map(potVal, 0, 1023, 0, 3300);

  display.drawLine(i-1, prevDrawVal, i, currDrawVal, SSD1306_WHITE);
  potVal = analogRead(potPin);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("V=");
  display.fillRect(12, 0, 38, 8, SSD1306_BLACK);
  display.setCursor(12, 0);
  display.print(volts_mV);
  display.setCursor(36, 0);
  display.print("mV SampleT=");
  display.fillRect(102, 0, 12, 8, SSD1306_BLACK);
  display.setCursor(102, 0);
  display.print(sampleRate_ms);
  display.setCursor(114, 0);
  display.print("ms");
  display.display();

  prevDrawVal = currDrawVal;
  i++;
  prevTime_ms = millis(); 
  Serial.println((float)volts_mV);
  }

  if (i >= SCREEN_WIDTH){
    i = 0;
  }
}

void fgmenu(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(7,0);
  display.println("Function Gen. Menu");
  display.fillRoundRect(1,8,125,51,4,WHITE);
    if (analogRead(potPin) <= 250){ 
      display.fillRoundRect(10,11,82,9,3,BLACK);
    }
  else if ((analogRead(potPin) > 250) & (analogRead(potPin) <= 500)){ 
      display.fillRoundRect(10,21,82,9,3,BLACK);
    }
  else if ((analogRead(potPin) > 500) & (analogRead(potPin) <= 750)){
      display.fillRoundRect(10,32,82,9,3,BLACK);
    }
  else if (analogRead(potPin) > 750){
      display.fillRoundRect(10,41,82,9,3,BLACK);
    }
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(13 , 12);
  display.println("Triangle Wave");
  display.setCursor(13 , 22);
  display.println("Square Wave");
  display.setCursor(13 , 33);
  display.println("Sine Wave");
  display.setCursor(13 , 42);
  display.println("Main Menu");
    
  display.display();
}
void tri(){
  static uint16_t previ = 0;

  if (i <= 0){
    previ = i;
    i++;
  }
  else if (i >= 64){
    previ = i;
    i--;
  }
  else if (i - previ > 0){
    previ = i;
    i++;
  }
  else if (previ - i > 0){
    previ = i;
    i--;
  }
  DACInput = (i * 4);
  analogWrite(DACPin, DACInput);
  Serial.println(analogRead((float)ADCPin));
}

void squ(){
  if (i >= 127){
    i = 0;
  }
  if (i > 64){
    DACInput = 255;
  }
  else if (i <= 64){
    DACInput = 0;
  }
  i++;
  analogWrite(DACPin, DACInput);
  Serial.println(analogRead((float)ADCPin));
}

void sin(){
  DACInput = (127 + (127 * sin((double)i/100)));
  analogWrite(DACPin, DACInput);
  i = i+3;
  Serial.println(analogRead((float)ADCPin));
}

void analyse(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Logic analyser");
  display.println("this is a placeholder");
  display.println("press button");
  display.display();
}


void ISRButtonPressed()
{
  noInterrupts();
  uint32_t currtime_ms = millis();
  static uint32_t prevtime_ms = 0;

  if ((currtime_ms - prevtime_ms) > debouncedelay_ms){
    buttonPressed = true;
    prevtime_ms = currtime_ms;
  }
  interrupts();

}