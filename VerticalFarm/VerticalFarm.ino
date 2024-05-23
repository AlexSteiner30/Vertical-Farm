#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ESPAsyncWebSrv.h>
#include <WiFiS3.h>

#define DHTPIN 2
#define RELAYPIN_WATER_MOTOR 3
#define RELAYPIN_FAN_MOTOR 4

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

BH1750 lightMeter(0x23);

int idealSoil, idealTemp, idealHum, idealLight;
bool fans = false;

const char* ssid = "Student_SmartDevice";
const char* password = "Sm4rtD3v!c3";

String data;

String serverName = "172.26.16.77";
String cameraIP = "172.26.16.77";

String serverPath = "/";
String ID;

const int serverPort = 4000;

bool firstConnection = true;

WiFiClient client;
HTTPClient http;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  dht.begin();
  Wire.begin();
  lightMeter.begin();

  pinMode(RELAYPIN_WATER_MOTOR, OUTPUT);
  pinMode(RELAYPIN_FAN_MOTOR, OUTPUT);

  digitalWrite(RELAYPIN_WATER_MOTOR, HIGH);
  digitalWrite(RELAYPIN_FAN_MOTOR, HIGH);

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  while (firstConnection) {
    if (client.connect(serverName.c_str(), serverPort)) {
      Serial.println("Connected to server: " + serverName);
      http.begin(client, "http://" + serverName + ":" + serverPort + "/add-farm");

      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"ip\":\"" + (WiFi.localIP()).toString() + "\",\"ssid\":\"" + String(ssid) + "\",\"password\":\"" + String(password) + "\"}");

      Serial.print("Response Code: ");
      Serial.println(httpResponseCode);

      ID = http.getString();
      http.end();

      Serial.println("Connected to server: " + serverName);
      http.begin(client, "http://" + serverName + ":" + serverPort + "/" + ID + "/data");

      http.addHeader("Content-Type", "application/json");
      httpResponseCode = http.POST("{}");

      Serial.print("Response Code: ");
      Serial.println(httpResponseCode);

      Serial.println(http.getString());

      String sensorData = http.getString();
      updateSensorData(sensorData);

      http.end();

      firstConnection = false;

      http.end();
      client.stop();
    } else {
      Serial.println("Connection to " + serverName + " failed.");
    }
  }

  startServer();
}

void loop() {
  sensors();
}

void startServer(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Data sent");
    request->send(200, "application/json", data);  
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest* request) {
    bool updated;
    while (!updated) {
      if (client.connect(serverName.c_str(), serverPort)) {
        Serial.println("Connected to server: " + serverName);
        http.begin(client, "http://" + serverName + ":" + serverPort + "/" + ID + "/data");

        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST("{}");

        Serial.print("Response Code: ");
        Serial.println(httpResponseCode);

        Serial.println(http.getString());

        String sensorData = http.getString();
        updateSensorData(sensorData);

        updated = true;

        http.end();
        client.stop();
      } else {
        Serial.println("Connection to " + serverName + " failed.");
      }
    }

    request->send(200, "text/plain", "Okay");
  });

  server.on("/fans", HTTP_GET, [](AsyncWebServerRequest* request) {
    fans = !fans;
    if(fans){
      digitalWrite(RELAYPIN_WATER_MOTOR, HIGH);
    }else{
      digitalWrite(RELAYPIN_WATER_MOTOR, LOW);
    }
    request->send(200, "text/plain", "Fans Enabled/Disabled");
  });

  server.begin();
}

void sensors(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilH1 = analogRead(A1);
  int soilH2 = analogRead(A2);
  uint16_t lux = lightMeter.readLightLevel();

  data = "{\"airHum\":\"" + String(h) + "\",\"airTemp\":\"" + String(t) + "\",\"soilHum1\":\"" + String(soilH1) + "\",\"soilHum2\":\"" + String(soilH2) + "\",\"light\":\"" + String(lux) + "\"}";

  motorControll(h, t, soilH1 soilH2, lux);

  delay(2000);
}

void motorControll(float h, float t, int soilH1, int soilH2, uint16_t lux) {
  digitalWrite(RELAYPIN_WATER_MOTOR, HIGH);
}

void updateSensorData(String sensorData){
  int commaIndex = sensorData.indexOf(',');

  String soilHumReading = sensorData.substring(sensorData.indexOf(':') + 1, commaIndex);
  sensorData = sensorData.substring(commaIndex + 1);

  int sensor2CommaIndex = sensorData.indexOf(',');
  String airTempReading = sensorData.substring(sensorData.indexOf(':') + 1, sensor2CommaIndex);
  sensorData = sensorData.substring(sensor2CommaIndex);

  int sensor3CommaIndex = sensorData.indexOf(',');
  String airHumReading = sensorData.substring(sensorData.indexOf(':') + 1, sensor3CommaIndex);
  sensorData = sensorData.substring(sensor2CommaIndex);

  String lightReading = sensorData.substring(sensorData.indexOf(':') + 1);

  idealSoil = soilHumReading.toInt();
  idealTemp = airTempReading.toInt();
  idealHum = soilHumReading.toInt();
  idealLight = lightReading.toInt();
}