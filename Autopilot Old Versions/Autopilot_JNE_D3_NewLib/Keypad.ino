  /**********************************
  
  KEYPAD TAB
  
  ***************************************/
 // based on eventkeypad by Alexander Brevig 
 
/* AutoPilotKeypad V1 (same as EventKeypad_and_LCD_v6) is set up to use keypad to get input for Autopilot by Jack Edwards
  it does the following:
  
  KEY TYPE  ACTION
   0 press - sets steering mode to 0 OFF
   1 press - sets steering mode to 1 COMPASS
   2 press - set steering mode to 2 GPS
   3 press - Tack
   4 press - decrease course b 10 deg, 90 in Tack
   5  press - none,  hold - none
   6 press - increase course 10 deg, 90 in Tack
   7 press - decrease course by 1 deg
   8 LCD screen increment then back to zero
   9 press - increase course by 1 deg
   * press/release - Left Rudder ON until released then rudder OFF
   0 press - sets steering mode to 0 OFF
   # press/release - Right Rudder ON until released then OFF
*/
void KEYPAD()
{
  char key = keypad.getKey();
  
           // if (key != NO_KEY) {   }
           //delay(100);  // added to debounce, cna be removed in a longer sketch
     //  Serial.print(" getket = "); Serial.println(key);
}

//take care of some special events
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      KeyPressed(key);
    break;
  
    case RELEASED:
     KeyReleased(key);
    break;
   
    case HOLD:
    switch (key){
     // put any commands to be executed when key held,  Pressed and Relased does the same but faster, the Hold command has a delay 
    /*
     case '5':
    */
     break; 
    
     }  // end switch (key)
    } // end switch(keypad.getstate)
  }  //  end void keypadEvent(KeypadEvent key)
  
  /*************************************************************************/
  
  void Remote_Keypad()
  {
   int count_b= 0;
   const int buffer_length = 5;
   char remote_buffer[buffer_length] = "";;
   int byteRemote=-1;
   int loop_time=millis();
   boolean print_Remote = 1;
   String Remote_data;
   char KeyIn;

        // Reset the buffer
     //  for (int i=0;i<buffer_length;i++)
     //  {      
     //    remote_buffer[i]=' ';     // erase buffer   
     // } 
     
         KeyIn=Serial1.read();         // Read a byte of the serial port
          if (KeyIn >= 0)    // See if the data available  
          {

            // remote_buffer[count_b]=byteRemote;        // If there is serial port data, it is put in the buffer
            // count_b++; 
       
            if (print_Remote)
            {
               Serial.write(KeyIn);  // under Arduino 022 it used to be Serial.print(byteGPS, BYTE);
                                     // BYTE not supported in Arduino 1.0
            } // end if print_Remote     
        
      KeyPressed(KeyIn); 
     
  } //if byteGPS>=0 

 }  // end remote keypad

 
 /****************************************************/
  
