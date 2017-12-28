

/* Remote Control Unit
 By Jack Edwards coyotewaits3@gmail.com
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
  H6B I am not sure what I did between H4 and H6.  H6B added data transfer to include bnoCAL_status and print
  H6C modified the LCD contrast to define user input for LCD contrast PWM control to be consistent with contrast control added to the AP-J8
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
    #define Remote_Version 2 // 1 Jack's old Serial remote with TFT, 2 Remote with LCD and Arduino Micro, 3 remote with TFT and LCD
    #define TFT_Attached 0  // 0 not attached, 1 is attached
    #define LCD_Attached 1 // 0 not attached, 1 is attached 
    // NOTE for unknown reasons if LCD_Attached is = 0 you get a compile error "'keypadEvent' was not declared in this scope"
    // if you comment out the LiquidCrystal block below the code will compile  
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    #include <Keypad.h>
  
   boolean nighttime = 0;  // sets TFT print colors red at night
   boolean refresh_screen = 1;
   
  // to get the code to compile with # LCD_Attached = 0 comment out this block if getting compile erros 
 #if LCD_Attached 
   #include <LiquidCrystal.h>
   #define LCD_Contrast_Pin 13  //LCD contrast PWM control pin using 13 on Micr0
   #define LCD_Contrast 100  // 0 to 255 over 128 not recommended This replaces the 10k pot for V0 contrast control input
  // LiquidCrystal lcd(22,24,26,28,30,32);
  // LiquidCrystal lcd(36,38,40,42,44,46);
    #if Remote_Version == 3 
    LiquidCrystal lcd(46,44,42,40,38,36); //use this for RFRemote with Mega
    #endif 
    #if Remote_Version == 2
    LiquidCrystal lcd(A5,A4,A3,A2,A1,A0);  // Jacks RF remote using Arduino Micro 
    #endif
  #endif
   // Comment out this block from above to here to compile with #if LCD_Attached = 0
   
  int Screen = 0;
   
     //  KEYPAD SETUP
  const byte ROWS = 4; //four rows
  const byte COLS = 3; //four columns
  char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
  };
 // #define Keypad_Data 1
 // byte rowPins[ROWS] = {36,38,40,42}; //connect to the row pinouts of the keypad
 // byte colPins[COLS] = {44,46,48}; //connect to the column pinouts of the keypad
  
   # if Remote_Version == 3
    byte rowPins[ROWS] = {24,34,32,28}; //key defs for RF remote with LCD and TFT
    byte colPins[COLS] = {26,22,30}; //key defs for RF remote with LCD and TFT
   #endif
   #if Remote_Version == 1
    byte rowPins[ROWS] = {43,33,35,39}; //connect to the row pinouts of the keypad  JACK'S old Serial remote
    byte colPins[COLS] = {41,45,37}; //connect to the column pinouts of the keypad  JACK'S old serial remote
   #endif
   #if Remote_Version == 2
    byte rowPins[ROWS] = {7,12,11,9}; //key defs for RF remote with Micro
    byte colPins[COLS] = {8,6,10}; //key defs for RF remote with Micro
   #endif
   char key = 0;
   Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );  
 // boolean key_pressed = true; 
  
    #if TFT_Attached  
      #include <Adafruit_ILI9341.h>
      #include <Adafruit_GFX.h>
    
      #define TFT_DC 9
      #define TFT_CS 10
      Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
     // int Screen = 1; moved
      int line;
      #define line_space 30 // needs to be 24 or greater
      
       float HDG_Old;
       float HTS_Old;
       float COG_Old;
       float CTS_Old;
       float XTE_Old;
       String WPT_Old;
       String Mode_Old;
       int Steering_Mode_Old;
       int bnoCAL_status_Old;
       
       // Color definitions
      #define BLACK    0x0000
      #define BLUE     0x001F
      #define RED      0xF800
      #define GREEN    0x07E0
      #define CYAN     0x07FF
      #define MAGENTA  0xF81F
      #define YELLOW   0xFFE0 
      #define WHITE    0xFFFF
      int Color1;
      int Color2;
   #endif
     boolean KeyLight = 0;
    int CountKeyLight = 0;  

    // For printf support on RF information
    int serial_putc( char c, FILE * ) 
    {
      Serial.write( c );
      return c;
    }
    
  const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
   #if Remote_Version == 2
    RF24 radio(2,3);// 2,3 for Remote with Micro
   #else
    RF24 radio(11,10);// Use 11,10 for Remote with Mega TFT and LCD, also use for old serial remote
   #endif
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
  //  int Screen=0;
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
    float bearingrate = 0;
    boolean newData;
    int Steering_Mode;
    int bnoCAL_status;  //used to send 4 digit cal status to RF remotes
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
      
      keypad.addEventListener(keypadEvent); //add an event listener for this keypad  
        //keypad.setDebounceTime(200);
     
     #if LCD_Attached       
      pinMode (LCD_Contrast_Pin, OUTPUT); //LCD Contrast Pin
      analogWrite(LCD_Contrast_Pin, LCD_Contrast); // pwm control of LCD contrast  Replaces 10K pot for contrast control
      lcd.begin(20,4); // LCD size
     #endif
     
      SPI.begin(); 

     #if TFT_Attached  
      tft.begin();
      //tft.setRotation(2);  // this command will rotate display 180 deg
      tft.fillScreen(ILI9341_BLACK);
      Print_Screen_0();
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
         
         #if LCD_Attached
         LCD();  // use this for LCD only prints when new data available
         #endif
         
         #if TFT_Attached
         Print_Screen(); // use this for TFT
         #endif
         
         newData = false;
       }
      KEYPAD();  // extra keypad calls improves keypad response  
    }

