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
}

//take care of some special events
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    
    case PRESSED:
      if(key != '8')   Serial1.println(key);  // sends key value to AP
      if(key == '8')
       {
        key_pressed = true; // Sets up printing static screen once
        screen = screen +1;
          if(screen > 3) screen = 1;
       }  //end if key == 8 
    break;
   
  
    case RELEASED:
      switch (key)
   { 
     // The Released case: is written as an extension of the pressed case.  It is easier to handle on the receive end.
        case '*':   
          Serial1.println("A");
        break;
          
        case '#':   
          Serial1.println("B");
        break;
          
      }  // end swtich key
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
  

