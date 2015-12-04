
// Coffee Timer
// (cc)(BY SA) Rob Ives 2015 
http://www.robives.com


/*********************************************************************
Adafruit library for Monochrome OLEDs based on SSD1306 drivers
https://github.com/adafruit/Adafruit_SSD1306
Adafruit GFX library:
https://github.com/adafruit/Adafruit-GFX-Library  
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//------------------------------------------------------ eo Adafruit header
//define the four timer states.
#define WAITING 0 // waiting to start
#define TIMERONE 1 // timer one running
#define TIMERTWO 2 // timer two running
#define FINISHED 3 // timer complete

const long minute = 60000; // 60000 milliseconds in a minute
const int resetPin = 2; 
const int buttonPin = 3; 
const int ledPin =  13;      // the number of the LED pin

long startTime = 0;
long timeOne = 0;
long timeTwo = 0;
int timer = WAITING; // timer state. Initially waiting for first button press
int reset = 0;  // reset button state
int button = 0; // main button state

void setup()   {    
  pinMode(buttonPin, INPUT); //init button pin to input
  pinMode(resetPin, INPUT);  //init reset pin to input
  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output: 
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3c
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  printDigits(0);
  
}


void loop() {
   //-------- main loop -------
 switch ( timer){
   case WAITING: // initial state - waiting for first button press.
     display.clearDisplay();
     display.setCursor(10,00); 
     printDigits(0);
     display.display(); // display screen showing 00:00s
     button = digitalRead(buttonPin);
     if(button){ // button is pressed.
       timer = TIMERONE; // start the first timer 
       startTime = millis();//read the time the button is pressed.
       while (digitalRead(buttonPin)){
         //--- wait for button to be released.
       }
     }
   break;
   case TIMERONE: //running first timer
     timeOne = millis();
     button = digitalRead(buttonPin);
     reset = digitalRead(resetPin);
     display.clearDisplay();
     display.setCursor(10,00);
     printDigits(timeOne - startTime);
     display.display();
     //-----------------------------------
     if(reset){ //reset button was pressed. Clear screeen and start again
       timer = WAITING;
       while (digitalRead(resetPin)){
         //--- wait for reset button to be released.
       }
     }
     //------------------------------------
     if(button){ // button was pressed
         timer = TIMERTWO;  // start the second timer
         //timeTwo = millis(); //record the time for the start of the second timer
         while (digitalRead(buttonPin)){
           //--- wait for button to be released.
         }
       }
   break;
   case TIMERTWO: //running first timer
     timeTwo = millis();
     button = digitalRead(buttonPin);
     reset = digitalRead(resetPin);
     display.clearDisplay();
     display.setCursor(10,00);
     printDigits(timeOne - startTime); // display the first time time
     display.setCursor(10,32);
     printDigits(timeTwo - timeOne); // display the second timer on the line below
     display.display();
     //------------------------------------
     if(reset){ //reset button was pressed. Clear screeen and start again.32w
       timer = WAITING;
       while (digitalRead(resetPin)){
         //--- wait for reset button to be released.
       }
     }
     //------------------------------------
     if(button){ // button was pressed
         timer = FINISHED;  // start the second timer
         while (digitalRead(buttonPin)){
           //--- wait for button to be released.
         }
       }
     
   break;
    case FINISHED: //running first timer
     reset = digitalRead(resetPin);
     if(reset){ //reset button was pressed. Clear screeen and start again
       display.clearDisplay();
       display.setCursor(10,0);
       printDigits(0);
       display.display();
       timer = WAITING;
       while (digitalRead(resetPin)){
         //--- wait for reset button to be released.
       }
     } 
   break;  
   } // eo switch
  }// eo void loop
  
void printDigits(long timeInMillis){ 
  // convert the time from milliseconds to four digit SS:HH 
  // seconds and hundredths and display at current cursor position
  int seconds;
  int hundredths;
  seconds = timeInMillis /1000;
  seconds = seconds % 60; // seconds never more than 60
  hundredths = (timeInMillis % 1000)/10;
  
  if (seconds <10){ // print leading zero if necessary
    display.print("0");
  }
  display.print(seconds);
  display.print(":");
  if (hundredths <10){ //print leading zero if necessary
  display.print("0");
  
  }
  display.print(hundredths);
  display.print("s");
  //display.display();
}
