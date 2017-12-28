/*******************************

PID TAB is the PID calculator and it computes and sends rudder control signals

*******************************/
/******************************
PID_MODE
 MODE 0: rudder_command = PID_output with no integral_error term
 MODE 1: rudder_command = rudder_command + PID_output.  This is a form of integral control. Was used successfully summer 2012
 MODE 3: rudder_command = PID_output and includes integral error.
RUDDER_MODE 
  Rudder mode determines which mode of PID control and rudder control are used.
  MODE 0: Rudder controlled to specified rudder_position
  MODE 1: Rudder keeps moving until rudder_command < deadband


***********************************/


        void Steer_PID(){  
        // float heading_error_max = 40; // limit rudder for large heading changes, use until a rudder position indicator installed
       //  static float PID_Gain;
        // float PID_MAX = .2;
         // static float integral_error;  
         //float rudderMAX = 0;      
        // float integral_error_max = 20;
        Serial.print(Waypoint_old); Serial.print("  "); Serial.println(Waypoint_next);
       // Serial.println("PID 1");
        // Get_Steering_Mode();
          //Serial.println(Steering_Mode);
          //Serial.println(sw1_turned_on); 
          deadband = 2.0;
          
        if(!DODGE_MODE)
        { 
           // if keypad "1" was pushed Steering_Mode = 1 (compass steer) and heading_to_steer was set to the then current heading  
           MSG = 0; // null
           if (Steering_Mode == 2) GPS_Steer();      
               // adjusts gyro heading_to_steer so GPS course = GPS course_to_steer, 
           
           heading_error = heading_to_steer - heading;             
          if (abs(heading_error) > 180) // this limits error to < 180 and makes turn short way on compass + right, - left
            {
               if(heading_to_steer > heading)  heading_error = heading_error - 360;
               if(heading_to_steer < heading) heading_error = 360 + heading_error;
            }

          /*
          lcd.setCursor(0,1);
          lcd.print("BRT       ");
          lcd.setCursor(5,1);
          lcd.print(bearingrate);
          */
          //if(abs(bearingrate) < 0.5 ) bearingrate = 0; // try to cut out bearing rate noise
      
           differential_error = bearingrate;
          
          // Serial.print(heading_error);
          // Serial.print("  ");
          // Serial.println(differential_error);
          
           integral_error = integral_error + PID_Ks[3] * heading_error; // integral error used to control droop
           if (!Steering) integral_error = 0;
           /*
           When sailing to windward a non zero rudder is needed to keep head into the wind with no integral term this requires a course error signal
           this is called droop and integral term will steer correct heading with non-zero rudder angles
           */
          // integral_error = constrain(integral_error,-integral_error_max,integral_error_max);  //may need work
          
           //PID_Ks[0] =  float (analogRead(2))/100;  //uses pot on front of A/P for overall gain 
          // Serial.print("Gain "); Serial.println(PID_Ks[0]);
            
            
          if(PID_MODE == 0)  
          {
            PID_output = PID_Ks[0] * (PID_Ks[1] * heading_error  - PID_Ks[2] * differential_error); 
            rudder_command = PID_output;
          }
          
          if(PID_MODE == 1)
          {   
           PID_output = PID_Ks[0] * (PID_Ks[1] * heading_error  - PID_Ks[2] * differential_error); 
           rudder_command = rudder_command + PID_output;  // this is a form of integral control was used summer of 2012 with estimated rudder position
          } 
          
          if(PID_MODE == 3)
          {
           PID_output = PID_Ks[0] * (PID_Ks[1] * heading_error  - PID_Ks[2] * differential_error + integral_error); 
           rudder_command = PID_output;
          // lcd.setCursor(0,1);
          // lcd.print("CMD ");
          // lcd.print(rudder_command,1);// diagnostic
           
           // if(abs(heading_error) < 10) TACK_ON = false;  // used to limit tack rate probably not needed
          //  if(TACK_ON) rudder_MAX = Tack_rudder_MAX; // 
          }
          
          if (Steering_Mode == 5)
         {
           Knob_Steering();          
         }  // end if(Steering_Mode == 5)
         
          rudder_MAX = Maximum_Rudder;
          if(TACK_ON)   rudder_MAX = Tack_rudder_MAX;
          if(new_waypoint) rudder_MAX = new_waypoint_rudder_MAX;
          rudder_command=constrain(rudder_command,-rudder_MAX,rudder_MAX);
        }   // end if  not DODGE_MODE
        
        if(!Steering)
           {
             rudder_command = 0;
             heading_to_steer = 0;
           }
    
        
         Rudder_Control(); // call Rudder for actual turning rudder
         
         if(Print_PID)
         {
           Serial.print("course : "); Serial.println(course,1);
           //Serial.print("Heading Average: "); Serial.println(Cavg,1);
           Serial.print("heading to steer: "); Serial.println(heading_to_steer,1);
           Serial.print("Heading Error: "); Serial.println(heading_error,1);
         //  Serial.print("compass delta T in sec: "); Serial.println(compass_delta_T);
          // Serial.print("delta heading: "); Serial.println(delta_heading);
           Serial.print("Bearing Rate: ");Serial.println(bearingrate);
           Serial.print("Integral error: "); Serial.println(integral_error);  
           Serial.print("PID Output: "); Serial.println(PID_output);
          // Serial.print("Rudder: "); Serial.println(rudder_change);
           Serial.println("***********************"); 
         }   // end if Print PID       
       }  // End Void PID_calc()  

