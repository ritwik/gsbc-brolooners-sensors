#include <SPI.h>
#include <SD.h>

// External Libraries. Need to install from the package manager.
// Sketch > Include Library > Manage Libraries ...
#include <Time.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 8;

// Our attached sensors.
const int uvSensorPin = A0;
const int ozoneSensorPin = A1;
const int barometricSensorPin = A2;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // The chipSelect pin you use should also be set to output
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // TODO: Do we need to software tune the analog inputs on startup?
  
}

void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";
  time_t currentTime = millis();
  dataString += String(currentTime);
  dataString += ",";
  dataString += String(analogRead(uvSensorPin));
  dataString += ",";
  dataString += String(analogRead(ozoneSensorPin));
  dataString += ",";
  dataString += String(analogRead(barometricSensorPin));


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  
}
