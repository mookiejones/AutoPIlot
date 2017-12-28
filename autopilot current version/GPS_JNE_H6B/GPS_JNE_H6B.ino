   
  
  /*********************************************************
  
  This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 Unported License. It my be used by others within the License restrictions granted
  by Creative Commons and And the Code authors. This Code is created by Jack Edwards and others as referenced
  in various sections.
  
  
  GPS_JNE_1_0:  Stand alone GPS read and Parse to run on separate Arduino
  and then transmit data to Main Arduino faster. GPS reading inside Autopilot
  is too slow for MinIMU9 to operate at 50 Hz
  V 1_2 working model
  V1_3 1_2 worked with Garmin  GPS 60 CSX, 
  V1_5 added transmission of UTC so you can verify GPS is updating with seconds updating
  V1_6 adds support to get input from Nobeltec
  V 1_7 commented out Waypoint_next in Data Structure and GPAPB to stop chrashes
  V 2_0 fixed data structure crashes. V 2_0 is the go to the boat test model
  V 35 starting from 2_0  put in a couple serial prints for fix status
   V36  add tabs
   V 37 restructure GET_sentence, GPAPB and GPRMC
   V 38 and 39 moved checksum call to each NEMA sentence Tab
   V 4_0 New working model with tabs and restructured read header then process appropriate sentence Streamlined NEMA parsing
   V 4_3_1 moved print sentences to separate voids and commented out. RMB call does immeadiate return.  Next version to work on RMB
   V 4_3_2 work on RMB
   V 4_3_4 last series for UNO
   V 5 port GPS to MEGA
   V 5_4_2 working version with good RMB and good data transfer
   V 5_5 same as 5_4_2
   V 5_5_1 changed UTC long to UTC string so if hour is 00 data is transfer as 00 not a blank
   V 5_5_2  added XTE_differential_error, deleted bearing to destination
   V5_5_2N is Nobeltec version. the Void To_Degrees(Lat_Lon) uses 4 decimals for Garmin60 input of Lat/Lon, and Nobeltec outputs 3 decimals
             need to make it an input parameter
   V 5_5_3 uses Input_Source to set correct decimal length. should generalize by counting digits after decimal
   V 5_5_4 10/4/13 mods to read Garmin GPSmap 198C Sounder Added TAB GPBOD
           10/9/13 delete course to steer from APB add Bearing from RMB calculate course to steer in AP
  V5_5_5 Did some cleanup, take out old references like to the sofware serial.
  V5_5_5_6 had to undo some of my cleanup I had commented out range and bearing to destinatiuon
  V 5_5_7 SAME AS 6 STARTING TO ROUTE AND WAYPOINT CAPTURE
  V 5_5_7_1 capture waypoints and route in an array
  V 5_5_7_2 in 2 I will load route and waypoints intoconsolodated array, added Lat/Lon to ETdata, Ported Lat and Lon over.  Must
              // be used with AP_6_7_6_6 or later
  V B1_0 same as V 5_5_7_2 renamed to indicate it must be used with AP JNE B1_0 or later 
  V B1_1 Added Range and beraing between waypoints and next turn 
  V C1  Use with C series of Autopilot and Serial Remote got the serial remote TFT working.  Changed how I get Easy Transfer data
  V F1 C2 to F1 skipped.  F1 added Anticipate turn. send new waypoint before getting to current waypoint
        use if(ET.receiveData()) before trying to read data.
   Note need to add code to check that all active NEMA sentences are current
   V C1 works with AP D series, AP D took care of checking GPS availability by monitoring UTC
  C2 commented out lat and Lon in RMB, not needed and they add time
  V F2 - Precision limit for cross track error set by BOD and Bearing to destination of 0.1 degrees of bearing precicion
   which is 15 ft at a mile.  This results in lumpy behavior.  F2 will compute bearing to destination from Lat Lon which is 4 digits of precision and should 
   get cross track error to a foot. 
  V F3 corrections to bearing from WPL lat and Lon, Anticipate turn off
  V F4 tested and working pretty good, turn anticipation not working
  V F4a revised turn anticipation
  V F4b3 commented out Bod and waypoint next in void loop so they only get set in tabs RMB and BOD objective to get refined XTE to work without anticipate turn
  V G0 Same as F4b3.  Worked well in GPS mode from Swinomish to Holmes Hbr Save as a Baseline 7_4_14
  V G1 fixed et data to transmit waypoint next, goofed it up earlier
  V G2 changed formulation of XTE rate so it  = Dxte/Dt Changed Subtraction of two angles correction for 360
  V G2B, G2 was the version tested August 2014,  G2B added a comment to call Destination_Bearing(); in XTE do structural change.
  V G3 Update ad Debug Anticipate turns, if( Waypoint_next = Route[i]) to == in Anticipate_Turn()v2
  V G3 V2 read and send APB course_to_steer In auto, in autopilot varible will be GPS_course_to_steer
  VG3 V3 commented out WPL,RTE,BOD Newdata XTE trying to speed up processing this version will use CTS from APB
  V H1 same as VG3 V3 9/21/15 this worked great with my Garmin 740s and is final version for sumer of 2015
  H2 restore things removed from v3 but make using course to steer from the GPS an option.  then add new anticipate turn structure
  H4 Working version with anticipated turns, renumbered to be the same as the AP version 
  H5 Found problem with goto waypoint as opposed to follow route.  destination Lat/Lon not captured because WPL sentence
     not sent.  this caused big overflow because lat/ lon of zero was used. Uncommented destination Lat/Lon in $GPRMB  Seems to work 
     Added if waypoint number = 0 in ANTICIPATE TURN then return This keeps the RMB waypoint as WPT next. also turn off RMB destination
     lat lon when anticipate turns  = 1 and number of waypoints >0
  H6 Same as H5 except user input.  This version tested 3-25-16 with AP...J9a with pololu compass and was woring like videoed versions last fall Save working model
  H6B 6-11-16 Added code to each place waypoint next set to get Waypoint_next age. so it is set to NO WPT if older than 6 seconds. See Waypoint_Current() in Subroutines
        Waypoint age can be be updated in APB RMB and BOD if user's NAV program or GPS sends more than one user needs to check that they all send a null value for
        Waypoint_next or sentences are not sent when when User selects Stop Navigation
        Added NewData = true; to GPAPB and GPBOD (not just GPRMC) for more frequent sending of ETdata to autopilot
 *********************************************************/
 
 /*************** USER INPUT ******************************/
  #include <EasyTransfer.h> // for arduino to arduino transfer
  