/**********************************************************************/
        void Rudder_Control()
        {
          int motorspeed_min = 30;
              
          RUDDER_POSITION();// update rudder position  
          if (RUDDER_MODE ==1) rudder_position = 0; // rudder feed back RFB not availabl  
          rudder_error = rudder_command - rudder_position;
          
          if(Steering_Mode == 0 || !sw1 || !sw2)  // sw1 and sw2 need to be on for automated steering
         {
            Steering = false;
            Rudder_Stop();
            Open_Solenoid();  // open solenoid to enable manual steering
         }
         
        if(Steering_Mode != 0 && sw1 && sw2) Close_Solenoid();   // this closes solenoid to enable hydraulic steering, hardware dependent
        
        if(Steering_Mode == 1 || Steering_Mode == 2 || Steering_Mode ==3 || Steering_Mode == 5) Steering = true; // could use if Steering_Mode >0
       // if(DODGE_MODE) Steering = false; //  if keypad LEFT or RIGHT RUDDER skip PID rudder control
            
        if(!DODGE_MODE) // do not steer if in dodge mode
        {
       //   if(rudder_on)  RUDDER_POSITION(); //if rudder on up date position      
          if(Steering)
          {
     
        // motorspeed = 40;
         motorspeed = 128/30*rudder_error;  // rudder error of 30 deg will result in full speed on rudder
        //  motorspeed = 128.0/40.0*(rudder_command); // + = right, - = left
         if( motorspeed > 127) motorspeed = 127;
         if ( motorspeed < -127) motorspeed = -127;
         motorspeed= abs(motorspeed);
         if (motorspeed < motorspeed_min) motorspeed = motorspeed_min;
        // TACK_ON is set true for steering mode 3 when key 4 or 6 is pushed.  this sets max rudder and rudder speed to tack values 
        // when regular rudder command is less than deadband + 1, TACK_ON is turned off to revert to regular steering
 
         if(TACK_ON) 
            {
               motorspeed = min(motorspeed, Tack_rudder_speed);
              if (abs(rudder_command) < deadband+1) TACK_ON = false;  // turns TACK_ON back off so steering speed and rudder max are regular            
            }  // end if tack on
            
             //    Rudder_Stop();  // stops the rudder breifly each cycle, eliminates complex logic to test if rudder should stop
                 
     //            if(RUDDER_MODE == 0)  // Rudder control using rudder position,
      //           {
                       if(abs(rudder_error) < deadband) 
                         {
                          Rudder_Stop();
                         }                    
                   
                      if(rudder_error > deadband)   
                         {
                           Right_Rudder();
                         }
                         
                      if(rudder_error < - deadband)
                         {
                           Left_Rudder();
                         }  
                         

                                
    //             }  // end if RUDDER_MODE == 0,
    /*             
                 if(RUDDER_MODE == 1)  // In this mode rudder keeps moving until rudder command less than deadband.
                                       // old mode did not have rudder position
                 {
                      if(rudder_command > deadband)   
                         {
                           Right_Rudder();
                         }
                         
                      if( rudder_command < (-deadband))
                         {
                           Left_Rudder();
                         }
                 }  //  end of if RUDDER_MODE == 1
   */              
            } // end  if Steering 
        }  // end if(!DODGE_MODE)          
        }  // void rudder control

      
  //------------------------  RUDDER POSITION  -----------------------

  void RUDDER_POSITION()
  {
     float rudder_position_max = 45;
     float rudder_position_min = -45;
     float counts_max = 900;  // from calibration in print statement
     float counts_at_zero = 492;
     float counts_min = 195;
     float counts;
     
     counts = analogRead(4);
     //Serial.print("Rudder = "); // use these print lines to get counts for calibration
     //Serial.println(counts);
      if(counts >= counts_at_zero) // linear calibration from zero
      {
          rudder_position = rudder_position_max *(counts - counts_at_zero) / (counts_max - counts_at_zero);
      }
      else
      {
          rudder_position = rudder_position_min * (counts - counts_at_zero) / (counts_min - counts_at_zero);
      }
      //rudder_position = - rudder_position;  // teporary reverse direction of positive rudder position
    
    // rudder_position =map(rudder_position, 187,910,-45,45); 
     
   //  Serial.print("rudder, "); Serial.println(rudder_position);
    
  }  // END VOID RUDDER POSITION
    
   /*  OLD RUDDER POSITION USING TIMING 
    float rudder_rate = .015; // deg/millisec
    int rudder_delta_time;
    int rudder_time;
    static float rudder_change; 
    
    if(rudder_on)
    {
      rudder_time = millis();
      rudder_delta_time = rudder_time - rudder_time_old;
      rudder_total_time = rudder_total_time + float(rudder_delta_time)/1000;  // diagnostic to see how much rudder motor is on can be commented out
      rudder_change = float(rudder_delta_time) * rudder_rate * rudder_direction; //time in milliseconds, change in deg 
      rudder_time_old = rudder_time;
      rudder_position = rudder_position + rudder_change;
    }
     if(abs(rudder_position) > rudder_MAX) Rudder_Stop();
   */  // END OLD RUDDER ROSITION USING TIMING  
    
   
  // ----------------------  END RUDDER POSITION --------------------
    
     
        /************************** 
         //Servo routine for rudder simulation
         rudder = 90+3 - PID_output; // 0 rudder = 90 deg position of servo so it will go 45 L and R the 3 is a trim on servo I used
          myservo.attach(38);  
          delay(50);
          myservo.write(rudder);
          delay(100);
          myservo.detach();  
         ****************************/
 
 //------------- RUDDER CONTROLS --------------------------------------------------
 void Open_Solenoid()
   {
        Serial2.write(Motor_0_fwd);  // set amotor 0 forward, 137 for Qik, 201 for Trex
        Serial2.write(0);  // speed = 0
          if(Print_Motor_Commands)
     {  Serial.print ("Motor Code, motorspeed ");
        Serial.print(Motor_0_fwd);Serial.print(", "); Serial.println(0);
     }
        
   }  // End Void Open Solenoid
  
  void Close_Solenoid()
   {
       Serial2.write(Motor_0_fwd); //  set amotor 0 forward, 137 for Qik, 201 for Trex
       Serial2.write(127); // speed = full 
         if(Print_Motor_Commands)
     {  Serial.print ("Motor Code, motorspeed ");
        Serial.print(Motor_0_fwd);Serial.print(", "); Serial.println(127);
     }
     //digitalWrite(10, LOW);   // close solenoid to enable autopilot steering steering 
   }  // end void Close-Solenoid
   
  void Rudder_Stop()
 {
      Serial2.write(Motor_1_fwd); //  for Qik 141.  set motor 1 forward for Trex(193)
      Serial2.write(0); // set speed = 0
  //  rudder_stop_time = millis();
      rudder_on = false; 
      rudder_was_off = true;    
     if(Print_Motor_Commands)
     {  Serial.print ("Motor Code, motorspeed ");
        Serial.print(Motor_1_fwd);Serial.print(", "); Serial.println(0); 
     }
 }  // end Rudder_Stop
 
 
  void Left_Rudder()
  {
  
      Serial2.write(Motor_1_rev);  //   set motor 1 in reverse, 143 for Qik, 194 for TREX
      Serial2.write(motorspeed); // set speed = motorspeed 0 to 127 is 0% to 100%
       //digitalWrite(11, HIGH);  // make sure stbd relay off
      //  digitalWrite(12, LOW);   // turn port relay on, command hardware dependent 
      //  rudder_direction = -1.0;
    if(Print_Motor_Commands)
     {  Serial.print ("Motor Code, motorspeed ");
        Serial.print(Motor_1_rev);Serial.print(", "); Serial.println(motorspeed);
     }
      rudder_on = true; //used in rudder position
      if (rudder_was_off)
          {
            rudder_time_old = millis();
            rudder_was_off = false;
          }
   // } 
  } // end Left_Rudder()
