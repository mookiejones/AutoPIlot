

/* Remote Control Unit
 By Jack Edwards
   Uses 2 way RF communiction.  Receives data for display on an
   LCD from main unit. Transmits KeyPad input to Main unit

   RF_Remote_1 is working model remote that receives data via RF and send keypad 
   input to the AP. received data is displayed on an LCD (8 x 2 at this time)
   looking for a better display.
  RF_Remote_2 is committed model
  V3 is attempt to add two SPI slaves.  one for RF and one for TFT so I can use TFT screen with this
  V3b rewired added 4 x 20 LCD, added LCD tab and reworked LCD printing.
  V3c change RF data structure for multiple packets
  H1 saved V3 as H1 to have same version name as autopilot and GPS these were final versions that worked great with GPS steering
  H2 and H2 _RF_TFT added TFT to the H1 skectch H2_RF works with or without the LCD attached should comment out the LCD pieces if
  LCD not attached.  or comment out pieces if TFT not used
  H4, Renamed H2_RF_TFT to H4 to coincide with current numbering on AP and GPS
  */
    /* 
    Sending formatted data packet with nRF24L01. 
    Maximum size of data struct is 32 bytes.
     contributted by iforce2d
    
    The wire numbers listed are using a ribbon wire and a two row ribbon wire connector
  1 - GND, wire 2
  2 - VCC 3.3V !!! NOT 5V, wire 1
  3 - CE to Arduino pin 9, wire 4
  4 - CSN to Arduino pin 10, wire 3 
  5 - SCK to Arduino pin 13 for Uno, 52 on Mega, wire 6
  6 - MOSI to Arduino pin 11 for Uno, 51  on Mega, wire 5
  7 - MISO to Arduino pin 12 for Uno,  50 on Mega, wire 8
  8 - UNUSED, wire 7
  */
    
    #define TFT_Attached 0  // 0 not attached, 1 is attached
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    #include <LiquidCrystal.h>
    #include <Keypad.h>
    

  
   //  LiquidCrystal lcd(22,24,26,28,30,32);
  // LiquidCrystal lcd(36,38,40,42,44,46);
    LiquidCrystal lcd(46,44,42,40,38,36);
   int screen = 1;
   
     //  KEYPAD SETUP
  const byte ROWS = 4; //four rows
  const byte COLS = 3; //four columns
  char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
  };
 // byte rowPins[ROWS] = {36,38,40,42}; //connect to the row pinouts of the keypad
 // byte colPins[COLS] = {44,46,48}; //connect to the column pinouts of the keypad
   byte rowPins[ROWS] = {24,34,32,28}; //key defs for RF remote with LCD
    byte colPins[COLS] = {26,22,30}; //key defs for RF remote with LCD
 //  byte rowPins[ROWS] = {43,33,35,39}; //connect to the row pinouts of the keypad  JACK'S old Serial remote
 //  byte colPins[COLS] = {41,45,37}; //connect to the column pinouts of the keypad  JACK'S old serial remote
  char key = 0;
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );  
  boolean key_pressed = true; 
  
    #if TFT_Attached == 1
      #include <Adafruit_ILI9341.h>
      #include <Adafruit_GFX.h>
    
      #define TFT_DC 9
      #define TFT_CS 10
      Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
     // int screen = 1; moved
      int line;
   #endif
boolean KeyLight = 0;
int CountKeyLight = 0;  
    // For printf support
    int serial_putc( char c, FILE * ) 
    {
      Serial.write( c );
      return c;
    }
    
  const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
    RF24 radio(11, 10);
  
  /*
     struct RF_DATA
     {
      char Waypoint_next[11]; 
      float heading;
      float heading_to_steer;
      char Mode[5];
      int MSG;  
     }; // end RF_DATA structure   
    RF_DATA RFdata; //NAME THE DATA STRUCTURE
    */
    
       struct RF_DATA
     {
      int RFD_set;  // read this value to determine data set
      char RFD_text[8];
      float RFD_float1;
      float RFD_float2;
      float RFD_float3;
      float RFD_float4;
      float RFD_float5;
      
      //float heading;
      //float heading_to_steer;;
      //char Mode[5];
      //int MSG;  
     }; // end RF_DATA1 structure    
    RF_DATA RFdata; //NAME THE DATA STRUCTURE
   /* 
    struct RF_DATA2
     {
      float SOG;     
      float XTE;
      float Bearing_origin_to_destination;
      float Bearing_to_destination;
      float Range_Destination;
      float course_to_steer;
      //float MagVar;
      //float bearingrate;
      //float rudder_position;
      //long UTC;
      // float Next_Turn;
     }; // end RF_DATA structure 
    RF_DATA2 RFdata2; //NAME THE DATA STRUCTURE
    */
    String Mode;
    String Waypoint_next;
    //const int RFslavePin = 53;  // for RF SPI Transaction
   // const int TFTslavePin = 25;  // for TFT SPI transaction
    int Screen=0;
    float heading;
    float heading_to_steer;
    float XTE;
    float RUDDER_MODE = 1;
    float rudder_position;
    int MSG = 0;
    float Bearing_to_destination;
    float course;
    float course_to_steer;
    float XTE_differential_error;
    float XTE_integral_error;
    float Bearing_origin_to_destination;
    float Avg_course;
    float XTE_course_correction;
    float rudder_command;
    boolean newData;
    int Steering_Mode;
    /**************************************************/
    
    void setup()
    {
      Serial.begin(57600);
      fdevopen( &serial_putc, 0 ); // for printf
      radio.begin();
      radio.printDetails();
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
      radio.startListening();
      
      lcd.begin(20,4); // LCD size
      pinMode (7,OUTPUT); //LCD backlight conected to pin 7
      analogWrite(7,24); // pwm controll backlight do not use more than 128 (out 0f 255) without a series resitor

      SPI.begin(); 


      keypad.addEventListener(keypadEvent); //add an event listener for this keypad  
        //keypad.setDebounceTime(200);
      #if TFT_Attached  
      tft.begin();
      //tft.setRotation(2);  // this command will rotate display 180 deg
      tft.fillScreen(ILI9341_BLACK);
      Print_Screen_1();
      Print_Screen();
      #endif 

        
      int dataSize = sizeof(RF_DATA);
      printf("Size of RF_DATA: %d\n", dataSize);
      if ( dataSize > 32 )
      printf("*** RF_DATA struct is too large ***\n");
    
   //   printf("This Arduino is set to be a %s\n", sender ? "sender" : "receiver");
    } // end setup
    
    
    /**************************************************/

    
    void loop()
    {
      recvData();
      KEYPAD();
      if (newData)
       {
         LCD();  // use this for LCDonly prints when new data available
         //Print_Screen(); // use this for TFT
         newData = false;
       }
      KEYPAD();  // extra keypad calls improves keypad response  
    }

