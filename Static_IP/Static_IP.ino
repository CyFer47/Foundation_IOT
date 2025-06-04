#include <WiFi.h>

const char* ssid = "SLT_4GLTE";         
const char* password = "39749584"; 

IPAddress local_IP(192, 168, 1, 184);     // ESP32 static IP
IPAddress gateway(192, 168, 0, 1);        // Router gateway
IPAddress subnet(255, 255, 255, 0);       // Subnet mask

void setup() {
  Serial.begin(115200);   
  WiFi.begin(ssid, password);
  WiFi.config(local_IP, gateway, subnet);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  
}