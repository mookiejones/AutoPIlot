/*  COMMENTS
 *  This code runs on the Arduino Mega that is set up to run the Autopilot code 
 *  i.e. it has the same wiring for IMU, LCD, and KeyPad.
 *  It's pupose is to collect data for calibrating the Pololu IMU9 compass
 *  It captures the running maximum and minmum magnetometer values and saves them to the LCD screen
 *  Recently a capability to capture the raw magnetometer readings and take an average value.
 *  Also I have added code so you can have the compass set up on a board with a compass rose so the  
 *  compass can be rotated.  The user will first input the compass heading on the keypad and press #
 *  The program will run and take a average of the magnetometer XYZ and accelerometer XYZ.  When it has taken the 
 *  average it will print the values to the serial monitor. After taking a round of data the data on the serial 
 *  monitor can be cut and pasted into a spread sheet to plot and analyze the data.  Have not done accelerometer data yet.
 *  Set CompassCalibrate = 1 to perform this option.
 *   
 */
#include <Wire.h>
#include <LSM303.h>
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(41,43,45,47,49,51);
LiquidCrystal lcd(41,43,45,47,49,39);

//#include <Key.h>
#include <Keypad.h>
 //  KEYPAD SETUP
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {34,44,42,38}; //connect to the row pinouts of the keypad  JACK'S KEYPAD
byte colPins[COLS] = {36,32,40}; //connect to the column pinouts of the keypad  JACK'S KEYPAD
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LSM303 compass;
LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};
boolean serial_Print_rawdata=0;
boolean LCD_Print_rawdata = 0;
boolean serial_Print_maxmin=0;
boolean LCD_Print_maxmin = 0; // this prints the maximum and minimums captured
boolean serial_Print_drift =0;
boolean CompassCalibration = 1; // captures avg compass.m .x .y .z and prints to serial monitor along with reference heading
float smooth_fraction = .01; // .01 means 100 point low pass filter
float Xsmooth;
float Ysmooth;
float Zsmooth;
long timer;
long timer1;
long timer2;
long timer3;
long timer4;
long counter;
String inHeading = "";
long Heading;
boolean newHeading = 0;
boolean juststarted = 1;
float XmTotal = 0;
float YmTotal = 0;
float ZmTotal = 0;
float XmAvg = 0;
float YmAvg = 0;
float ZmAvg = 0;
int n = 0;
void setup() {
  Serial.begin(57600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  lcd.begin(20,4);
//  keypad.addEventListener(keypadEvent); //add an event listener for this keypad

}
char key = 0;

void loop() {
 
  compass.read();
  
  running_min.x = min(running_min.x, compass.m.x);
  running_min.y = min(running_min.y, compass.m.y);
  running_min.z = min(running_min.z, compass.m.z);

  running_max.x = max(running_max.x, compass.m.x);
  running_max.y = max(running_max.y, compass.m.y);
  running_max.z = max(running_max.z, compass.m.z);

  Xsmooth =(1-smooth_fraction) * Xsmooth + smooth_fraction * compass.m.x;
  Ysmooth =(1-smooth_fraction) * Ysmooth + smooth_fraction * compass.m.y;
  Zsmooth =(1-smooth_fraction) * Zsmooth + smooth_fraction * compass.m.z;

  
  
  if(CompassCalibration){

         if(juststarted){
         Serial.println("Average Magnetometer Readings"); 
         Serial.println("Heading,  XmAvg,  YmAvg,  ZmAvg");
         juststarted = 0;         
        }
        
      newHeading = 0;
      lcd.begin(20,4);  // clears the screen
      lcd.setCursor(0,0);
      lcd.print("Enter Heading or #");
      lcd.setCursor(0,1);
        while(newHeading !=1){ 
          KEYPAD();
        }
      Serial.print(Heading);
      for(int avgCounter =0; avgCounter<1000; avgCounter++){
        compass.read();
        XmTotal = XmTotal + compass.m.x;
        YmTotal = YmTotal + compass.m.y;
        ZmTotal = ZmTotal + compass.m.z;
        n = avgCounter;
      }
     XmAvg = XmTotal/n;
     YmAvg = YmTotal/n;
     ZmAvg = ZmTotal/n;
     
     Serial.print(", "); Serial.print(XmAvg,1); Serial.print(", "); Serial.print(YmAvg,1); Serial.print(", "); Serial.println(ZmAvg,1);
     XmTotal = YmTotal = ZmTotal = 0;
     XmAvg = YmAvg = ZmAvg = 0; 
      }
  
  if (serial_Print_maxmin)
  {
   // if(millis() - timer1 > 100) {
   
      timer1 = millis();
  Serial.print("M min ");
  Serial.print("X: ");
  Serial.print((int)running_min.x);
  Serial.print(" Y: ");
  Serial.print((int)running_min.y);
  Serial.print(" Z: ");
  Serial.print((int)running_min.z);

  Serial.print(" M max ");  
  Serial.print("X: ");
  Serial.print((int)running_max.x);
  Serial.print(" Y: ");
  Serial.print((int)running_max.y);
  Serial.print(" Z: ");
  Serial.println((int)running_max.z);
 // }
  }
  
  if( serial_Print_rawdata)  
  {
    if(millis() - timer2 >500)
  {
    timer2=millis();
  Serial.print(compass.m.x); Serial.print(", "); 
  Serial.print(compass.m.y); Serial.print(", ");
  Serial.print(compass.m.z); Serial.println();
  }
  }
  //This block prints Max and Mins
  if (LCD_Print_maxmin)
  {
      lcd.setCursor(0,0);
      lcd.print ("XMIN   YMIN   ZMIN  ");
      lcd.setCursor(0,1); lcd.print("                    "); 
      lcd.setCursor(0,1);  lcd.print((int)running_min.x);
      lcd.setCursor(7,1); lcd.print((int)running_min.y);
      lcd.setCursor(14,1); lcd.print((int)running_min.z);
      lcd.setCursor(0,2);
     
      lcd.print ("XMAX   YMAX   ZMAX  ");
      lcd.setCursor(0,3); lcd.print("                    ");
      lcd.setCursor(0,3);  lcd.print((int)running_max.x);
      lcd.setCursor(7,3); lcd.print((int)running_max.y);
      lcd.setCursor(14,3); lcd.print((int)running_max.z);
  }

 
     //This block prints raw compass readings
      if(LCD_Print_rawdata) 
     {  

     if(millis() - timer >1000)
    {
      timer = millis();
     lcd.setCursor(0,0);
     lcd.print(Xsmooth);
     lcd.setCursor(0,1);
     lcd.print(Ysmooth);
     lcd.setCursor(0,2);
     lcd.print(Zsmooth);
    }
     }

  // print every 10 minutes to watch drift

 if (serial_Print_drift)
 {
  if (millis()/1000 - timer3 >= 600) //1 min = 60 
 { 
   timer3 =  millis()/1000;
   counter += 1;
   Serial.print(counter);
   Serial.print(", ");
   Serial.print(Xsmooth);
   Serial.print (", ");
   Serial.print(Ysmooth);
   Serial.print (", ");
   Serial.print(Zsmooth);
   Serial.println();
 }
 } 
}

