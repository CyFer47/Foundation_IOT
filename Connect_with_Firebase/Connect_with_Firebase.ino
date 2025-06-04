#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "DHT.h"

// --- Wi-Fi credentials ---
#define WIFI_SSID "Your_SSID"
#define WIFI_PASSWORD "Your_PASSWORD"

// --- Firebase project credentials ---
#define API_KEY "API_KEY"
#define DATABASE_URL "DATABASE_URL"

// --- DHT sensor config ---
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- LED pin ---
#define LED_PIN 2

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastMillis = 0;

void setup() {
  //ESP32-Config
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  //WiFi-Config
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Connected!");

  // Firebase-Config
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Anonymous sign-in
  auth.user.email = "";
  auth.user.password = "";

  //DHT 11 - Config
  dht.begin();
}

void loop() {
  // Update every 5 seconds
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();

    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temp) && !isnan(humidity)) {
      Serial.printf("Temp: %.2f °C, Humidity: %.2f %%\n", temp, humidity);

      Firebase.RTDB.setFloat(&fbdo, "dht11/temperature", temp);
      Firebase.RTDB.setFloat(&fbdo, "dht11/humidity", humidity);
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }

    // Read LED status from Firebase
    if (Firebase.RTDB.getString(&fbdo, "led/status")) {
      String ledStatus = fbdo.stringData();
      Serial.print("LED status from Firebase: ");
      Serial.println(ledStatus);

      if (ledStatus == "ON") {
        digitalWrite(LED_PIN, HIGH);
      } else if (ledStatus == "OFF") {
        digitalWrite(LED_PIN, LOW);
      }
    } else {
      Serial.print("Failed to read LED status: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
