#include "DHT.h"
#include <WiFiS3.h>
#include <ArduinoHttpClient.h>

#define DHTPIN 2
#define RELAYPIN_WATER_MOTOR 3
#define RELAYPIN_FAN_MOTOR 4
#define RELAYPIN_LIGHT 5

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int status = WL_IDLE_STATUS;

int idealSoil = 40;
int idealTemp = 25;
int idealHum = 50;
int idealLight = 100;

bool fans = true;
bool motor = true;

const char* ssid = "Student_SmartDevice";
const char* pass = "Sm4rtD3v!c3";

String serverName = "172.26.16.74";
String cameraIP = "172.26.16.78";

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

  pinMode(RELAYPIN_WATER_MOTOR, OUTPUT);
  pinMode(RELAYPIN_FAN_MOTOR, OUTPUT);
  pinMode(RELAYPIN_LIGHT, OUTPUT);

  digitalWrite(RELAYPIN_WATER_MOTOR, LOW);
  digitalWrite(RELAYPIN_FAN_MOTOR, LOW);
  digitalWrite(RELAYPIN_LIGHT, HIGH);

  delay(20000);

  wifiConnection();
  startServer();
}

void loop() {
   
}

void wifiConnection() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("WiFi connected");

  Serial.print("Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  byte mac[6]; 

  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);

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
    sensors();

    WiFiClient client = server.available();
    if (client) {
      String currentLine = "";
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              float h = dht.readHumidity();
              float t = dht.readTemperature();
              int soilH1 = int(analogRead(A1) / 10);
              int soilH2 = int(analogRead(A2) / 10);
              unsigned int lux = analogRead(A0);

              String data = "{\"airHum\":\"" + String(h) + "\",\"airTemp\":\"" + String(t) + "\",\"soilHum1\":\"" + String(soilH1) + "\",\"soilHum2\":\"" + String(soilH2) + "\",\"light\":\"" + String(lux) + "\"}";

              client.print(data);
              client.println();
              break;
            } else {
              currentLine = "";
            }
          } else if (c != '\r') {
            currentLine += c;
          }

          if (currentLine == "GET /update") {
            Serial.println("Updating");
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
          } else if (currentLine == "GET /fans") {
            Serial.print("Turning Fans ");
            fans = !fans;
            Serial.println(fans);
            if(fans == 0){ digitalWrite(RELAYPIN_FAN_MOTOR, LOW); }
            else{ digitalWrite(RELAYPIN_FAN_MOTOR, HIGH); }
          } else if (currentLine == "GET /motor") {
            Serial.print("Turning Motor ");
            motor = !motor;
            Serial.println(motor);
            if(motor == 0){ digitalWrite(RELAYPIN_WATER_MOTOR, LOW); }
            else{ digitalWrite(RELAYPIN_WATER_MOTOR, HIGH); }
          }
        }
      }
      client.stop();
    }
  }
}

void sensors() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilH1 = int(analogRead(A1) / 10);
  int soilH2 = int(analogRead(A2) / 10);
  unsigned int lux = analogRead(A0);

  motorControll(h, t, soilH1, soilH2, lux);
}

void motorControll(float h, float t, int soilH1, int soilH2, uint16_t lux) {
  if ((soilH1 > idealSoil + 15 || soilH2 > idealSoil + 15) && motor) {
    digitalWrite(RELAYPIN_WATER_MOTOR, HIGH);
  }else{
    motor = false;
    digitalWrite(RELAYPIN_WATER_MOTOR, LOW);
  }
  if ((t > idealTemp + 5 || h > idealHum + 10) && fans) {
    digitalWrite(RELAYPIN_FAN_MOTOR, HIGH);
  } else{
    fans = false;
    digitalWrite(RELAYPIN_FAN_MOTOR, LOW);
  }
  if (lux > idealLight + 50) {
    digitalWrite(RELAYPIN_LIGHT, LOW);
  } else if (lux < idealLight - 50) {
    digitalWrite(RELAYPIN_LIGHT, HIGH);
  }
}

void updateSensorData(String sensorData) {
  int commaIndex = sensorData.indexOf(',');
  String soilHumReading = sensorData.substring(sensorData.indexOf(':') + 1, commaIndex);
  sensorData = sensorData.substring(commaIndex + 1);

  commaIndex = sensorData.indexOf(',');
  String airTempReading = sensorData.substring(sensorData.indexOf(':') + 1, commaIndex);
  sensorData = sensorData.substring(commaIndex + 1);

  commaIndex = sensorData.indexOf(',');
  String airHumReading = sensorData.substring(sensorData.indexOf(':') + 1, commaIndex);
  sensorData = sensorData.substring(commaIndex + 1);

  commaIndex = sensorData.indexOf(',');
  String lightReading = sensorData.substring(sensorData.indexOf(':') + 1, commaIndex);
  sensorData = sensorData.substring(commaIndex + 1);

  idealSoil = soilHumReading.toInt();
  idealTemp = airTempReading.toInt();
  idealHum = airHumReading.toInt();
  idealLight = lightReading.toInt();
}