#include <ESP8266WiFi.h>
#include <Servo.h>
 
#define PIN_SERVO D0
 
const char* ssid = "KAACT Class 1-2 2.4G";
const char* password = "kaact4091";

//char host[] = "36350366.ngrok.io";
 
WiFiServer server(80);
Servo myServo;
 
void setup() {
  Serial.begin(115200);
  myServo.attach(PIN_SERVO);
 
  //WiFi.mode(WIFI_STA);
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
int value = 0;

void loop() {
  WiFiClient client = server.available();
  if(!client) return;
 
  Serial.println("새로운 클라이언트");
  client.setTimeout(5000);
 
  String request = client.readStringUntil('\r');
  Serial.println("request: ");
  Serial.println(request);
  
  while(client.available()) {
    client.read();
  }


  if(request.indexOf("/pos=") >= 0) {
    int pos1 = request.indexOf('=');
    int pos2 = request.indexOf('d');
    String servoPos = request.substring(pos1+1, pos2);
 
    myServo.write(servoPos.toInt());
    Serial.println(servoPos); 
  }
}
