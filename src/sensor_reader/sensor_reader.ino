#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// External Libraries. Need to install from the package manager.
// Sketch > Include Library > Manage Libraries ...
#include <Time.h>
// Compiled from https://github.com/adafruit/Adafruit_MPL3115A2_Library
#include <Adafruit_MPL3115A2.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 8;

// Our attached sensors.
const int uvSensorPin = A0;
const int ozoneSensorPin = A1;

// Power by connecting Vin to 3-5V, GND to GND
// Uses I2C - connect SCL to the SCL pin, SDA to SDA pin
// See the Wire tutorial for pinouts for each Arduino
// http://arduino.cc/en/reference/wire
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

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
  
  float pressurekPa = 0.0;
  float altm = -100.0;
  float tempC = -100.0;
  
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
  } else {
    float pascals = baro.getPressure();
    // We want to convert to kPa for storage space.
    pressurekPa = pascals / 1000;
    altm = baro.getAltitude();
    tempC = baro.getTemperature();
  }
  
  // make a string for assembling the data to log:
  String dataString = "";
  time_t currentTime = millis();
  dataString += String(currentTime);
  dataString += ",";
  dataString += String(analogRead(uvSensorPin));
  dataString += ",";
  dataString += String(analogRead(ozoneSensorPin));
  dataString += ",";
  dataString += String(pressurekPa);
  dataString += ",";
  dataString += String(altm);
  dataString += ",";
  dataString += String(tempC);


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
  
  // Slow down the loop.
  delay(250);
  
}
