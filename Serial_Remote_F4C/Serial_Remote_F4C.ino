



//  REMOTE CONTROLLER
// NOTE CAUTION THE AP CANNOT BE SENDING DATA TO RX (UNO) WHEN UPLOADING SOFTWARE THEY INTERFERE
// OK IF THE RECIEVE DATA IS ON A DIFFERENT RX 1,2, OR 3 (MEGA)
//  Serial Remote B1_0 Killer Serial Remote Works with AP B1 and later
// V B1_1 changed Serial1 baud rate to 28800
// V B1_3 incorporating TFT screen
// V C1 Use with C series of GPS and Autopilot. got the serial remote TFT working.  Changed how I get Easy Transfer data
      // use if(ET.receiveData()) before trying to read data.
// V C2  Added more data. new version so I didn't lose previous version
// V C3 C2 uploaded to dopbox, C3 general refinement
// C4  added multiple screen structure
// D Added int MSG to data uploaded. 
//  E same as D4
// F added screen for gps autopilot debug
// F2 changed serial1 baud 57600 to 19200
// F4 went from 2 to 4 skipped over 2B and 3.  Added line in setupt to send mode 0 at startup allows reboot from serial w/o putting AP in a random mode
// F4B  Added extra KEYPAD() calls in TFTT print to get faster key response
// F4C  More KEYPAD() calls in all TFT print routines
#include <EasyTransfer.h>
#include <Keypad.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"


// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);



boolean Print_ET2data = 0;// 1 = print, 0 no print
boolean Print_ETtime = 0;  // prints Easy Transfer loop time 1 on 0 off

//  KEYPAD SETUP
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
//byte rowPins[ROWS] = {23,25,27,29,}; //connect to the row pinouts of the keypad  STANDARD
//byte colPins[COLS] = {31,33,35}; //connect to the column pinouts of the keypad  STANDARD
byte rowPins[ROWS] = {43,33,35,39}; //connect to the row pinouts of the keypad  JACK'S waterproof KEYPAD
byte colPins[COLS] = {41,45,37}; //connect to the column pinouts of the keypad  JACK'S waterproof KEYPAD
//byte rowPins[ROWS] = {45,43,41,39}; //connect to the row pinouts of the keypad  membrane KEYPAD
//byte colPins[COLS] = {37,35,33}; //connect to the column pinouts of the keypad  membrane KEYPAD
char key = 0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/****************  EASY TRANSFER 2  START****************************************/

// Set up for Easy Transfer to receive GPS data
 
 //create object
  EasyTransfer ET2; 
/**********************************************************************************/  
 
boolean NEMA_sentence = false;
boolean GPRMC_fix = false;
boolean GPAPB_fix = false;
String GPS_status= "NO GPS";
float course;
float course_to_steer; 
String CTS_MorT = "";
float SOG; 
float course_error;
//float tracking_error;
float AVG_tracking_error;
float MagVar; // Magnetic Variation East is plus and West is Minus
float heading_to_steer=0; //  see PID
float XTE;
float XTE_differential_error;
float XTE_integral_error;
float XTE_course_correction;
String XTE_LR="";
String XTE_unit="";
String Waypoint_next="";
float heading;
String Origin_Waypoint;
float Bearing_origin_to_destination=0;
String BOD_MorT = "";
float Bearing_to_destination;
float Range_Destination;
long UTC;
long Date;
float Next_Turn;
float bearingrate;
float rudder_position;
String Mode;
int screen = 1;
boolean key_pressed;
int MSG;  // Message number set in the AP and displaed on TFT. used integer to minimize  ez transfer data.
String Message;
int line;  
/**********************************************************************************/ 
  struct RECEIVE_DATA_STRUCTURE{
    //put your variable definitions here for the data you want to receive
    //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float course;
  float SOG;
  float SD_MagVar;
  float XTE;
  char Waypoint_next[11]; 
  float Bearing_origin_to_destination;
  float Bearing_to_destination;
  float Range_Destination;
 // long UTC;
  float Next_Turn;
  float heading;
  float heading_to_steer;
  float course_to_steer;
  float bearingrate;
  float rudder_position;
  char Mode[5];
  int MSG;
  
  };  //  end receive data structure
  
  //give a name to the group of data
  RECEIVE_DATA_STRUCTURE ET2data;
  

/****************  EASY TRANSFER 2  END******************************************/

void setup() 
{
  Serial.begin(57600);  // Serial connection to Serial Monitor
  Serial1.begin(19200); // Serial Key pad to external MEGA 
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad  
   //keypad.setDebounceTime(200);
  ET2.begin(details(ET2data), &Serial1);  // RECEIVE ONLY
  tft.begin();
  //tft.setRotation(2);  // this command will rotate display 180 deg
  tft.fillScreen(ILI9341_BLACK);
  Print_Screen_1();
  Print_Screen();
  key_pressed = true;
  Mode = "OFF";
  Serial1.println(0); // sends mode 0 to AP

}

void loop()
{
   KEYPAD();
   if(ET2.receiveData()) 
   {
    Recieve_ET2Data();
   }
   
}
