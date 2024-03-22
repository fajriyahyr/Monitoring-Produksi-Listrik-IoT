#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial node(5, 6); // rx,tx

//acs712 sensor arus
float sensitivity = 0.100;
float current = 0.0;

//voltage sensor tegangan
#define ANALOG_IN_PIN A1 //pin A1 sebagai pin input tegangan
float AdcVoltage = 0.0; 
float InputVoltage = 0.0; 
float R1 = 30000.0; //nilai resistor sensor 
float R2 = 7500.0;  //nilai resistor sensor 
float ref_voltage = 5.0; //nilai sumber tegangan sebesar 5v
int adc_value = 0;

void setup() {
  Serial.begin(9600);
  node.begin(9600);
  delay(1000);
}

void loop() { 

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  current_read();
  voltage_read();

  //kirim data ke nodemcu
  data["arus"] = current;
  data["tegangan"] = InputVoltage;
  data.printTo(node);
  
  jsonBuffer.clear();

  delay(2000);
}

void current_read() { //pembacaan nilai arus dari panel
  unsigned int x = 0;
  float AdcAcs = 0.0, Sampel= 0.0, AcsAverage = 0.0;

  for (int x = 0; x < 200; x++) {
    AdcAcs = analogRead(A0);
    Sampel = Sampel + AdcAcs;
    delay (5);
  }
  AcsAverage = Sampel/200.0;

  float voltageAcs = AcsAverage * 5.0 / 1024.0;
  current = (voltageAcs - 2.50) / sensitivity;
  if(current < 0){
    current = 0;
  }
  Serial.print("Current : ");
  Serial.println(current);
  delay(300);
}

void voltage_read() { //pembacaan nilai tegangan panel
  adc_value = analogRead(ANALOG_IN_PIN);
  AdcVoltage  = (adc_value * ref_voltage) / 1024.0;
  InputVoltage = AdcVoltage / (R2 / (R1 + R2)) ;
  Serial.print("Input Voltage = ");
  Serial.print(InputVoltage);
  Serial.print(Volt);
  Serial.println();
  delay(1000);
}
