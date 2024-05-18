#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>

#define DHTPIN 2     
#define RELAYPIN 3     
 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
 
BH1750 lightMeter;


void setup() 
{
  Serial.begin(9600);

  dht.begin();
  Wire.begin();
  lightMeter.begin();

  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
}

void loop() { 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilH1 = analogRead(A0);
  int soilH2 = analogRead(A1);
  uint16_t lux = lightMeter.readLightLevel();

  Serial.print("Humidity: " + String(h) + "\n");
  Serial.print("Temperature: " + String(t) + "\n");
  Serial.print("Soil Humidity 1: " + String(soilH1) + "\n");
  Serial.print("Soil Humidity 2: " + String(soilH2) + "\n");
  Serial.print("Light: " + String(lux) + "\n");
  Serial.print("\n");

  delay(2000);
}

void motorControll(h, t, soilH1, soilH2, lux){
  digitalWrite(RELAYPIN, HIGH);
}