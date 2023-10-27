#include <SoftwareSerial.h>
#include "dht.h"
#define RX 10
#define TX 11
#define dht_apin A2  // Analog Pin sensor is connected to
#define mq_apin A0   // MQ135 is connected to
dht DHT;
String AP = "Wifi";               // AP NAME
String PASS = "asdfghjkl";        // AP PASSWORD
String API = "PXCGBKOGS1JUIW06";  // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;

SoftwareSerial esp8266(RX, TX);

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop() {
  Serial.println();
  String getData = "GET /update?api_key=" + API + "&field1=" + getHumidityValue() + "&field2=" + getTemperatureValue() + "&field3=" + getAirQuality();
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=1,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=1," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData);
  delay(50);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=1", 5, "OK");
  delay(50);
  delay(1500);
  Serial.println();
}


String getTemperatureValue() {
  DHT.read11(dht_apin);
  int temp = DHT.temperature;
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C ");
  delay(50);
  return String(temp);
}


String getHumidityValue() {
  DHT.read11(dht_apin);
  int humidity = DHT.humidity;
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("% ");
  // delay(50);
  return String(humidity);
}

String getAirQuality() {
  int gasLevel = analogRead(mq_apin);
  Serial.print("Air quality index: ");
  Serial.print(gasLevel);
  if (gasLevel <= 50) {
    Serial.println(" (Great)");
  } else if (gasLevel <= 181) {
    Serial.println(" (Good)");
  } else if (gasLevel <= 225) {
    Serial.println(" (Poor)");
  } else if (gasLevel <= 300) {
    Serial.println(" (Unhealthy)");
  } else if (gasLevel <= 350) {
    Serial.println(" (Very Unhealthy)");
  } else {
    Serial.println(" (Hazardous)");
  }
  // delay(50);
  return String(gasLevel);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");

  esp8266.println(command);  //at+cipsend
  Serial.println(esp8266.readStringUntil('\n'));

  countTimeCommand++;


  // if (found == true) {
  //   Serial.println("OYI");
  //   countTrueCommand++;
  //   countTimeCommand = 0;
  // }

  // if (found == false) {
  //   Serial.println("Fail");
  //   countTrueCommand = 0;
  //   countTimeCommand = 0;
  // }

  // found = false;
}
