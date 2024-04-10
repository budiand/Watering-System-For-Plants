#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>

const int rs = 2;
const int en = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int secBuffer = 0;

// Data wire is plugged into digital pin 1 on the Arduino
#define ONE_WIRE_BUS 1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
dht DHT;
Servo servo;

void printToFile(String dataString) {
  File dataFile = SD.open("log.txt", FILE_WRITE);
  dataFile.println(dataString);
  dataFile.close();
  Serial.println("Wrote to file");
}
void setup(void) {
  Serial.begin(9600);
  lcd.begin(16,2);
  if (!SD.begin(10)) {
    Serial.println("Card init failed");
  }
  pinMode(8, OUTPUT);
  servo.attach(9, 0 ,180);
}

void loop(void) {
  // Send the command to get temperatures
  DHT.read11(1);
  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(DHT.temperature);
  Serial.print((char)176);//shows degrees character
  Serial.print("C | ");
  Serial.print((DHT.temperature * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);//shows degrees character
  Serial.println("F");
  lcd.setCursor(0,0);
  lcd.print(DHT.temperature);
  if (DHT.temperature < 25) {
    //Play low pitch
    tone(8, 100);
  }
  else if (DHT.temperature > 35) {
    //Play high pitch
    tone(8, 800);
  } else {
    noTone(8);
    //Water for 1 min then wait 1 hour
    if (secBuffer == 0) {
      for (int pos = 0; pos < 180; pos++) {
        servo.write(pos);
        delay(5);
      }
    }
    if (secBuffer == 60) {
      for (int pos = 180; pos >0; pos--) {
        servo.write(pos);
        delay(5);
      }
    }
    secBuffer++;
    if (secBuffer >= 500) { //Replace with 3600 for 1 hour
      secBuffer = 0;
    }
  }
  delay(1000);
}