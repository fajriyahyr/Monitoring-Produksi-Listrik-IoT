#include <Wire.h> //library I2C untuk sensor BH17250
#include <BH1750.h> //library untuk sensor BH1750
#include "DHT.h"//library untuk sensor DHT
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <AntaresESP8266HTTP.h>

SoftwareSerial node(D6, D5); //rx,tx

#define ACCESSKEY   "1dafd109ff4a4873:bc6e464b8ff0b45f"
#define applicationName "DataPanel"
#define deviceName  "PanelSurya"
#define WIFISSID "SEINFRAM"
#define PASSWORD "buruansae"
AntaresESP8266HTTP antares(ACCESSKEY);

BH1750 lightMeter; 
DHT dht(2,DHT11); //2->pin D4, DHT11->jenis DHT yang dipakai


void setup() {
  Serial.begin(9600);
  node.begin(9600);
  dht.begin();
  Wire.begin();
  lightMeter.begin();
 
  WiFi.begin(WIFISSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(.);
    }
  
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID, PASSWORD);  
  while (!Serial) continue;
  
}

void loop() {
  
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  delay(1000);

  float lux = lightMeter.readLightLevel();
  delay(1000);
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(node);
  
  if(data == JsonObject::invalid()){
    jsonBuffer.clear();
    return;
  }
  
  Serial.print("Terima data arus: ");
  float current = data["arus"]; //terima data dari nilai arus
  Serial.println(current); //print arus
  Serial.print("Terima data tegangan:");
  float voltage = data["tegangan"]; //terima data dari nilai tegangan 
  Serial.println(voltage); //print tegangan
  Serial.println("");

  float c = current;
  float v = voltage;
  float t = temp;
  float h = humid;
  float l = lux; 
  float p = v * c;
  
  antares.add("arus", c);
  antares.add("tegangan", v);
  antares.add("suhu", t);
  antares.add("kelembaban", h);
  antares.add("cahaya", l);
  antares.add("Daya", p);
  antares.send(applicationName,deviceName);
  delay(15000);  
}
