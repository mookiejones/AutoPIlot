
/******************* PARSE SENTENCE *************************/

void Parse_Sentence()
{
  char comma = ',';
     char star = '*';
     int last_k =0;
     int k_start = 0;
     Word_count = 0;
     
   //  j_Max = 14; // number of words for NEMA type set j_MAX in calling subroutine
     
       Checksum_calc();   // checksum calculation subroutine
        if (checksum_status)
        {
          NEMA_sentence = true;
           
      for (int j=0; j<j_MAX + 1; j++)
      {data_IN[j] = "";
      }
              
                for (int j=0; j<j_MAX; j++)
                {  
                       for(int k = k_start; k < buffer_length; k++)
                       {   
                              data_IN[j] = data_IN[j] + gps_buffer[k]; 
                              last_k = k; 
                              if (gps_buffer[k] == comma || gps_buffer[k] == star )
                               {
                                 data_IN[j] = data_IN[j].substring(0,data_IN[j].length() - 1); //cuts off last character which is comma or star
                                 Word_count = Word_count +1;
                                 break; 
                               }   
                       }  // end for k
                           
                        k_start = last_k +1;
                        

                }// end for j  
      } // end for i 
}  // end Parse_Sentence
/******************  CONVERT LAT/LON TO DEGREES  *************************/

 void To_Degrees(String Lat_Lon)
{

  // NOTE ASSUMES NORTH LATITUDE AND WEST LOGITUDE ARE POSITIVE NEED TO ADD HANDLING FOR SOUTH LATITUDE AND EAST LOGITUDE (treat them as negative
  
String Degrees_str;
String Minutes_str;
String Decimal_str;
float Degrees;
float Minutes;
float Decimal;
int N1;
 int N2;
 int N3;
 int N4;

if(Input_Source == 1)
{
N1=4;  N2=5; N3=7; N4=10000;
}


if (Input_Source == 2)
{
  N1 = 3; N2 = 4; N3 = 6; N4 = 1000;
} // end if input source == 2 Input Source 2 is Nobeltec and has 3 deimals for Lat/Lon, Garminmap 60CSX has 4 

if (Input_Source == 3)
{
N1=4;  N2=5; N3=7; N4=10000;
}


//float Lat_Lon_Deg;
              //  temp = strtol (Lat_Lon, &pEnd, 10);
               // temp *=100;  //takes only the integer part x 10  
	       // temp2 = strtol (pEnd + 1, NULL, 10);  //takes the decimal part as an integer   
	       // temp3 = temp + temp2;  // adds two parts = Course to steer x 10  

// Lat/Lon have form ddd.mmpp(p, ddd is degrees (1 to 3 characters, mm is min, and ppp(p) is decimal minutes 3 o 4 characters
string1 = Lat_Lon.substring(Lat_Lon.length() - N1);// last 4 characters of Lat_Lon
string1.toCharArray(char_buf,16);
long1 = strtol(char_buf,&pEnd,10);
Decimal = float(long1)/N4; // convert integer to decimal

string1 = Lat_Lon.substring(Lat_Lon.length()-N3, Lat_Lon.length()-N2); // gets minutes
string1.toCharArray(char_buf,16);
long1 = strtol(char_buf,&pEnd,10);
Minutes = float(long1);

string1 = Lat_Lon.substring(0, Lat_Lon.length()-N3); // gets degrees
string1.toCharArray(char_buf,16); 
long1 = strtol(char_buf,&pEnd,10);
Degrees = float(long1);

Lat_Lon_Deg = Degrees + (Minutes + Decimal)/60;


//Serial.print("Degrees = ");
//Serial.println(Degrees_char);

//Serial.print("char_buf = ");
//Serial.println(char_buf);

//Serial.print("temp = ");
//Serial.println(temp);
//Serial.print("temp2 = ");
//Serial.println(temp2);

//Serial.print("Degrees float = "); Serial.println(Degrees);

//Serial.print("Minutes = ");
//Serial.println(Minutes);

//Serial.print("Decimal = ");
//Serial.println(Decimal,4);

//Serial.print("Lat_Lon = ");
//Serial.println(Lat_Lon_Deg,4);

} // end To_Degrees

/************************************************************************/
int To_Integer(String string1)
{
  int int1;
  string1.toCharArray(char_buf,16);
  int1 = strtol(char_buf,&pEnd,10);
  return int1;
}  // end To_Integer

/******************* CONVERT NEMA INPUT TO FLOATING VARIABLE *****************/

void NEMA_TO_FLOAT(int N)
{  // this needs work not getting decimal part. need new approach so i don't have to identfy how many decimals.
 //string1 input, float3 output  
    string2 = string1.substring(string1.length()- N);// last N characters of data
    string2.toCharArray(char_buf,16);
    long1 = strtol(char_buf,&pEnd,10);
    float1 = float(long1)/pow(10,N);
    //Serial.println();
   // Serial.println(float1);
    string2 = string1.substring(0, string1.length() - N+1);// all but last N+1 characters, includes decimal point
   // Serial.println(string2);
    string2.toCharArray(char_buf,16);
    long1 = strtol(char_buf,&pEnd,10);
    float2 = float(long1);
    //Serial.println(float2);
    float3 = float1 + float2;
}  // END NEMA TO FLOAT



    /************************************************/
    void Range_Bearing_Between_Points(int j, float Lat_from, float Lon_from, float Lat_to, float Lon_to)
    {
      float Dx1, Dy1;
      float bearing;
      float range;
      
      
      Dx1 = cos(Lat_from*PI/180)*360000*(Lon_to - Lon_from); 
      Dy1 = 360000*(Lat_to - Lat_from);
      bearing = (180/PI) * atan2(Dx1 , Dy1); // note 180/PI*atan2 returns -180 to +180 accounts for sign Dx and Dy
      if(bearing < 0 ) bearing = 360 + bearing; 
      range = sqrt(Dx1*Dx1 + Dy1*Dy1);
      Range_and_Bearing[0] = range;
      Range_and_Bearing[1] = bearing;
      if(j>=0)  // This routine was set up to calculate the bearing from waypoint to waypoint.  To use it to calculate range and bearing
                // from a position to a second position or waypoint use a negative value in the input.  This skips assigning results to watpoints 
      {
        Waypoint_Range_From[j] = range;
        Waypoint_Bearing_From[j] = bearing;
      }
    }  // end void Range and Bearing

/***********************  WAYPOINT  AGE  ****************************/
void Waypoint_Current()
{
  /* Waypoint current checks to see if the waypoint next has been updated within a specified amount of time
   *  if not the waypoint next is set to NO WPT. Necessary because nav programs quit sending GPAPB while others send GPAPB 
   *  but set the waypoint next to null,"", which prgram detects but does not detect thatGPAPB is not sent.
   */
  int Waypoint_Age_Max = 6000; // in milli seconds
   if (millis() - Waypoint_Age > Waypoint_Age_Max) 
   {
    Waypoint_next = "NO WPT";
    Serial.println("Waypoint_Age > 6");    
   }
  // else Waypoint_Age_Old = millis();
}  // end waypoint current

