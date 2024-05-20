#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(6, 7);

#define DHTPIN 2
#define RELAYPIN_WATER_MOTOR 3
#define RELAYPIN_FAN_MOTOR 4

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

BH1750 lightMeter(0x23);

void setup() {
  Serial.begin(9600);

  dht.begin();
  Wire.begin();
  lightMeter.begin();

  pinMode(RELAYPIN_WATER_MOTOR, OUTPUT);
  pinMode(RELAYPIN_FAN_MOTOR, OUTPUT);

  digitalWrite(RELAYPIN_WATER_MOTOR, LOW);
  digitalWrite(RELAYPIN_FAN_MOTOR, HIGH);

  espSerial.begin(115200);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilH1 = analogRead(A1);
  int soilH2 = analogRead(A2);
  uint16_t lux = lightMeter.readLightLevel();

  Serial.print("Humidity: " + String(h) + "\n");
  Serial.print("Temperature: " + String(t) + "\n");
  Serial.print("Soil Humidity 1: " + String(soilH1) + "\n");
  Serial.print("Soil Humidity 2: " + String(soilH2) + "\n");
  Serial.print("Light: " + String(lux) + "\n");
  Serial.print("\n");

  digitalWrite(SS, LOW);
  SPI.transfer("Hello from Arduino!"); // Send data
  digitalWrite(SS, HIGH);
  
  delay(2000);
}

void motorControll(float h, float t, int soilH1, int soilH2, uint16_t lux) {
  digitalWrite(RELAYPIN_WATER_MOTOR, HIGH);
}
