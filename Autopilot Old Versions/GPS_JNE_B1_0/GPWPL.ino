
 void Get_GPWPL()
 {
   //Serial.println("GPBOD");
    j_MAX = 6;  // number of Words in NEMA Sentence
    Parse_Sentence();
    for(int j = 0; j<j_MAX; j++)
     {
      data_WPL[j] = data_IN[j];  //see void Parse Sentence
     } 
   //    *************  PROCESS DATA  **********************  
 
        // data_BOD[0], header, Processed in Get_Sentence()
        // data_BOD[1],Waypoint Lattitude
         
     To_Degrees(data_WPL[1]);
     Lat_Waypoint = Lat_Lon_Deg; // value returned from function
     if(data_WPL[2] == "S") Lat_Waypoint = - Lat_Waypoint;
     // data_WPL[2] = Lat N/S, no processing
      
     To_Degrees(data_WPL[3]);
     Lon_Waypoint = Lat_Lon_Deg; // value returned from function
     if(data_WPL[4] == "W") Lon_Waypoint = - Lon_Waypoint;
    // data_WPL[4] = Lon E/W, no processing
    
    // data_WPL[5] = Waypoint name
       Waypoint_name = data_WPL[5];  
      
      //  STORE DATA IN ARRAY UISING ROUTE INDEX
     for (int i = 0; i< Number_of_waypoints; i++) 
     {
       if (data_WPL[5] == Route[i])
       {
         Waypoint_Lat[i] = Lat_Waypoint;
         Waypoint_N_S[i] = data_WPL[2];
         Waypoint_Lon[i] = Lon_Waypoint;
         Waypoint_E_W[i] = data_WPL[4];
        
        int1 = i;
        
        
        break;
       }
     } // end for 
        
      if(print_WPL) {PRINT_WPL();}
                      
   
   
 }  // END GET GPWPL
 
 /***************************************************************************/
 
   void PRINT_WPL()
 {  
    Serial.println();
    Serial.println("---------------");   
   
            Serial.print("Waypoint: ");  
                 Serial.println(Waypoint_name);
              
            Serial.print("WPL Lat ");    
                 Serial.print(Lat_Waypoint,4);
                 
            // Serial.print("WPL Lat  N/S ");    
                // Serial.println(data_WPL[2]);  
              if (data_WPL[2] == "N") Serial.println ( "   NORTH");   
                 
            Serial.print("WPL Lon ");    
                 Serial.print(Lon_Waypoint,4);
                 
           // Serial.print("WPL Lon  E/W ");    
               //  Serial.println(data_WPL[4]);  
              if (data_WPL[4] == "W") Serial.println ( "  WEST"); 
        Serial.print( Waypoint_Lat[int1]);  Serial.print(" "); Serial.println( Waypoint_N_S[int1]);
        Serial.print( Waypoint_Lon[int1]);  Serial.print(" "); Serial.println( Waypoint_E_W[int1]);     
   }
