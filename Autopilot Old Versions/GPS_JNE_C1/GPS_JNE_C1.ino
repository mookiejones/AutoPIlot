  
  
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
      // use if(ET.receiveData()) before trying to read data.
// NOte need to add code to check that all active NEMA sentences are current

  *********************************************************/
  #include <EasyTransfer.h> // for arduino to arduino transfer
  
   int Input_Source = 1; // 1 = Garmin GPS60CSX(has $GPRMC & $GPAPB, Garmin GPSmap 198C also // I ought to change this to sw 2 position
     // 2= Nobeltec (missing Last term of $GPRMC and $GPAPB for GPS status. note seems to work with GPS 60CSX in Source 2
     //3 = same as 1 may need to change 198 c does not have APB but does have BOD
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
   boolean print_Nav_Data = 0;
   int oldtime = 0;
   int newtime= 0;
   int deltatime=0;
   int byteGPS=-1;
   const int buffer_length = 200;
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
   int j_MAX;
   int Word_count= 0;
   String Route[20];
   float Waypoint_Lat [20];  //note intend to store waypoint data here with same index as Route[20]
   float Waypoint_Lon [20];
   float Waypoint_Range_From[20]; // range from previous waypoint to this waypoint
   float Waypoint_Bearing_From[20]; // beraing from previous waypoint to this waypoint
   float Next_Turn[20];
   float NEXT_TURN; 
   long Number_of_sentences = 0;
   int Current_sentence_number = 0;
   int Number_of_waypoints;
   boolean Route_Completeness = 0; //GPRTE reports data as c - complete, or w for working see NMEA standard
         // for c all waypoints are sent.  For w the previous waypoint, the destination wpl and the rest of the wpls is reported
         //  code only currently treats complete routes
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
 // float Lat_origin;
 // float Lon_origin;
  long Waypoint= 0;
  long Latitude = 0;
  float Bearing_origin_to_destination = 0;
  String BOD_MorT = "";
  float Bearing_to_destination = 0;
  String BTD_MorT = "";
  float Range_Destination;
  float Velocity_towards_destination;
  long UTC;
  String UTC_string;
  long Date;
  
 
  
   //create object
  EasyTransfer ET; 
  
  struct SEND_DATA_STRUCTURE{  //Arduino to Arduino communication Eazy Transfer by Bill Porter
    //put your variable definitions here for the data you want to send
    //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float SD_course;
  //float SD_course_to_steer;
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
 
     //   ETdata.SD_NEMA_sentence = NEMA_sentence;      
       if(NewData)
        {  
          Get_Cross_Track_Error();
          ET.sendData();
         // Serial.println();
         // print_ETdata = false;
          NewData = false; // Set to true in GPRMC sends data once per loop through GPRMC
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
   
   void PRINT_ETDATA()
      {
          Serial.print("course: ");
          Serial.println(ETdata.SD_course);
           // Serial.print("course to steer: ");
           //Serial.println(ETdata.SD_course_to_steer );
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
           Serial.print("Waypoint next: ");
           Serial.println(ETdata.SD_Waypoint_next );
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
           Serial.println(ETdata.SD_Range_Destination );           
           Serial.print("UTC: ");
           Serial.println(ETdata.SD_UTC);
           Serial.print("Date: ");
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
    
