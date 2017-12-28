
   void recvData()
    {
   //   digitalWrite (RFslavePin, LOW); // Selects RF module as SPI slave 
   
      //lcd.setCursor(0, 0); LCD test
      //lcd.print("    test");
   
      if ( radio.available() ) 
      {     
        //Serial.println(" Radio Available ");  
      radio.read(&RFdata, sizeof(RF_DATA));
      
      if (RFdata.RFD_set == 1) Parse_RFdata1();
      if (RFdata.RFD_set == 2) Parse_RFdata2();
      newData = true;
      
     /* 
      Mode = RFdata1.Mode;
      if(Mode=="GPS")
      {
      Waypoint_next = RFdata1.Waypoint_next;  
      }     
     
      heading = RFdata.heading;
      heading_to_steer = RFdata1.heading_to_steer;
      */

      /* 
      Serial.println(RFdata.Waypoint_next);
      Serial.println(RFdata.heading);
      Serial.println(RFdata.heading_to_steer);
      Serial.println(RFdata.Mode);
      Serial.println(RFdata.MSG); 
     */
      }  // end if radio available
    } // end recvData
    
    /********************/
    
    void  Parse_RFdata1()
    {
      Waypoint_next = RFdata.RFD_text;
      heading = RFdata.RFD_float1;
      heading_to_steer = RFdata.RFD_float2;
      course = RFdata.RFD_float3;
      course_to_steer = RFdata.RFD_float4;
      Steering_Mode = int(RFdata.RFD_float5);
     // Serial.println(Waypoint_next);
    } // end void Parse_RFdata1
    
        /********************/
    
    void  Parse_RFdata2()
    {
 Mode = RFdata.RFD_text;
 XTE =  RFdata.RFD_float1;
 Bearing_origin_to_destination = RFdata.RFD_float2;
 Bearing_to_destination = RFdata.RFD_float3;
 MSG =  RFdata.RFD_float4; 
    } // end void Parse_RFdata2
    
