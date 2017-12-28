/* code for Jeffery to operate a set of solidstate relays
 Substitute these subroutines for the ones in the AP code in the PID Tab
 */
 
 void Open_Solenoid()
   {
     Rudder_Stop();  
   }
        
//   }  // End Void Open Solenoid
  
  void Close_Solenoid()
   {
     return;  
   }
     //digitalWrite(10, LOW);   // close solenoid to enable autopilot steering steering 
 //  }  // end void Close-Solenoid
   
  void Rudder_Stop()
 {
   digitalWrite(46, LOW);
   digitalWrite(48, LOW);
     
 }  // end Rudder_Stop
 
 
  void Left_Rudder()
  {
  
   digitalWrite( 46, HIGH);
   
      rudder_on = true; //used in rudder position
      if (rudder_was_off)
          {
            rudder_time_old = millis();
            rudder_was_off = false;
          }
  } // end Left_Rudder()
// --------------------------------------- 

  void Right_Rudder()
  {  
      digitalWrite(48, HIGH);
      
        rudder_on = true; //used in rudder position
        if (rudder_was_off)
          {
            rudder_time_old = millis();
            rudder_was_off = false;
          } 
  } // end Right_Rudder
  

