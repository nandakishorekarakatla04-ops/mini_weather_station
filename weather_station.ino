#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

// ---------------- PIN DEFINITIONS ----------------
#define DHTPIN 2
#define DHTTYPE DHT22

#define RAIN_PIN A0
#define SD_CS 10

// ---------------- OBJECTS ----------------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- VARIABLES ----------------
float temperature;
float humidity;
float pressure;
int rainValue;

void setup() {

  Serial.begin(9600);

  // Start DHT22
  dht.begin();

  // Start I2C
  Wire.begin();

  // Start LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Weather Station");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);

  // Start BMP280
  if (!bmp.begin(0x76)) {

    // Try alternative BMP280 address
    if (!bmp.begin(0x77)) {

      Serial.println("BMP280 not found!");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BMP280 ERROR");
      delay(2000);
    }
  }

  // Start SD card
  if (!SD.begin(SD_CS)) {

    Serial.println("SD Card Failed!");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD Card Error");
    delay(2000);

  } else {

    Serial.println("SD Card OK");

    // Create file if it doesn't exist
    if (!SD.exists("weather.csv")) {

      File dataFile = SD.open("weather.csv", FILE_WRITE);

      if (dataFile) {
        dataFile.println("Temperature,Humidity,Pressure,Rain");
        dataFile.close();
      }
    }
  }

  lcd.clear();
}

void loop() {

  // ---------------- READ DHT22 ----------------

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Check DHT22
  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("DHT22 Error");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT22 ERROR");

    delay(2000);
    return;
  }

  // ---------------- READ BMP280 ----------------

  pressure = bmp.readPressure() / 100.0;

  // ---------------- READ RAIN SENSOR ----------------

  rainValue = analogRead(RAIN_PIN);

  // ---------------- SERIAL MONITOR ----------------

  Serial.println("------------------------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Rain Sensor: ");
  Serial.println(rainValue);

  // ---------------- LCD DISPLAY ----------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C ");

  lcd.print("H:");
  lcd.print(humidity, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(pressure, 0);
  lcd.print("hPa");

  delay(3000);

  // ---------------- SECOND LCD SCREEN ----------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Rain:");
  lcd.print(rainValue);

  lcd.setCursor(0, 1);

  // Adjust this threshold after testing
  if (rainValue < 500) {
    lcd.print("RAIN DETECTED");
  } else {
    lcd.print("NO RAIN");
  }

  // ---------------- SAVE TO SD CARD ----------------

  File dataFile = SD.open("weather.csv", FILE_WRITE);

  if (dataFile) {

    dataFile.print(temperature);
    dataFile.print(",");

    dataFile.print(humidity);
    dataFile.print(",");

    dataFile.print(pressure);
    dataFile.print(",");

    dataFile.println(rainValue);

    dataFile.close();

    Serial.println("Data saved to SD card");

  } else {

    Serial.println("Error opening weather.csv");
  }

  delay(3000);
}