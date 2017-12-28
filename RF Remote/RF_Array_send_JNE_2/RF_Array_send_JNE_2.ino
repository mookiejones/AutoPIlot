
// V3 added the wiring comments in line 6 to 20
#include <SPI.h>
#include "RF24.h"

 /* 
Sending formatted data packet with nRF24L01. 
Maximum size of data struct is 32 bytes.
 contributted by iforce2d

The wire numbers listed are using a ribbon wire and a two row ribbon wire connector
1 - GND, wire 2
2 - VCC 3.3V !!! NOT 5V, wire 1
3 - CE to Arduino pin 9, wire 4
4 - CSN to Arduino pin 10, wire 3 
5 - SCK to Arduino pin 13 for Uno, 52 on Mega, wire 6
6 - MOSI to Arduino pin 11 for Uno, 51  on Mega, wire 5
7 - MISO to Arduino pin 12 for Uno,  50 on Mega, wire 8
8 - UNUSED, wire 7
*/

// Your chosen radio CE,CS pins
RF24 radio(9,10);

uint64_t addresses[] = {0xABABABABABLL, 0xC3C3C3C3C3LL};

/****** Configure this for sender or receiver *****/
              
int role  = 1; // 1 send, 0 receive
// load program on two arduinos one with role = 0 and one with role = 1

/*** data structure  ***/
struct JNEData {
  char RF_Waypoint_next[20];
  float float0;
};
JNEData RF;

void setup() {
 
 Serial.begin(57600);
 radio.begin(); 

  if(role == 0){
   radio.openWritingPipe(addresses[1]);
   radio.openReadingPipe(1,addresses[0]);
   radio.startListening();
 }else{
   radio.openWritingPipe(addresses[0]);    //*Edit to correct address assignments
   radio.openReadingPipe(1,addresses[1]);
   radio.stopListening(); 
 }
}

String myArray = "This is my test data"; //type any characters

float heading = 159.64;

void loop() {
  myArray.toCharArray(RF.RF_Waypoint_next,20); 
  RF.float0 = heading;
  
 if(role == 0){ //0 is receiver
     if(radio.available()){
      radio.read(&RF, sizeof(JNEData));
      Serial.println(RF.float0);
      Serial.println(RF.RF_Waypoint_next);
     }   
 }else{
 
     bool ok = radio.write(&RF, sizeof(JNEData)); 
     Serial.print("data size: "); Serial.println(sizeof(JNEData));
     if(ok)
     { Serial.println("Transfer OK");}
     else { Serial.println("Transfer Fail"); 
     }
     delay(2000);
 }

}
