
/*  GPS TAB INCLUDES
    void GET_sentence() it reads the NEMA sentence.
    void Checksum_calc() computes a check sum on NEMA sentence to verify data integrity. 
    void Get_GPRMC() parses the NEMA sentence and looks for the GPRMC sentence.
    void Get_GPAPB() parses the NEMA sentence and looks for the GPRMC sentence.
    Get_sentence is called from Get_GPRMC and Get_GPAPB so program can alternate between the two GPS sentences.
    GPS Reading based on code by Igor Gonzalez Martin. 05-04-2007 igor.gonzalez.martin@gmail.com
          English translation by djmatic 19-05-2007
    Modified for this Application by Jack Edwards
 */

  void GET_sentence() 
  {   // read one NEMA sentence
        // Reset the buffer
       for (int i=0;i<buffer_length;i++){      
         gps_buffer[i]=' ';        
         } 
     checksum_status= false;   
     NEMA_sentence=false;
     long loop_time=millis();
     word_count = 1; // word count counts the commas add one for the star
     count_b = 0;
     do{
         byteGPS=Serial2.read();         // Read a byte of the serial port
          if (byteGPS >= 0)    // See if the data available  
          {
             gps_buffer[count_b]=byteGPS;        // If there is serial port data, it is put in the buffer
             count_b++; 
             if(byteGPS == ',') word_count = word_count+1;
       
            if (print_NEMA)
            {
               Serial.write(byteGPS);  // under Arduino 022 it used to be Serial.print(byteGPS, BYTE);
                                     // BYTE not supported in Arduino 1.0
            } // end if print_NEMA     
                
         } //if byteGPS>=0
         
  /* 9/21/15 comment out timer to improve reliability
         if (millis() - loop_time >2000)  // kick out of do loop if incomming data is lost
               {  
                   NEMA_sentence=false;
                   GPAPB_fix = false;
                   GPRMC_fix = false;
                 Serial.println("Timed Out ");
                   return;                 
               } // end if(millis... 
 */              
               
     } while (byteGPS !=13); // reads data until a complete sentence available
    
    
        GPSheader = "";
       bien=0;
       for (int i=2;i<7;i++){     // Verifies if the received sentence starts with $GPRMC and checksum is valid
        // if (gps_buffer[i]==gps_RMC[i-1]){   // char gps_RMC[7] = "$GPRMC";
        //  bien++;}
        GPSheader = GPSheader + gps_buffer[i];
          } // end for i= 1,7
    // Serial.println(GPSheader);     
      
       if (GPSheader == "GPAPB")
       {  
           Get_GPAPB();
         // Serial.println("GPAPB");
          return;  
       } 
       
         if (GPSheader == "GPRMC")
        {  
            Get_GPRMC();
         // Serial.println("GPRMC");
            return;   
        }  
       
         if (GPSheader == "GPRMB")
        {  
         // Serial.println("GPRMB");
            Get_GPRMB();
            return; 
        }  

//   version G3_V2comment out BOD, WPL, RTE to speed other seentences for reliability 9/21/15 
// version H2 changed to using "Use_CTS" to skip these statements if using CTS from $GPAPB like my garmin gpsmap 740s       
       
     if(Use_CTS == 0)
   {  
        if (GPSheader == "GPBOD")
        {  
         // Serial.println("GPBOD");
            Get_GPBOD();
            return;              
        }   
       
        if (GPSheader == "GPWPL")
        {  
         // Serial.println("GPWPL");
            Get_GPWPL();
            return;              
        }     
        
          if (GPSheader == "GPRTE")
        {  
         // Serial.println("GPRTE");
            Get_GPRTE();
            return;              
        }     
   }  // end if(USe_CTS)
   
       NewData=false;
 } // End of Void GET_sentence()

   

    void Checksum_calc(){
      
       /**********************************************
        From garmin.com search support for "how is checksum calculated in NEMA 0183
        The checksum is the 8-bit exclusive OR (no start or stop bits) of all characters in the sentence, including the "," delimiters, 
        between -- but not including -- the "$" and "*" delimiters. The hexadecimal value of the most significant and least significant
        4 bits of the result are converted to two ASCII characters (0-9, A-F) for transmission. The most significant character is transmitted first. 
        Therefore in the routine below the counter starts at 1 to skip "$" in  checksum routine and ends at index of "*".
      ***********************************************/      
             int index=0;                  
                              //  Serial.println("");                          // diagnostic may be commented out
                              //  Serial.print("DATA TO BE CHECK SUMMED  ");   // diagnostic may be commented out
                                checksum=0;
    				for(int x=1; x<100; x++){  // you have to skip the $ sign and it works if x starts at 1
				        if (gps_buffer[x] == '$'){
                                        index=x;}
                                      break;}
                                     //   Serial.print("index = ");  // diagnostic may be commented out
                                      //  Serial.println(index);  // diagnostic may be commented out
                                    for(int x=index+1; x<100; x++){    
                                        if(gps_buffer[x]=='*'){ 
					checksum_received = strtol(&gps_buffer[x + 1], NULL, 16);//Parsing received checksum...			
                                        
                                          break; 
					}else{ 
					checksum ^= gps_buffer[x]; //XOR the received data... 
                                       // Serial.print(gps_buffer[x]);  // this should = gps_buffer less $ in and * at end which is data to checksum
					}}
				       
					checksum_status = false;
					if(checksum_received == checksum){ 
                                        checksum_status=true; 
                                          }
                                       
                                     /***********************************************/
                                     // this section added to diagnose checksum and is no longer needed
                                       if(print_checksum) { 
                                         //Checking checksum Convert hex number to hex decimal representation i.e. 15 = F
                                            // Lines below are diagnostic and may be commented out   
                                                 temp = checksum_received/16;
                                                 temp2 = checksum_received-temp*16;
                                                 temp4 = String(temp2);
                                                 //temp3 = temp*10 + temp2;
                                                     if(temp2 == 10){temp4="A";}
                                                      if(temp2 == 11){temp4="B";}
                                                      if(temp2 == 12){temp4="C";}
                                                      if(temp2 == 13){temp4="D";}
                                                      if(temp2 == 14){temp4="E";}
                                                      if(temp2 == 15){temp4="F";}
                                                      temp5 = String(temp);
                                                      temp5 += temp4;
                                        
                                        
                                          //printing checksums for a check 
                                        Serial.print("checksum received hexadecimal = "); 
                                        Serial.println(temp5);
                                        Serial.print("checksum received decimal =");
                                        Serial.println(checksum_received);
                                        Serial.print("checksum calculated = "); 
                                        Serial.println((checksum));
                                        Serial.println("***********************");
                                       }  //  end if print_checksum
                                       
                                       /****************************************************/
                                        
        }    // End of Checksum subroutine
        
