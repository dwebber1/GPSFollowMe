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
int on = 13;
int hot = 8;


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

const int threshold = 50;


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
  pinMode(on, OUTPUT);
  pinMode(record, INPUT);


}
void loop() {

  if (gpsSerial.available() >= 6 && gpsSerial.read() == '$' && gpsSerial.read() == 'G' && gpsSerial.read() == 'P' && gpsSerial.read() == 'G' && gpsSerial.read() == 'G' && gpsSerial.read() == 'A') {
    String gpsString = "";
    char c = 0;

    do {
      //Prints the data from the GPS onto the SD card and a Serial Monitor.
      if (gpsSerial.available()) {
        digitalWrite(on, HIGH);
        c = gpsSerial.read();
        gpsString.concat(String(c));

        LatDeg = gpsString.substring(12, 14).toInt();
        LatMinLeft = gpsString.substring(14, 16).toInt();
        LatMinRight = gpsString.substring(17, 21).toInt();

        LonDeg = gpsString.substring(24, 27).toInt();
        LonMinLeft = gpsString.substring(27, 29).toInt();
        LonMinRight = gpsString.substring(30, 35).toInt();
        
        Math();



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



    Serial.print("The Lat Min Right Diff is: ");
    Serial.println(LatMinRightDiff);
    Serial.print("The Long Min Right Diff is: ");
    Serial.println(LonMinRightDiff);



  }











  if (digitalRead(record)) {

    MylocationLat = LatDeg;
    MylocationLong = LonDeg;
    
    MylocationLonMinRight = LonMinRight;
    MylocationLonMinLeft = LonMinLeft;


    MylocationLatMinRight = LatMinRight ;
    MylocationLatMinLeft = LatMinLeft;




    lcd.clear();
    delay(500);
    lcd.print("Recorded");
    lcd.setCursor(0, 1);
    lcd.print("Location !");
    delay(2000);
  }

  else {
    printLocationCheck();






  }












}

void Math() {
  LonMinRightDiff = abs(MylocationLonMinRight - LonMinRight);

  LonMinLeftDiff = abs(MylocationLonMinLeft - LonMinLeft);


  LatMinRightDiff = abs(MylocationLatMinRight -  LatMinRight);

  LatMinLeftDiff = abs(MylocationLatMinLeft - LatMinLeft);
}

void printLocationCheck() {



  if (MylocationLat == LatDeg && MylocationLong == LonDeg) {
    //Serial.println("The lat and long are correct");
    if (LatMinLeftDiff == 0 && LonMinLeftDiff == 0) {
      // Serial.println("The Left sides are the same");
      if (LonMinRightDiff < threshold && LatMinRightDiff < threshold) {
        lcd.setCursor(0, 0);
        digitalWrite(hot, HIGH);
        lcd.clear();


        lcd.print("You have found");
        lcd.setCursor(0, 1);
        lcd.print("the location !");
        delay(200);



      }

      else {
        digitalWrite(hot, LOW);
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

      }



    }

  }
}















