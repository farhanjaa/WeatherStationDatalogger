// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <Wire.h>
#include "DFRobot_SHT20.h"
#include <BH1750.h>
#include <SPI.h>
#include <SD.h>

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

BH1750 lightMeter;
DFRobot_SHT20 sht20;

File myFile;
const int chipSelect = 53;

unsigned long previousMillis = 0; //--> will store last time was updated
const long interval = 1000; //--> interval (1000 = 1 second)


void setup () {
  Serial.begin(9600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
  Serial.println("Successfully Initializing the RTC DS1307");
  Serial.println();

  Serial.println("Starting to run RTC DS1307...");
  delay(100);

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  Serial.println("Successfully running RTC DS1307");
  delay(100);
  //-------------Memulai Sensornya
    Serial.println("SHT20 Example!");
    sht20.initSHT20();        // Init SHT20 Sensor
    delay(100);
    sht20.checkSHT20(); 
    lightMeter.begin();
     Serial.println(F("BH1750 Test begin"));// Check SHT20 Sensor
    //----------------------------------------------Initializing Micro SD Card
     Serial.println("Initializing Micro SD Card...");
    delay(100);

  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    while (1);
  }
   Serial.println("Successfully Initializing Micro SD Card");
  Serial.println();
  delay(100);

}

void loop () {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    DateTime now = rtc.now();
    float humd = sht20.readHumidity();                  // Read Humidity
    float temp = sht20.readTemperature();               // Read Temperature
    float lux = lightMeter.readLightLevel();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

      //----------------------------------------------Printing Temperature, Humidity and Lux
     Serial.print(" Suhu:");
    Serial.print(temp, 1);
    Serial.print("C");
    Serial.print(" | ");
    Serial.print(" Kelembapan:");
    Serial.print(humd, 1);
    Serial.print("%");
    Serial.println();
    Serial.print(" Light:");
    Serial.print(lux);
    Serial.println("lx");
    delay(1000);

    myFile = SD.open("SensorTA.txt", FILE_WRITE);

    // calculate a date which is 7 days, 12 hours, 30 minutes, and 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));
     if (myFile) {
        Serial.println("Writing to SensorTA.txt...");
        
        //----------------------------------------------Writing Date Data to Micro SD Card
        myFile.print(now.day());
        myFile.print("-");
        myFile.print(now.month());
        myFile.print("-");
        myFile.print(now.year());
        //----------------------------------------------
        
        myFile.print(","); //--> Write the delimiter between the data in the form of a comma character to the Micro SD Card

        //----------------------------------------------Writing Time Data to Micro SD Card
        myFile.print(now.hour());
        myFile.print(":");
        myFile.print(now.minute());
        myFile.print(":");
        myFile.print(now.second());
        //----------------------------------------------
    
        myFile.print(","); //--> Write the delimiter between the data in the form of a comma character to the Micro SD Card
        
        myFile.print(temp, 1); //--> Writing Temperature Data to Micro SD Card
    
        myFile.print(","); //--> Write the delimiter between the data in the form of a comma character to the Micro SD Card
    
        myFile.println(humd, 1); //--> Writing Humidity Data to Micro SD Card
        
        myFile.print(","); //--> Write the delimiter between the data in the form of a comma character to the Micro SD Card
        
        myFile.println(lux, 1); //--> Writing Temperature Data to Micro SD Card
    
          
        myFile.close(); //--> close the file
        
        Serial.println("Successfully writing Data to Micro SD Card");
        Serial.println("----");
        Serial.println();
      } else {      
        Serial.println("Error opening SensorTA.txt"); //--> if the file didn't open, print an error
      }
   

    Serial.println();
    delay(1000);
}
}
