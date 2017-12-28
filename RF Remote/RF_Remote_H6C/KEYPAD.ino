  
  void KEYPAD()
  {
    char key = keypad.getKey();
  }
  
  void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
      
      case PRESSED:
      //Serial.print("key sent "); Serial.println(key); // for testing
      #if TFT_Attached
       tft.setCursor(0,288); tft.fillRect(0,288,240,24,Color1); 
       tft.setTextSize(3);
       tft.setTextColor(ILI9341_BLACK);
       tft.setCursor(0,288); tft.print("   KEY  "); tft.print(key);
       KeyLight = 1; // sets up counter to turn off the key stroke display at the bottom of the TFT
      #endif
      
      // send key via RF
      #if Remote_Version == 1 // remote version 1 is old serial remote and it has knob steering Be sure remote local SW is in remote
       if(key != '8')  // key 8 not sent as it changes the Screen viewed on the main unit,
      #endif         //  
     
      #if Remote_Version != 1
      if(key != '8' && key !='5') // key 8 not sent as it changes the Screen viewed on the main unit, key 5 would start knob
      #endif                            //  steering using main unit knob with unknown position
                                 
      {
        radio.stopListening(); 
        radio.write( &key, sizeof(key) );
        radio.startListening();  
       // Serial.print("key sent "); Serial.println(key); 
      }   
       
       //if(key != '8')  Serial1.println(key);  // sends key value to AP
       if(key == '8')
         {
          refresh_screen = true; // Sets up printing static TFT screen once
          Screen = Screen +1;
            if(Screen > 1) 
            {
              Screen = 0;
              nighttime=!nighttime;
            }
        }  //end if key == 8 
         
        #if Remote_Version != 1 // Remote version 1 uses key 5 for knob steering and seerate screen for nighttime      
         if(key == '5')
         {
          nighttime = !nighttime;
          // Serial.print (" nighttime = ");Serial.println(nighttime);
          refresh_screen = true;
          break; 
         }
        #endif 
      break;
     
   
      case RELEASED:
        switch (key)
     { 
       // The Released case: is written as an extension of the pressed case.  It is easier to handle on the receive end.
          case '*': 
          key = 'A';  
          radio.stopListening(); 
          radio.write( &key, sizeof(key) );
          radio.startListening();
          break;
            
          case '#':   
          key = 'B'; 
          radio.stopListening(); 
          radio.write( &key, sizeof(key) );
          radio.startListening();
          break;
            
        }  // end swtich key
      break;
     
      case HOLD:
      switch (key){
        case '2':
         key = 'C'; 
         radio.stopListening(); 
         radio.write( &key, sizeof(key) );
         radio.startListening();
        break;
         
       break;     
       }  // end switch (key)
       
      } // end switch(keypad.getstate)
    }  //  end void keypadEvent(KeypadEvent key)
    