/**********   USER DATA ***************/
 
   int Input_Source = 1; // 1 = Garmin GPS60CSX(has $GPRMC & $GPAPB, Garmin GPSmap 198C also // I ought to change this to sw 2 position
     // 2= Nobeltec (missing Last term of $GPRMC and $GPAPB for GPS status. note seems to work with GPS 60CSX in Source 2
     //3 = same as 1 may need to change 198 c does not have APB but does have BOD
   float  Turn_distance = 200;  //  Anticipateturn distance in feet. for compute from how long to turn 360 deg and boat speed  = distance traveled/ 2 Pi
   boolean Anticipate_Turns = 0; // 1 yes, 0 no turns off turn anticpation
   boolean Use_CTS = 1;  //  1 to use the CTS (course to steer) from GPS sentence $GPAPB. turn Anticipate Turns off if GPS has this feature see video
   boolean print_NEMA = 0;  //1 = print, 0 = not print 
   boolean print_RMC = 0;  //1 = print, 0 = not print
   boolean print_APB = 0; //1 = print, 0 = not print
   boolean print_RMB = 0; //1 = print, 0 = not print
   boolean print_BOD = 0; //1 = print, 0 = not print
   boolean print_WPL = 0;  //1 = print, 0 = not print
   boolean print_RTE = 0;  //1 = print, 0 = not print
   boolean print_timing = 0;
   boolean print_checksum = 0;
   boolean print_ETdata = 0;
   boolean print_Nav_Data =0 ;
   
