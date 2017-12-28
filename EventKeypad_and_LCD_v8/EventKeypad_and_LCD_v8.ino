/***********************************
By Jack Edwards
Here is a simple code that test functionality of the LCD and keypad. 
It uses the same pin assignments as the Autopilot.
It can be used to test that things are wired properly and running.  
Read the code to see what the keys do.
Does not require the complexity that adding the IMU does.

V8 prints to the serial monitor at 57600 baud.  You can test keypad operation without the LCD.
It can be used to experiment with. 

*******************************************************/

/* @file EventSerialKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates using the KeypadEvent.
|| # 
*/

/* EventKeypad and LCD is set up to use keypad to get input for Autopilot by Jack Edwards
  it does the following
  KEY TYPE  ACTION
   0 press - sets steering mode to 0 OFF
   1 hold - sets steering mode to 1 COMPASS
   2 hold - set steering mode to 2 GPS
   3 none
   4 press - decrease course b 10 deg
   5 - none
   6 press - increase course 10 deh
   7 press - decrease course by 1 deg
   8 none
   9 press - increase course by 1 deg
   * press/release - Left Rudder ON until released then rudder OFF
   0 press - sets steering mode to 0 OFF
   # press/release - Right Rudder ON until released then OFF
*/

#include <Keypad.h>

// LCD library code:
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(41,43,45,47,49,51);

// Keypad Setup
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
byte rowPins[ROWS] = {23,25,27,29,}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31,33,35}; //connect to the column pinouts of the keypad

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char input;
int mode = 0;
String str_mode = "OFF";
float heading = 15;
float heading_to_steer;

void setup(){
  Serial.begin(57600);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  lcd.begin(20,4);  // initialize LCD for 4 row 20 characters
  lcd.setCursor(0,0);
}
  
void loop(){
  // put if (sw1) here then if(!sw1) get key
  char key = keypad.getKey();
  if (key != NO_KEY) {
    //lcd.print(key);
  }
lcd.setCursor (0,3);
lcd.print(str_mode);
}

//take care of some special events
void keypadEvent(KeypadEvent key){
  
  delay(20);
  switch (keypad.getState()){
    case PRESSED:
    Serial.print("Key = "); Serial.println(key);
      switch (key){
        case '0': 
          mode = 0;
          str_mode = "OFF";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key = ");
          lcd.print(key);
          lcd.setCursor(0,1);
          lcd.print("MODE = OFF");
          break; 
          
        case '1': 
          mode = 1;
          str_mode = "COMPASS";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key = ");
          lcd.print(key);
          lcd.setCursor(0,1);
          lcd.print("mode = compass");
          heading_to_steer = heading;
          lcd.setCursor(0,2);
          lcd.print("HTS = ");
          lcd.print(heading_to_steer);
          break;
          
        case '2': 
          mode = 2;
          str_mode = "GPS";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key = ");
          lcd.print(key);
          lcd.setCursor(0,1);
          lcd.print("mode = GPS");
          break;
         
          case '3': 
          mode = 2;
          str_mode = "TACK";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key = ");
          lcd.print(key);
          lcd.setCursor(0,1);
          lcd.print("mode = TACK");
          break;

          
        case '4': 
            str_mode = "Left 10";
            heading_to_steer = heading_to_steer -10;
            if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
            if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360;
            lcd.begin(20,4);
            lcd.setCursor(0,0);
            lcd.print("Key = ");
            lcd.print(key);
            lcd.setCursor(0,1);
            lcd.print("HTS = ");
            lcd.print( heading_to_steer);

            break;

            
            case '5': 
              mode = 2;
              str_mode = "Knob Steering";
              lcd.begin(20,4);
              lcd.setCursor(0,0);
              lcd.print("Key = ");
              lcd.print(key);
              break;
 
         case '6': 
            str_mode = "Right 10";
            heading_to_steer = heading_to_steer +10;
            if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
            if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360;
            lcd.begin(20,4);
            lcd.setCursor(0,0);
            lcd.print("Key = ");
            lcd.print(key);
            lcd.setCursor(0,1);            
            lcd.print("HTS = ");
            lcd.print( heading_to_steer);
             break;
                       
           case '7': 
            str_mode = "Left 1";
            heading_to_steer = heading_to_steer -1;
            if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
            if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360;
            lcd.begin(20,4);
            lcd.setCursor(0,0);
            lcd.print("Key = ");
            lcd.print(key);
            lcd.setCursor(0,1);
            lcd.print("HTS = ");
            lcd.print( heading_to_steer);
            break;

         case '8':               
            str_mode = "Switch Screens";
            lcd.begin(20,4);
            lcd.setCursor(0,0);
            lcd.print("Key = ");
            lcd.print(key);
            break;            

        case '9': 
            str_mode = "Right 1";
            heading_to_steer = heading_to_steer +1;
            if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
            if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360;
            lcd.begin(20,4);
            lcd.setCursor(0,0);
            lcd.print("Key = ");
            lcd.print(key);
            lcd.setCursor(0,1);
            lcd.print("HTS = ");
            lcd.print( heading_to_steer);
            break;     
       } // end key pressed       
    break;
    
    case RELEASED:
    Serial.print("Key "); Serial.print(key); Serial.println(" Released");
      switch (key){
        case '*':   
          str_mode = "COMPASS";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key * Released");
          lcd.setCursor(0,1);
          lcd.print("RUDDER OFF");
          break;
          
         case '#':   
          str_mode = "Rudder Stop";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key # Released");
          lcd.setCursor(0,1);
          lcd.print("RUDDER OFF");
          break;
      } // end key released
   break;
   
    case HOLD:
    Serial.print("Key "); Serial.print(key); Serial.println(" Held");
      switch (key){
        case '*':   
          str_mode = "Dodge Left";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key = ");
          lcd.print(key);
          lcd.setCursor(0,1);
          lcd.print("LEFT RUDDER ON");
          break;
          
        case '#':   
          str_mode = "Dodge Right";
          lcd.begin(20,4);
          lcd.setCursor(0,0);
          lcd.print("Key = ");
          lcd.print(key);
          lcd.setCursor(0,1);
          lcd.print("RIGHT RUDDER ON");
          break;
          
      }  // end key HOLD
      break;
    }  // end get keypad state (Pressed, Released or Hold)
  }  // end keypad event

