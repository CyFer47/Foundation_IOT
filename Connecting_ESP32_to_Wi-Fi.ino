#include <WiFi.h>

const char* ssid = "SLT_4GLTE";         
const char* password = "39749584"; 

void setup() {
  Serial.begin(115200);   
  WiFi.begin(ssid, password);
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