/************   END USER DATA   ************/ 
   
   
   int oldtime = 0;
   int newtime= 0;
   int deltatime=0;
   int byteGPS=-1;
   const int buffer_length = 84;//chnged from 200 oct-15-15
   char gps_buffer[buffer_length] = "";
   char gps_RMC[7] = "$GPRMC";
   char gps_APB[7] = "$GPAPB";
   char gps_RMB[7] = "$GPRMB";
   char gps_BOD[7] = "$GPBOD";
   char gps_WPL[7] = "$GPWPL";
   char gps_RTE[7] = "$GPRTE";
   int count_a=0;                            //buffer
   int bien=0;
   int count_b=0;  //  counter to put incoming data into gps_buffer
 //  int indices[15];
   String data_IN[17];
   String data_APB[17];
   String data_RMB[17];
   String data_RMC[17];
   String data_BOD[7];
   String data_WPL[6];
   String data_RTE[17];
   int j_MAX = 0;
   int Word_count= 0;      
   int GPS_WPT_index = 0;
   int WPT_index = 0;
   boolean Anticipated_Turn_Active = 0;
   String Route[20];
   float Waypoint_Lat [20];  //note intend to store waypoint data here with same index as Route[20]
   float Waypoint_Lon [20];
   float Waypoint_Range_From[20]; // range from previous waypoint to this waypoint
   float Waypoint_Bearing_From[20]; // beraing from previous waypoint to this waypoint
   float Next_Turn[20];
   float NEXT_TURN; 
  // boolean Turn_Anticipation = false;
   String Active_waypoint;
   long Number_of_sentences = 0;
   int Current_sentence_number = 0;
   int Number_of_waypoints;
   int word_count;
   boolean Route_Completeness = 0; //GPRTE reports data as c - complete, or w for working see NMEA standard
         // for c all waypoints are sent.  For w the previous waypoint, the destination wpl and the rest of the wpls is reported
         //  code only currently treats complete routes
   unsigned long RTE_timer;
   boolean RTE_Active = 0;   
   char *brkb, *pEnd;
   unsigned  long temp = 0;  
   unsigned long temp2 = 0;  ; 
   String temp4 = "";
   String temp5 ="";
   char char1;
   char char2;
   char char3;
   String string1;
   String string2;
   String string3;
   unsigned  long long1;
   unsigned long long2;
   unsigned long long3;
   float float1;
   float float2;
   float float3;
   int int1;
   char char_buf[16];
   long checksum_received = 0; // or static byte
   long checksum = 0;
   boolean checksum_status = false;
   boolean GPS_available= false;
  String GPScase = "";
  String GPSheader = "";
  boolean NEMA_sentence = false;
  boolean GPRMC_fix = false;
  String GPRMC_fix_status = "";
  boolean GPAPB_fix = false;
  String GPAPB_fix_status = "";
  boolean GPRMB_fix= false;
  String GPRMB_fix_status = "";
  boolean NewData = false;
  unsigned  long Waypoint_Age; // Waypoint_Age_Old;
  float course;
  float course_to_steer;
  String Lat_Lon;
  float Lat_Lon_Deg;
  String CTS_MorT;
  float SOG;
  float MagVar; //Magnetic Variation E is plus, W is minus
  float XTE;
  float XTE_differential_error;
  String XTE_LR;
  String XTE_unit;
  String Nullstring;
  String Waypoint_next; 
  float Lat_current;
  float Lon_current;
  float Lat_destination;
  float Lon_destination;
  String Origin_Waypoint;
  String Waypoint_name;
  float Lat_Waypoint;
  float Lon_Waypoint;
  float Current_Waypoint_Lat;
  float Current_Waypoint_Lon;
 // float Lat_origin;
 // float Lon_origin;
  long Waypoint= 0;
  long Latitude = 0;
  float Bearing_origin_to_destination = 0;
  String BOD_MorT = "";
  float Bearing_to_destination = 0;
  float Active_Bearing_to_destination;
  float Bearing_to_destination_by_LatLon= 0;
  String BTD_MorT = "";
  float Range_Destination;
  float Active_Range_Destination;
  float Velocity_towards_destination;
  unsigned long UTC;
  String UTC_string;
  long Date;
  float Range_and_Bearing[2];
  
 
  
   //create object
  EasyTransfer ET; 
  
  struct SEND_DATA_STRUCTURE{  //Arduino to Arduino communication Eazy Transfer by Bill Porter
    //put your variable definitions here for the data you want to send
    //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float SD_course;
  float SD_course_to_steer;
  //char SD_CTS_MorT[2];
  float SD_SOG;
  float SD_MagVar;
  float SD_XTE;
  float SD_XTE_differential_error;
  // char SD_XTE_LR[2];
  // char SD_XTE_unit[2];
  char SD_Waypoint_next[11]; 
  //boolean SD_NEMA_sentence;
  boolean SD_GPRMC_fix;
  boolean SD_GPAPB_fix;
  float SD_Bearing_origin_to_destination;
  char SD_BOD_MorT[2];
  //char SD_Origin_Waypoint[11];
  float SD_Bearing_to_destination;
 // char SD_BTD_MorT[2];
  float SD_Range_Destination;
 // float SD_Velocity_towards_destination; 
  long SD_UTC;
 // long SD_Date;
 // float SD_Lat_current;
//  float SD_Lon_current;
  float SD_NEXT_TURN;
  //float SD_Time_decimal;
 }; // End Data Sttructure  
  
  SEND_DATA_STRUCTURE ETdata;
  
    
   void setup()
   {
     Serial.begin(57600); // Serial conection to Serial Monitor 
     Serial2.begin(4800); // Serial Input for GPS data
     Serial3.begin(57600); // Serial conection for ET.data to other Arduino
     ET.begin(details(ETdata), &Serial3);            
   }
   
   void loop()
   {  
        GET_sentence(); 
        Waypoint_Current();  // if Waypoint_next is not updated in Waypoint_Age_Max = 6 sec Waypoint_neaxt = "NO WPT"
     //   ETdata.SD_NEMA_sentence = NEMA_sentence;      
       if(NewData)
        { 
          Active_waypoint = Waypoint_next; 
          if (millis() - RTE_timer < 60000) RTE_Active = true; // less than one minute since last RTE sentence processed
          if(Anticipate_Turns && RTE_Active) ANTICIPATE_TURN(); // if distance to waypoint is less than turning distance set the active waypoint as Waypoint_next
          if (Anticipated_Turn_Active)
          {
            Range_Destination = Active_Range_Destination;
            Bearing_to_destination = Active_Bearing_to_destination;
          }
          ETdata.SD_Range_Destination = Range_Destination;
          ETdata.SD_Bearing_to_destination = Bearing_to_destination;
          
          Get_Cross_Track_Error(); // gets cross track error and calculated course_to_steer
          Active_waypoint.toCharArray(ETdata.SD_Waypoint_next,11);
          ETdata.SD_Bearing_origin_to_destination = Bearing_origin_to_destination;
          if(Use_CTS == 0) course_to_steer = -1;  // AP can use the negative value to determine wether to use incoming CTS from APB or to calculate CTS from BOD and XTE
          ETdata.SD_course_to_steer = course_to_steer; 
          ET.sendData();
         // Serial.println();
         // print_ETdata = false;
        //  NewData = false; // Set to true in GPRMC sends data once per loop through GPRMC
          //Serial.println(UTC_string);
         if (print_ETdata) PRINT_ETDATA();
        }  
         
 
     //Will print loop interval time
    if (print_timing){
        newtime = millis();
        deltatime = newtime-oldtime;
        oldtime = newtime;
         Serial.println();
         Serial.print("time (seconds) = ");
         Serial.print(millis()/1000);
         Serial.print(",  delta time (milliseconds) = ");
         Serial.println(deltatime);
    }  // end if print timing
   } // end void loop()
   
   /************************************************************/
   void ANTICIPATE_TURN()
   // if distance to waypoint less than turn distance this sets Waypoint_next and BOD to the values for the waypoint after waypoint next 
   {
     if(Number_of_waypoints == 0) return; // This line added for GPS60 case where there is a goto waypoint but not a route and no $GPWPL
                 // which results in no WPL lat lon (i. = 0, 0 ) and 6000 mile xte so junk WPT next and XTE do know effect with other GPSs
   //  Serial.print("Waypoint Next "); Serial.println(Waypoint_next);
     for (int i = 0; i< Number_of_waypoints; i++)  // get the index of the current GPS destination waypoint
       {      
         if( Waypoint_next == Route[i])
           { GPS_WPT_index = i; // this is the index of the current waypoint coming from the GPS
          // Serial.print("GPS waypoint index "); Serial.println(GPS_WPT_index);
           break;
           } // end if Waypoint_next = 
       }  // end for 
     
      if( 6000 * Range_Destination < Turn_distance)
       {
        if(Anticipated_Turn_Active == false) // to only advance the index once each time anticipated turn is activated
        {
          WPT_index = GPS_WPT_index+1; 
          Anticipated_Turn_Active = true;
        }
       }
       else  // if range not < turn distance
       {
         /*  twitchy logic here.  If we get to a turn we want the index to advance.  After passing the orginal WPT the GPS
         should shift to the next waypoint which is the one the anticipate turn already advanced to.  we don't want the
         anticpated next waypoint to shift back to the old waypoint.  Likewise, when we pass the wapoint the GPS goes to the 
         next waypoint but if we are still < turn distance we don't want the index to advance a second time.
         */
        if( WPT_index ==GPS_WPT_index) // i.e. we have passed waypoint and GPS waypoint has advanced to our anticipated waypoint
         {
           Anticipated_Turn_Active = false;
         }
       }
      // Serial.print("Latitude current = "); Serial.println(Lat_current,4);
      // Serial.print("Longitude current = "); Serial.println(Lon_current,4);
      // Serial.print( Waypoint_Lat[GPS_WPT_index],4); Serial.print(",  ");  
       //Serial.print( Waypoint_Lon[GPS_WPT_index],4); Serial.print(",  ");
      // Range_Bearing_Between_Points(-1, Lat_current, Lon_current, Waypoint_Lat[GPS_WPT_index], Waypoint_Lon[GPS_WPT_index]);
       //Serial.print("WPT_index "); Serial.println(WPT_index);
       //Serial.print("Range Calc ");  Serial.print(Range_and_Bearing[0]); Serial.print("; BRG Calc ");  Serial.println(Range_and_Bearing[1]);  
      // Serial.print("Range GPS ");  Serial.print(Range_Destination *6000); Serial.print("; BRG GPS ");  Serial.println(Bearing_to_destination);  

         if(Anticipated_Turn_Active)
         {
          Active_waypoint = Route[WPT_index];  // this is the next waypoint after the GPS reported Waypoint_next 
          Bearing_origin_to_destination = Waypoint_Bearing_From[WPT_index];
          Range_Bearing_Between_Points(-1, Lat_current, Lon_current, Waypoint_Lat[WPT_index], Waypoint_Lon[WPT_index]);
          Active_Range_Destination = Range_and_Bearing[0]/6000;
          Active_Bearing_to_destination = Range_and_Bearing[1];
         }
        
   }  //  End ANTICIPATE_TURN
   
  /*************************************************************/ 
   void PRINT_ETDATA()
      {
          Serial.print("course: ");
          Serial.println(ETdata.SD_course);
            Serial.print("course to steer: ");
           Serial.println(ETdata.SD_course_to_steer );
           // Serial.print("CTS M or T: ");
          // Serial.println(ETdata.SD_CTS_MorT );
            Serial.print("speed: ");
           Serial.println( ETdata.SD_SOG);
            Serial.print("magnetic variation: ");
           Serial.println(ETdata.SD_MagVar );
            Serial.print("cross track error: ");
           Serial.println(ETdata.SD_XTE,0);
           Serial.print("differential cross track error: ");
           Serial.println(ETdata.SD_XTE_differential_error);
       //     Serial.print("XTE L/R: ");
       //    Serial.println(ETdata.SD_XTE_LR );
        //    Serial.print("XTE Units: ");
         //  Serial.println(ETdata.SD_XTE_unit);
           Serial.print( "RMB waypoint; "); Serial.println(data_RMB[5]);
           Serial.print("Waypoint next: ");
           Serial.println(ETdata.SD_Waypoint_next  );
           Serial.print("active WPT ");  Serial.println(Active_waypoint);
           Serial.print("GPS WPT Index "); Serial.print(GPS_WPT_index); Serial.print("; WPT_index ");Serial.println(WPT_index);
        //    Serial.print("NEMA Sentence: ");
       //    Serial.println(ETdata.SD_NEMA_sentence );
           Serial.print("GPRMC_fix: ");
           Serial.println(ETdata.SD_GPRMC_fix );
           Serial.print("GPAPB_fix: ");
           Serial.println(ETdata.SD_GPAPB_fix );
           Serial.print("Bearing origin to destination: ");
           Serial.println(ETdata.SD_Bearing_origin_to_destination );
           Serial.print("BOD M or T: ");
           Serial.println(ETdata.SD_BOD_MorT);
           Serial.print("Bearing Destination: ");
           Serial.println(ETdata.SD_Bearing_to_destination);
           
           Serial.print("Range To Destination: ");
           Serial.println(ETdata.SD_Range_Destination,3 );           
           Serial.print("UTC: ");
           Serial.println(ETdata.SD_UTC);
         //  Serial.print("Date: ");
           //Serial.println(ETdata.SD_Date );
         //  Serial.print("Latitude current = "); Serial.println(ETdata.SD_Lat_current);
         //  Serial.print("Longitude current = "); Serial.println(ETdata.SD_Lon_current);
           Serial.print("NEXT_TURN = "); Serial.println(ETdata.SD_NEXT_TURN);  
       //     Serial.print("Origin Waypoint: ");
       //    Serial.println(ETdata.SD_Origin_Waypoint );
        //    Serial.print("Bearing to destination: ");
        //   Serial.println(ETdata.SD_Bearing_to_destination );
         //   Serial.print("BTD M or T: ");
         //  Serial.println(ETdata.SD_BTD_MorT);

         //    Serial.print("Destination Bearing: ");
         //  Serial.println(ETdata.SD_Bearing_to_destination );
         //   Serial.print("Velocity_towards_destination: ");
         //  Serial.println(ETdata.SD_Velocity_towards_destination );
         
         //  Serial.print("Bearing to Destination ");
         // Serial.println(ETdata.SD_Bearing_to_destination );
           Serial.print(" ");
           Serial.println( );
        //   print_ETdata = false;
         }// endet print data
    
