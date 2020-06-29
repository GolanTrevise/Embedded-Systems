#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum states {startScreen, menuScreen, oScope, funcGen, logicAn, triWave, squWave, sinWave};

enum states mode;

volatile bool buttonPressed = 0;
uint32_t debouncedelay_ms = 50;
int32_t i = 0;

#define potPin 14
#define buttonPin 10


void start();
void menu();
void scope();
void fgmenu();
void tri();
void squ();
void sin();
void analyse();
void ISRButtonPressed();

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

void loop()
{
  switch (mode)
  {
  case startScreen:
    
    if(buttonPressed == (true))
    {
      buttonPressed = false;
      mode = menuScreen;
    }
    start();
    break;

  case menuScreen:
    
    if ((buttonPressed == true) & (analogRead(potPin) < 350))
    { 
      i = 0;
      buttonPressed = false;
      mode = oScope;
    }
    else if ((buttonPressed == true) & (analogRead(potPin) >= 350) & (analogRead(potPin) < 700))
    { 
      i = 0;
      buttonPressed = false;
      mode = funcGen;
    }
    else if ((buttonPressed == true) & (analogRead(potPin) >= 700))
    {
      buttonPressed = false;
      mode = logicAn;
    }
    menu();
    

    break;
    
  case oScope:
    
    if(buttonPressed == true)
    {
      buttonPressed = false;
      mode = menuScreen;
    }
    scope();
    break;  

  case funcGen:
    
    if(buttonPressed == true)
    { 
      buttonPressed = false;
      mode = menuScreen;
    }
    fgmenu();
    break;

  /* case triWave:
    tri();
    if(button_pressed == true)
    {
      mode = game_playing;
      button_pressed = false;
    }
    break;

  case squWave:
    squ();
    if(button_pressed == true)
    {
      mode = game_playing;
      button_pressed = false;
    }
    break;
    
  case sinWave:
    sin();
    if(button_pressed == true)
    {
      mode = game_playing;
      button_pressed = false;
    }
    break; */
    
   case logicAn:
    
    if(buttonPressed == true)
    {
      buttonPressed = false;
      mode = menuScreen;
    }
    analyse();
    break;

  default:
    break;
  }
}


void start(){
  /* Serial.println("Start-Press button to go to menu");  delay(100); */
  display.clearDisplay();
  display.fillRoundRect(5, 5, 121, 38, 5, SSD1306_WHITE);
  display.setTextSize(3);      // Normal 1:1 pixel scale
  display.setTextColor(BLACK); // Draw white text
  display.setCursor(22, 13);     // Start at top-left corner
  display.print("Start");
  display.setTextSize(1);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 45);     // Start at top-left corner
  display.print("Press button to go to         menu");
  display.display();
}
void menu(){
/*   Serial.print("Menu - ");  delay(100);
  Serial.println(analogRead(potPin)); */
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(10, 10);
  display.println("Menu");
  display.setCursor(10 , 20);
  display.println("0-350 = scope");
  display.setCursor(10 , 30);
  display.println("351-700 = func gen");
  display.setCursor(10 , 40);
  display.println("700+ = log an");
  display.setCursor(70 , 5);
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.println(analogRead(potPin));
  display.display();
}
void scope(){
  
  static int32_t prevTime_ms = 0;
  static int32_t currDrawVal = 0;
  static int32_t prevDrawVal = 0;
  static int32_t volts_mV = 0;

  if (i == 0){
    display.clearDisplay();
  }

  if (millis() - prevTime_ms >= 20 ){
  /* for (i = 0; i < 127; i++){ */

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(1, 1);     // Start at top-left corner
  display.println("Scope");
  int32_t potVal = analogRead(potPin);
  currDrawVal = map(potVal, 0, 1023, 62, 0);
  volts_mV = map(potVal, 0, 1023, 0, 3300);
  display.drawLine(i-1, prevDrawVal, i, currDrawVal, SSD1306_WHITE);
  display.fillRect(0, 8, 38, 10, SSD1306_BLACK);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(1, 10);     // Start at top-left corner
  display.print(volts_mV);
  display.print("mV");
  display.display();
  prevDrawVal = currDrawVal;
  /* delay(20); */
  display.display();
  i++; 
  }

  if (i >= SCREEN_WIDTH){
    i = 0;
  }
}

void fgmenu(){
  /* Serial.println("func gen");  delay(100); */
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(10, 10);     // Start at top-left corner
  display.println("Function generator-Press button to return to menu");
  display.display();
}
void tri(){
  
}
void squ(){
  
}
void sin(){
  
}
void analyse(){
/*   Serial.print("logic analyser - ");   delay(100); */
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(10, 10);     // Start at top-left corner
  display.println("Logic analyser-Press button to return to menu");
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