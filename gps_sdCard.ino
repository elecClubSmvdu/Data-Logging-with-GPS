// OM NAMAH SHIVAAY
#include<SD.h> // include sd card library
#include<SPI.h> // load SPI library

#include<Adafruit_GPS.h>  // install adafruit gps library
#include<SoftwareSerial.h>  // load the serial software library
SoftwareSerial mySerial(3,2);
Adafruit_GPS GPS(&mySerial);  // create a gps object

String NMEA1;  // string for first sentence
String NMEA2;  // string for 2nd sentence
char c;

int chipSelect =4;  // pin select for sd card reader
File mySensorData;  // data object to store your info

void setup()
{
  Serial.begin(115200);  // turn on serial monitor
  GPS.begin(9600);  // turn on gps at 9600 baud rate
  GPS.sendCommand("$PGCMD,33,0*6D");  // remove unnecessary updates  of antenna
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Request RMC and GGA Sentences only
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //Set update rate to 1 hz
  delay(1000); 
  pinMode(10,OUTPUT);  // FOR THE SD CARD
  SD.begin(chipSelect);  // initialise sd card
  if (SD.exists("NMEA.txt")) 
  { //Delete old data files to start fresh
    SD.remove("NMEA.txt");
  }
  if (SD.exists("GPSData.txt"))
  { //Delete old data files to start fresh
    SD.remove("GPSData.txt");
  }
 
}

void loop()
{
  readGPS();
   if(GPS.fix==1) { //Only save data if we have a fix
  mySensorData = SD.open("NMEA.txt", FILE_WRITE); //Open file on SD card for writing
  mySensorData.println(NMEA1); //Write first NMEA to SD card
  mySensorData.println(NMEA2); //Write Second NMEA to SD card
  mySensorData.close();  //Close the file
  mySensorData = SD.open("GPSData.txt", FILE_WRITE);
  mySensorData.print(GPS.latitude,4); //Write measured latitude to file
  mySensorData.print(GPS.lat); //Which hemisphere N or S
  mySensorData.print(",");
  mySensorData.print(GPS.longitude,4); //Write measured longitude to file
  mySensorData.print(GPS.lon); //Which Hemisphere E or W
  mySensorData.print(",");
  mySensorData.println(GPS.altitude);
  mySensorData.close();
  }
  
}
void readGPS() {
  
  clearGPS();
  while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
  NMEA1=GPS.lastNMEA();
  
   while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
  NMEA2=GPS.lastNMEA();
  
  Serial.println(NMEA1);
  Serial.println(NMEA2);
  Serial.println("");
  
}
void clearGPS() {  //Clear old and corrupt data from serial port 
  while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
  
  while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
   while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
  
}


