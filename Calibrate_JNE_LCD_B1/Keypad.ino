void KEYPAD(){ 
 char key = keypad.getKey();
   if (key != NO_KEY){
   //Serial.println(key);
   lcd.print(key);
  
     if (key == '#'){
      newHeading = 1;
      Heading = inHeading.toInt();
     // Heading = 2 * Heading;  // test if a valid integer
      // Serial.print(Heading); 
      inHeading = "";
      return;
     }
   
    inHeading = inHeading + key;
   // Serial.print(Heading);
   // Heading = inHeading.toInt();
   // Heading = 2 * Heading;
   // Serial.println (Heading);
   // lcd.print(key);
  } // end if (key != NO_KEY)

}  // end KEYPAD()
 
/********************************************/ 
/*
 void KEYPAD()
{
  char key = keypad.getKey();
  
           // if (key != NO_KEY) {   }
           //delay(100);  // added to debounce, cna be removed in a longer sketch
     //  Serial.print(" getket = "); Serial.println(key);
}

/***********************************************/

/*void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
     
    case PRESSED:
     KeyPressed(key);
    break;
  
    case RELEASED:
     KeyReleased(key);
    break;
   
    case HOLD:
     KeyHeld(key);
    break; 
    
    // }  // end switch (key)
    } // end switch(keypad.getstate)
  }  //  end void keypadEvent(KeypadEvent key)

/********************************************************/

  /*void KeyPressed(char keyin)
{  
   
    
} // End void KeyPressed

/********************************************/

/*void KeyReleased (char keyin)
{
   switch (keyin){ 
       
        case '*':  
       
        break;
          
        case '#': 
        
        break;
          
      }  // end swtich key
  
} // end key released

/*************************************************/
/*
void KeyHeld(char keyin){
 switch (keyin){
  case '#':
  break;
   }  // end switch (keyin)
}  // end KeyHeld
*/
