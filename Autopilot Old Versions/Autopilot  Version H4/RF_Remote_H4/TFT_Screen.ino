/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#if TFT_Attached
void Print_Screen()
{
  if(key_pressed)
    {
      tft.fillScreen(ILI9341_BLACK);
      if(screen == 1) Print_Screen_1();
      if(screen == 2) Print_Screen_2();
      if(screen == 3) Print_Screen_3();
      key_pressed = false;  //  prints static screen once
    }
    
  if(screen == 1) Print_TFT_1();
  if(screen == 3) Print_TFT_3();
}

/*************************************************************/
 void Print_Screen_1()  //Print the static screen elements once
{
   line =1; // this structure lets you rearrange lines without dealing with counting
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_GREEN);

    line++; tft.setCursor(0,line * 24); tft.print("HTS: ");
    line++; tft.setCursor(0,line * 24); tft.print("HDG: ");
   KEYPAD();
    line++; tft.setCursor(0,line * 24); tft.print("COG: ");
    line++; tft.setCursor(0,line * 24); tft.print("SOG: ");  
   KEYPAD();

    line++; tft.setCursor(0,line * 24); tft.print("RNG: ");
    line++; tft.setCursor(0,line * 24); tft.print("BRG: "); 
   KEYPAD();
    line++; tft.setCursor(0,line * 24); tft.print("TURN ");
   // line++; tft.setCursor(0,line * 24); tft.print("UTC: ");
    line++; tft.setCursor(0,line * 24); tft.print("RUD: ");
    //tft.fillRect(0, 270,240,48, ILI9341_GREEN);tft.fillRect(05, 275,230,38, ILI9341_BLACK); // draws rudder graphic at bottom
}  
/*******************************************************************/
void Print_TFT_1()
{ 
 tft.setTextColor(ILI9341_WHITE); 
  line = -1;
   line++; tft.setCursor(0, line * 24); tft.fillRect(0, line * 24,108,24,ILI9341_BLACK); tft.print(Mode); tft.print(" ");tft.print(Steering_Mode);// the fill rectblanks out old data
   line++; tft.setCursor(0, line * 24); tft.fillRect(0, line * 24,240,24,ILI9341_BLACK); tft.print(Waypoint_next);
   KEYPAD();
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(heading_to_steer,0); 
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(heading,0);
  KEYPAD();
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(course,0);
  // line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(SOG,1);
   KEYPAD();
  // line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(Range_Destination,2);
 // line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(Bearing_origin_to_destination,0);
  // KEYPAD();
 //  line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(Next_Turn,0);
  // line++; tft.setCursor(96,line * 24); tft.fillRect(70,line * 24,140,24,ILI9341_BLACK); tft.print(UTC);
   line++; tft.setCursor(96,line * 24); tft.fillRect(70,line * 24,167,24,ILI9341_BLACK); tft.print(rudder_position,1);
 // KEYPAD();
  
  if(KeyLight) 
  {
    CountKeyLight ++;
    if(CountKeyLight >3)
    {
      KeyLight = 0;
      CountKeyLight = 0;
     tft.setCursor(0,288); tft.fillRect(0,288,240,24,ILI9341_BLACK);
    }
  }
  
  // line= line+2; tft.setCursor(0,line * 24); tft.fillRect(0,line * 24,240,48,ILI9341_BLACK); tft.print(Message);
 }  // end Print TFT
  
/*******************************************************************/
 void Print_Screen_2()  //Print the static screen elements once
{
    tft.setCursor(75,0); tft.setTextSize(3); tft.setTextColor(ILI9341_RED);tft.println("HELP");
    tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
    tft.println("1 Compass");
    tft.println("2 GPS");
    KEYPAD();
    tft.println("3 Tack");
    tft.println("4 L 10/tack");
    KEYPAD();
    tft.println("5 Knob Steer");
    tft.println("6 R 10/tack");
    KEYPAD();
    tft.println("7 L 1 deg");
    tft.println("8 Screen Up");
    KEYPAD();
    tft.println("9 R 1 deg");
    tft.println("* DODGE L");
    KEYPAD();
    tft.println("# DODGE R");
    tft.setTextColor(ILI9341_YELLOW);tft.println("0 to cancel"); 
}  
/*******************************************************************/
 void Print_Screen_3()  //Print the static screen elements once
{
   line =1; // this structure lets you rearrange lines without dealing with counting
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_GREEN);
    line++; tft.setCursor(0,line * 24); tft.print("BOD: "); 
    line++; tft.setCursor(0,line * 24); tft.print("BRG: ");
    KEYPAD();
    line++; tft.setCursor(0,line * 24); tft.print("COG: ");
    line++; tft.setCursor(0,line * 24); tft.print("CTS: ");
    KEYPAD();
    line++; tft.setCursor(0,line * 24); tft.print("HDG: ");
    line++; tft.setCursor(0,line * 24); tft.print("HTS: ");
    KEYPAD();
    line++; tft.setCursor(0,line * 24); tft.print("XTE: ");
    line++; tft.setCursor(0,line * 24); tft.print("RUD: ");
    //tft.fillRect(0, 270,240,48, ILI9341_GREEN);tft.fillRect(05, 275,230,38, ILI9341_BLACK); // draws rudder graphic at bottom
}  

