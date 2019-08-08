#include <ESP8266WiFi.h>
#include <Servo.h>
#include <String.h>
#include <ArduinoJson.h>
#include "DHT.h"
 
#define PIN_SERVO D1
#define DHTPIN D2
#define DHTTYPE DHT11   // DHT 11
 
const char* ssid = "KAACT Class 1-2 2.4G";
const char* password = "kaact4091";
 
WiFiServer server(80);
Servo myServo;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
 Serial.begin(115200);
  myServo.attach(PIN_SERVO);
  pinMode(D0, OUTPUT);
  
  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  server.begin();
  Serial.println("Server started");

  
}
 
void loop() {
  /*
   * Serial.print("connecting to ") ;
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client = server.available();
  String request = client.readStringUntil('\r');
  
  const int httpPort = 80;
  
  if (!client.connect(host,httpPort)) {
    Serial.println("connection failed");
    client.println("Connected!");
    return;
  }
  */

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(request);
  
  client.setTimeout(1000);

 char index = request[request.indexOf('/') + 1];
 Serial.println(index);
 
 switch(index){
  case '0':
    myServo.write(180);
    delay(1000);
    myServo.write(0);
    break;
  case '1':
    digitalWrite(D0, 0);
    delay(1000);
    digitalWrite(D0, 1);
    break;
  case '2':
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();
    
    client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n"));
    client.print(temp);
    client.print("/");
    client.print(hum);
    
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    break;
  } 
 }
}
