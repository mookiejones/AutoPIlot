  
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
      #if TFT_Attached
       tft.setCursor(0,288); tft.fillRect(0,288,240,24,ILI9341_GREEN); 
       tft.setTextSize(3);
       tft.setTextColor(ILI9341_BLACK);
       tft.setCursor(0,288); tft.print("   KEY  "); tft.print(key);
      #endif
      
       KeyLight = 1;
     // Serial.println (key);
      //lcd.setCursor(0, 0);
      //lcd.print(key);
      
      // send key via RF
      if(key != '8') // key 8 not sent as it changes the screen viewed on the main unit
      {
        radio.stopListening(); 
        radio.write( &key, sizeof(key) );
        radio.startListening();  
        //Serial.println("key sent ");  
      }   
       
       if(key != '8')  Serial1.println(key);  // sends key value to AP
       if(key == '8')
         {
          key_pressed = true; // Sets up printing static screen once
          screen = screen +1;
            if(screen > 2) screen = 1;
         }  //end if key == 8 
         
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
       // put any commands to be executed when key held,  Pressed and Relased does the same but faster, the Hold command has a delay 
      /*
       case '5':
      */
       break;     
       }  // end switch (key)
       
      } // end switch(keypad.getstate)
    }  //  end void keypadEvent(KeypadEvent key)
    