void KeyPressed(char keyin)
{  
     switch (keyin){
        case '0': 
          Key0_Pressed();
        break; // end case 0
     
        case '1': 
        Key1_Pressed();
        break;
  
        case '2': 
          if(!GPS_Available)break;
          Steering_Mode = 2;
          Steering = true;
          Mode = "GPS";   
          Waypoint_old = Waypoint_next; 
          UTC_timer_old = millis();
          Date_Time();
          Time_decimal_old = Time_decimal;
          new_waypoint = false;
          GPS_Steering = true; 
          Dead_Reckon = false;
          waypoint_timer = millis();
          lcd.setCursor(0,3);
          lcd.print("          ");
          lcd.setCursor(0,3);
          lcd.print(Mode);
        break; 
          
        case '3': 
          // needs to be entered from mode 1
          if (Steering_Mode != 1) heading_to_steer = heading;
          Steering_Mode = 3;
          Steering = true;
          Mode = "TACK";      
          lcd.setCursor(0,3);
          lcd.print("          ");
          lcd.setCursor(0,3);
          lcd.print(Mode);
        break; 
         
        case '*':   //Rudder on Dodge Left
          if(Steering_Mode == 0 || Steering_Mode ==5) break;
          {  
            DODGE_MODE = true;
            Previous_Mode = Mode;
            Mode = "DGE L";
           motorspeed = 127;
            Left_Rudder();
          }
         break;
          
        case '#': //  rudder on Dodge right
         if(Steering_Mode == 0 || Steering_Mode ==5) break;
          {   
            DODGE_MODE = true;
            Previous_Mode = Mode;
            Mode = "DGE R";
           motorspeed = 127;
            Right_Rudder();
          }
        break;
          
        case '4': 
             if (Steering_Mode==1)  heading_to_steer = heading_to_steer -10;
             if(Steering_Mode ==3)
            { 
              heading_to_steer = heading_to_steer - Tack_Angle;
              TACK_ON = true;
            }
                if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
                if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360; 
                lcd.setCursor(15, 2);
                lcd.print(heading_to_steer,1);
             if(Steering_Mode != 1 && Steering_Mode != 3)
             {
                lcd.begin(20,4);
                lcd.setCursor(6,3);
                lcd.print("WRONG MODE");  // this would be a good place to put a audible alarm beep
               // delay(250);
             }
         break;  // Break for case PRESSED
         
              
         case '5': 
           
          Steering_Mode = 5;
          Steering = true;
          Mode = "KNOB";     
          lcd.setCursor(0,3);
          lcd.print("          ");
          lcd.setCursor(0,3);
          lcd.print(Mode);
         break;  // case5
         
        case '6': 
             if(Steering_Mode==1)  heading_to_steer = heading_to_steer + 10;
             if(Steering_Mode ==3)
            { 
              heading_to_steer = heading_to_steer + Tack_Angle;
              TACK_ON = true;
            }
                if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
                if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360; 
                lcd.setCursor(15, 2);
                lcd.print(heading_to_steer,1);
             if(Steering_Mode != 1 && Steering_Mode != 3)
             {
                lcd.begin(20,4);
                lcd.setCursor(6,3);
                lcd.print("WRONG MODE");  // this would be a good place to put a audible alarm beep
                delay(250);
             }
          break;
            
            
          case '7': 
             if (Steering_Mode==1 || Steering_Mode ==3)
             {
                heading_to_steer = heading_to_steer -1;
                if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
                if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360;
                lcd.setCursor(15, 2);
                lcd.print(heading_to_steer,1);
                 break;
             }
               lcd.begin(20,4);
                lcd.setCursor(6,3);
                lcd.print("WRONG MODE");
                delay(250);
          break;
           
         case '8': 
              Screen = Screen +1;
             if( Screen > 3) Screen = 0;
         break;
            
        case '9': 
             if (Steering_Mode == 1 || Steering_Mode == 3)
             {
                heading_to_steer = heading_to_steer +1;
                if (heading_to_steer < 0) heading_to_steer = heading_to_steer +360;
                if (heading_to_steer > 360) heading_to_steer = heading_to_steer -360;
                lcd.setCursor(15, 2);
                lcd.print(heading_to_steer,1);
               break;
             }
               lcd.begin(20,4);
                lcd.setCursor(6,3);
                lcd.print("WRONG MODE");
                delay(250);
            break;
        // these next two cases come from the remote keypad where A or B sent when * or # released
        case'A':
         Star_Released(key);
        break;    
        
        case'B':
         Pound_Released(key);
        break;    
            
            
      }   // end swtich key
} // End void KeyPressed

/************************************************************************/
void KeyReleased (char keyin)
{
   switch (keyin){ 
       
        case '*':  
         Star_Released(key);
        break;
          
        case '#': 
         Pound_Released(key);
        break;
          
      }  // end swtich key
  
} // end key released

/***********************************************/

        void Key0_Pressed()
        {
          Open_Solenoid();
          Steering_Mode = 0;
          Mode = "OFF";
          Steering = false;
          Accept_Terms = 0;  // this quits printing the Terms and conditions on start up
          Screen = 0;  
          lcd.setCursor(0,3);
          lcd.print("          ");
          lcd.setCursor(0,3);
          lcd.print(Mode);  
        }  // end Key0 pressed

void Key1_Pressed()
{
          Steering_Mode = 1;
          Steering = true;
          Mode = "COMP";
          heading_to_steer = heading;
          integral_error = 0; // reset integral error
          lcd.setCursor(0,3);
          lcd.print("          ");
          lcd.setCursor(0,3);
          lcd.print(Mode);
          lcd.setCursor(15, 2);
          lcd.print(heading_to_steer,1);
}  // end key1 pressed
        
void Star_Released(char keyin)
  {
          if(Steering_Mode == 0 || Steering_Mode ==5) return; 
          DODGE_MODE = false;
          Rudder_Stop();
          Mode = Previous_Mode;
  
   }  // end Star_Released
   
/*****************************************************/
void Pound_Released(char keyin)
  {
          if(Steering_Mode == 0 || Steering_Mode ==5) return; 
          DODGE_MODE = false;
          Rudder_Stop();
          Mode = Previous_Mode;
  
   }  // end Star_Released
