#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include <WiFiS3.h>
#include <ArduinoHttpClient.h>

#define DHTPIN 2
#define RELAYPIN_WATER_MOTOR 3
#define RELAYPIN_FAN_MOTOR 4
#define RELAYPIN_LIGHT 5

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

BH1750 lightMeter(0x23);

int status = WL_IDLE_STATUS;

int idealSoil = 700;
int idealTemp = 25;
int idealHum = 75;
int idealLight = 600;

bool fans = false;

const char* ssid = "James ";
const char* pass = "james007";

String data;

String serverName = "172.20.10.3";
String cameraIP = "172.20.10.3";

String serverPath = "/";
String ID;

const int serverPort = 4000;

bool firstConnection = true;

WiFiServer server(80);
WiFiClient wifiClient;
HttpClient httpClient = HttpClient(wifiClient, serverName, serverPort);

void setup() {
  Serial.begin(115200);
  Serial.println();

  dht.begin();
  Wire.begin();
  lightMeter.begin();

  pinMode(RELAYPIN_WATER_MOTOR, OUTPUT);
  pinMode(RELAYPIN_FAN_MOTOR, OUTPUT);
  pinMode(RELAYPIN_LIGHT, OUTPUT);

  digitalWrite(RELAYPIN_WATER_MOTOR, HIGH);
  digitalWrite(RELAYPIN_FAN_MOTOR, HIGH);
  digitalWrite(RELAYPIN_LIGHT, HIGH);

  delay(20000);

  wifiConnection();
  startServer();
}

void loop() {
  sensors();
}

void wifiConnection(){
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  while (firstConnection) {
    if (wifiClient.connect(serverName.c_str(), serverPort)) {
      String addFarmUrl = "/add-farm";
      Serial.println("Connecting to server: " + String(serverName) + addFarmUrl);

      httpClient.beginRequest();
      httpClient.post(addFarmUrl);
      httpClient.sendHeader("Content-Type", "application/json");
      String postData = "{\"ip\":\"" + WiFi.localIP().toString() + "\",\"ssid\":\"" + String(ssid) + "\",\"password\":\"" + String(pass) + "\",\"camera_ip\":\"" + cameraIP + "\"}";
      httpClient.sendHeader("Content-Length", postData.length());
      httpClient.beginBody();
      httpClient.print(postData);
      httpClient.endRequest();
      
      int httpResponseCode = httpClient.responseStatusCode();
      Serial.print("Response Code: ");
      Serial.println(httpResponseCode);

      ID = httpClient.responseBody();

      String dataUrl = "/" + ID + "/data";
      Serial.println("Connecting to server: " + String(serverName) + dataUrl);

      httpClient.beginRequest();
      httpClient.post(dataUrl);
      httpClient.sendHeader("Content-Type", "application/json");
      httpClient.sendHeader("Content-Length", 2);
      httpClient.beginBody();
      httpClient.print("{}");
      httpClient.endRequest();

      httpResponseCode = httpClient.responseStatusCode();
      Serial.print("Response Code: ");
      Serial.println(httpResponseCode);


      String sensorData = httpClient.responseBody();
      updateSensorData(sensorData);

      firstConnection = false;
      wifiClient.stop();
    } else {
      Serial.println("Connection to " + serverName + " failed.");
    }
  }
}

void startServer() {
  server.begin();
  Serial.println("Server started");

  while (true) {
    WiFiClient client = server.available();
    if (client) {
      Serial.println("New Client.");
      String currentLine = "";
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          Serial.write(c);
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              if (client.readString().indexOf("GET / ") >= 0) {
                Serial.println("Data sent");
                client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
                client.print(data);
              } else if (currentLine.indexOf("GET /update ") >= 0) {
                bool updated = false;
                while (!updated) {
                  if (wifiClient.connect(serverName.c_str(), serverPort)) {
                    String dataUrl = "/" + ID + "/data";
                    Serial.println("Connecting to server: " + String(serverName) + dataUrl);

                    httpClient.beginRequest();
                    httpClient.post(dataUrl);
                    httpClient.sendHeader("Content-Type", "application/json");
                    httpClient.sendHeader("Content-Length", 2);
                    httpClient.beginBody();
                    httpClient.print("{}");
                    httpClient.endRequest();

                    int httpResponseCode = httpClient.responseStatusCode();
                    Serial.print("Response Code: ");
                    Serial.println(httpResponseCode);

                    String sensorData = httpClient.responseBody();
                    updateSensorData(sensorData);

                    updated = true;
                    wifiClient.stop();
                  } else {
                    Serial.println("Connection to " + serverName + " failed.");
                  }
                }

                client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nOkay");
              } else if (currentLine.indexOf("GET /fans ") >= 0) {
                fans = !fans;
                digitalWrite(RELAYPIN_WATER_MOTOR, fans ? HIGH : LOW);
                client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nFans Enabled/Disabled");
              }
              break;
            } else {
              currentLine = "";
            }
          } else if (c != '\r') {
            currentLine += c;
          }
        }
      }
      client.stop();
      Serial.println("Client Disconnected.");
    }
  }
}

void sensors(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilH1 = analogRead(A1);
  int soilH2 = analogRead(A2);
  uint16_t lux = lightMeter.readLightLevel();

  data = "{\"airHum\":\"" + String(h) + "\",\"airTemp\":\"" + String(t) + "\",\"soilHum1\":\"" + String(soilH1) + "\",\"soilHum2\":\"" + String(soilH2) + "\",\"light\":\"" + String(lux) + "\"}";
  Serial.println(data);

  motorControll(h, t, soilH1, soilH2, lux);

  delay(2000);
}

void motorControll(float h, float t, int soilH1, int soilH2, uint16_t lux) {
  if(soilH1 > idealSoil + 250 || soilH2 > idealSoil + 250){
    digitalWrite(RELAYPIN_WATER_MOTOR, HIGH);
    delay(10000);
    digitalWrite(RELAYPIN_WATER_MOTOR, LOW);
  }
  if(t > idealTemp + 7 || h > idealHum + 20 || h < idealHum - 20){
    digitalWrite(RELAYPIN_FAN_MOTOR, HIGH);
  }
  else{
    digitalWrite(RELAYPIN_WATER_MOTOR, LOW);
  }
  if(lux > idealLight + 1000){
    // turns lights off
  }
  else if(lux < idealLight - 1000){
    // turns lights on
  }
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