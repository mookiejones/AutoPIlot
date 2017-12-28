
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
       //Serial.println ("newData");
      
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
      //Serial.println (RFdata.RFD_float2);
      //Serial.println(RFdata.RFD_text);
    } // end void Parse_RFdata1
    
        /********************/
    
    void  Parse_RFdata2()
    {
 Mode = RFdata.RFD_text;
 XTE =  RFdata.RFD_float1;
 Bearing_origin_to_destination = RFdata.RFD_float2;
 Bearing_to_destination = RFdata.RFD_float3;
 MSG =  RFdata.RFD_float4; 
 bnoCAL_status =  RFdata.RFD_float5;
 
    } // end void Parse_RFdata2
    