// --------------------------------------- 

  void Right_Rudder()
  {  
        Serial2.write(Motor_1_fwd); // set motor forward 141 for Qik, 193 for Trex
        Serial2.write(motorspeed);  // set speed = motorspeed 0 to 127 is 0% to 100%
        rudder_on = true; //used in rudder position
        if (rudder_was_off)
          {
            rudder_time_old = millis();
            rudder_was_off = false;
          }
        if(Print_Motor_Commands)
         {  Serial.print ("Motor Code, motorspeed ");
            Serial.print(Motor_1_fwd);Serial.print(", "); Serial.println(motorspeed);
         }  
    //}  // end if rudder < rudder MAX  
  } // end Right_Rudder
  

    /***********************************************************************/    
 
    void GPS_Steer()
    { 
      static int waypoint_error_count;
      
      if(!GPS_Available)
        {
          if(GPS_Was_Available) // GPS_Was_Available set true in void Is GPS Available in GPS ta
            {
              heading_to_steer = heading;
              GPS_Was_Available = false; // captures the heading to steer only once
            }  // end If(GPS_Was_Available
         GPS_Steering = false; // HTS is unchanged and boat will steer current compass HTS
         MSG = 2; //  NO GPS Steering HTS
        }  // end if GPS not available
     if(GPS_Available)
     {
      MSG = 0; // null message  
      GPS_Steering = true;
      if (GPRMC_fix  && Waypoint_next != "NO WPT")   //if you don't have valid fixes don't steer the boat, probably need an alarm. Needs to be more than just GPRMC 
      {      
      
       // NEW WAYPOINT   steer to waypoint bearing until steadied up after turn to prevent oversteering. Sets cross track error to zero for set time
           // remove this piece if I get anticipatory steering using $GPRTE
      if (Waypoint_old == Waypoint_next) waypoint_error_count = 0;
       if (Waypoint_old != Waypoint_next) waypoint_error_count = waypoint_error_count++;
        if (waypoint_error_count > 3) // prevents transmission errors from triggering a new waypoint condition takes 3 consecutive inequalities to trigger new waypoint
         { 
           new_waypoint = true;
           waypoint_timer = millis();
           Waypoint_old = Waypoint_next;
         }
         
       if(new_waypoint)
       {
           if ((millis() - waypoint_timer)/1000 > new_waypoint_time_delay )
            {
              new_waypoint = false;
            //  Waypoint_old = Waypoint_next; 
            }
       
           if( GPS_Auto_Steer) // if new waypoint and auto steer
            {
              XTE_course_correction = 0; 
              GPS_Steering = true;
              MSG = 4;
            } // end if autosteer

            if(!GPS_Auto_Steer)  // if newwaypoint and not auto steer
            {
              XTE_course_correction = 0; 
              if( !Dead_Reckon)
               {
                 heading_to_steer = heading;
                 Dead_Reckon = true;
               } // captures the heading only once dead reckon set false when GPS key pressed
        }  // end if(!GPS_Auto_Steer)
       }// end if new_waypoint
       
      if(Dead_Reckon) 
       {
          GPS_Steering = false; // 
          MSG = 3;
       }
       Serial.print("new Waypoint "); Serial.println(new_waypoint);
      if(GPS_Steering)   
        {
         Actual_GPS_Steering();
        if (MSG != 4) MSG = 0; // NullMsg 4 turning to new waypoint durng time delay then null
        }  // end if GPS_Steering is true
      } // end if(GPRMC_fix && ..... 
     }  // end if GPS Available
    }  // end void GPS_Steer()
 
 /*************************************************************************************/