/*******************************************************************/
void Print_TFT_3()
{ 
 tft.setTextColor(ILI9341_WHITE); 
  line = -1;
   line++; tft.setCursor(0, line * 24); tft.fillRect(0, line * 24,108,24,ILI9341_BLACK); tft.print(Mode); // the fill rectblanks out old data
   line++; tft.setCursor(0, line * 24); tft.fillRect(0, line * 24,240,24,ILI9341_BLACK); tft.print(Waypoint_next);
   KEYPAD();
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(Bearing_origin_to_destination,2);
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(Bearing_to_destination,2);
   KEYPAD();
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(course,1);
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(course_to_steer,1);
   KEYPAD();
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(heading,1);
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(heading_to_steer,2);
   KEYPAD();
   line++; tft.setCursor(96,line * 24); tft.fillRect(72,line * 24,140,24,ILI9341_BLACK); tft.print(XTE,1);
   line++; tft.setCursor(96,line * 24); tft.fillRect(70,line * 24,167,24,ILI9341_BLACK); tft.print(rudder_position,1);
 //  line= line+2; tft.setCursor(0,line * 24); tft.fillRect(0,line * 24,240,48,ILI9341_BLACK); tft.print(Message);
 }  // end Print TFT
  
/**************************************************************/  
//Fred's Screen
/*
 void Print_Screen_2()  //Print the static screen elements once
{
   line =1; // this structure lets you rearrange lines without dealing with counting
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_GREEN);
    line++; tft.setCursor(0,line * 24); tft.print("BOD: "); 
    line++; tft.setCursor(0,line * 24); tft.print("HDG: ");
    line++; tft.setCursor(0,line * 24); tft.print("HTS: ");
    line++; tft.setCursor(0,line * 24); tft.print("COG: ");
    line++; tft.setCursor(0,line * 24); tft.print("SOG:"); 
    line++; tft.setCursor(0,line * 24); tft.print("TURN: ");
    line++; tft.setCursor(0,line * 24); tft.print("UTC: ");
    line++; tft.setCursor(0,line * 24); tft.print("RUD: ");
    
    tft.setCursor(0,250); tft.setTextSize(1); tft.setTextColor(ILI9341_RED);
    tft.println("Push 1 for Compass Mode");
    tft.println("Push 2 for GPS Mode");
    tft.println("Push 5 for Knob Steering Mode");
    tft.println("Push Zero to cancel above commands");
    //tft.fillRect(0, 270,240,48, ILI9341_GREEN);tft.fillRect(05, 275,230,38, ILI9341_BLACK); // draws rudder graphic at bottom    
}  
*/  
/***********************************************************/
/*
void Print_TFT_2()
{
  tft.setTextSize(3);
   tft.setCursor(0, 0);  tft.fillRect(0,0,108,24,ILI9341_BLACK); tft.setTextColor(ILI9341_WHITE);tft.println(Mode); // the fill rectblanks out old data
   tft.setCursor(0, 24); tft.fillRect(0,24,240,24,ILI9341_BLACK);tft.println(Waypoint_next);
   tft.setCursor(96,48); tft.fillRect(72,48,140,24,ILI9341_BLACK); tft.println(Bearing_origin_to_destination,1);
   tft.setCursor(96,72); tft.fillRect(72,72,140,24,ILI9341_BLACK); tft.println(heading,1);
   tft.setCursor(96,96); tft.fillRect(72,96,140,24,ILI9341_BLACK); tft.println(heading_to_steer,1);
   tft.setCursor(96,120); tft.fillRect(72,120,140,24,ILI9341_BLACK); tft.println(course,1);
   tft.setCursor(96,144);tft.fillRect(72,144,140,24,ILI9341_BLACK); tft.println(SOG,1);
   tft.setCursor(96,168); tft.fillRect(72,168,140,24,ILI9341_BLACK); tft.println(Next_Turn);
   tft.setCursor(96,192); tft.fillRect(70,192,140,24,ILI9341_BLACK); tft.println(UTC);
   tft.setCursor(96,216); tft.fillRect(70,216,140,24,ILI9341_BLACK); tft.println(rudder_position);
   line= 11; tft.setCursor(0,line * 24); tft.fillRect(0,line * 24,0,48,ILI9341_BLACK); tft.print(Message);
   
} // end Print TFT
*/
#endif
