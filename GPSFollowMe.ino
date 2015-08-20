#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
//test
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

// Choose two Arduino pins to use for software serial
// The GPS Shield uses D2 and D3 by default when in DLINE mode
int RXPin = 2;
int TXPin = 3;

// The Skytaq EM-506 GPS module included in the GPS Shield Kit
// uses 4800 baud by default
int GPSBaud = 4800;

int LatDeg = 0;
int LatMinLeft = 0;
int LatMinRight = 0;

int hot = 13;


int LonDeg = 0;
int LonMinLeft = 0;
int LonMinRight = 0;

int MylocationLat = 0;
int MylocationLatMinLeft = 0;
int MylocationLatMinRight = 0;

int MylocationLong = 0;
int MylocationLonMinLeft = 0;
int MylocationLonMinRight = 0;

int LonMinLeftDiff = 0;
int LonMinRightDiff = 0;

int LatMinLeftDiff = 0;
int LatMinRightDiff = 0;

int record = 7;


// Create a software serial port called "gpsSerial"
// This is how we will communicate with the GPS
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup() {
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.leftToRight();

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  pinMode(hot, OUTPUT);
  pinMode(record, INPUT);

}
void loop() {

  if (gpsSerial.available() >= 6 && gpsSerial.read() == '$' && gpsSerial.read() == 'G' && gpsSerial.read() == 'P' && gpsSerial.read() == 'G' && gpsSerial.read() == 'G' && gpsSerial.read() == 'A') {
    String gpsString = "";
    char c = 0;

    do {
      //Prints the data from the GPS onto the SD card and a Serial Monitor.
      if (gpsSerial.available()) {
        c = gpsSerial.read();
        gpsString.concat(String(c));

        LatDeg = gpsString.substring(12, 14).toInt();
        LatMinLeft = gpsString.substring(14, 16).toInt();
        LatMinRight = gpsString.substring(17, 21).toInt();

        LonDeg = gpsString.substring(24, 27).toInt();
        LonMinLeft = gpsString.substring(27, 29).toInt();
        LonMinRight = gpsString.substring(30, 35).toInt();



      }
    }

    while (c != '\n');

    Serial.print("The lat Degree  is ");
    Serial.println(LatDeg);
    Serial.print("The lat min is ");
    Serial.print(LatMinLeft);
    Serial.print(".");
    Serial.println(LatMinRight);

    Serial.print("The long degree is ");
    Serial.println(LonDeg);
    Serial.print("The long min is ");
    Serial.print(LonMinLeft);
    Serial.print(".");
    Serial.println(LonMinRight);

    Math();

    
    Serial.println(LatMinRightDiff);
    Serial.println(LonMinRightDiff);
    

    
  }

  









  if (digitalRead(record)) {

   MylocationLat = LatDeg;
   MylocationLong = LonDeg;
   MylocationLonMinRight =LonMinRight;
   MylocationLonMinLeft =LonMinLeft;


   MylocationLatMinRight = LatMinRight ;
   MylocationLatMinLeft = LatMinLeft  ;




   lcd.clear();
   delay(500);
   lcd.print("Recorded");
   lcd.setCursor(0, 1);
   lcd.print("Location !");
   delay(2000);
 }

 else {


  lcd.setCursor(0, 0);
  lcd.print("Lat:");
  lcd.print(LatDeg);
  lcd.print(" N ");
  lcd.print(LatMinLeft);
  lcd.print(".");
  lcd.print(LatMinRight);
  lcd.setCursor(0, 1);
  lcd.print("Lon:");
  lcd.print(LonDeg);
  lcd.print(" W ");
  lcd.print(LonMinLeft);
  lcd.print(".");
  lcd.print(LonMinRight);

  // lcd.setCursor(0,0);
  // lcd.print("Lat Diff: ");
  // lcd.print(LatMinRightDiff);
  // lcd.setCursor(0,1);
  // lcd.print("Lon Diff: ");
  // lcd.print(LonMinRightDiff);
  // delay(1500);
  // lcd.clear();
}










  if (MylocationLat == LatDeg && MylocationLong == LonDeg) {
    //Serial.println("The lat and long are correct");
    if (LatMinLeftDiff == 0 && LonMinLeftDiff == 0) {
      // Serial.println("The Left sides are the same");
      if (LonMinRightDiff < 500 && LatMinRightDiff < 500) {
        digitalWrite(hot, HIGH);
    
        }
      else{
        digitalWrite(hot,LOW);
        }
  

  
    }

  }





}

void Math(){
 LonMinRightDiff = abs(MylocationLonMinRight - LonMinRight);

 LonMinLeftDiff = abs(MylocationLonMinLeft - LonMinLeft);


 LatMinRightDiff = abs(MylocationLatMinRight -  LatMinRight);

 LatMinLeftDiff = abs(MylocationLatMinLeft - LatMinLeft);
}