void Actual_GPS_Steering()
{
  //CORRECTION FOR CROSS TRACK ERROR
      // XTE in feet computed in GPS_JNE_V# in feet + means right of track, steer left 
       XTE_integral_error = XTE_integral_error + Kxte[2] * XTE;
       XTE_course_correction = Kxte[0] * XTE - Kxte[1] * XTE_differential_error + XTE_integral_error; // XTE and XTE differential error from ETdata
       XTE_course_correction = constrain(XTE_course_correction,-45,45);
        course_to_steer = Bearing_to_destination - XTE_course_correction;  // if XTE is positive(right of track) need to steer left hence negative term
        course_error = course_to_steer - course; 
        heading_to_steer = heading + course_error;
            // NOTE heading_error = heading_to_steer - heading = course_error
        if (heading_to_steer > 360) heading_to_steer = heading_to_steer - 360;
        if (heading_to_steer < 0) heading_to_steer = heading_to_steer + 360;
  
}// end actual gps steering
    
 /*************************************************************************************/   
 void Knob_Steering()
 {
   float Knob;
    Knob =   analogRead(2); 
   //Serial.println(Knob);
   rudder_command =  82 * float(Knob/1000) -42 ; // rudder command +/-45 
   
   /*
   motorspeed = 255/40*(rudder_command); // + = right, - = left
   // sets motor speed 0 to 1023 for command = 0 to 40 degrees
   if( motorspeed > 255) motorspeed = 255;
   if ( motorspeed < -1023) motorspeed = -255;
   motorspeed= abs(motorspeed);
  */
  
  
   lcd.setCursor(0, 0);
  // lcd.print("           ");
  // lcd.setCursor(0, 0);
   lcd.print("CMD ");
   lcd.print(rudder_command);
   
     lcd.setCursor(5,3);
     lcd.print("Rud    "); // extra spaces clear old data
     lcd.setCursor(9,3);
     lcd.print(rudder_position,0);
   
   
 
   /*
   lcd.setCursor(0,1);
   lcd.print("speed      ");
   lcd.setCursor(7,1);
   lcd.print("     ");
   lcd.setCursor(7,1);
   lcd.print(motorspeed);
   */
   
   
 } // end void knob steering
 
 
 
 /*************************************************************************************/
 /*
     void Tracking_Error()
  {
    /*  tracking error is difference between course over ground and compass heading.
        It's purpose is to compensate for lack of accuracy in heading sensor. It also
        corrects drift or anything else that causes a deviation betweeen the direction the boat 
        is travelling and the direction the compass says it is pointing.  The results are low pass
        filtered to provide a time averaged result that will not change abruptly wwhen going to a new course
        but recompute on a new course.
     */
     

/*  
           float tracking_error_LPF = .999; // tracking error updates when GPS updates about  once per second
           if(GPRMC_fix && SOG > 1)// only updates if valid fix  may want to add a lower speed limit
           {
             tracking_error = course - heading; // would like to use an average heading but need to figure out how so the 1  and 359 doesn't avg to 180
             
                if (abs(tracking_error) > 180) // this limits err0r to < 180 and makes turn short way on compass + right, - left
                  {
                     if(course > heading)  tracking_error = tracking_error - 360;
                     if(course < heading) tracking_error = 360 + tracking_error;
                  }
             
             AVG_tracking_error = tracking_error_LPF * AVG_tracking_error + (1 - tracking_error_LPF) * tracking_error;
           } // end if
         } // end Tracking_Error
   
 */ 
/********************************************************************************/
