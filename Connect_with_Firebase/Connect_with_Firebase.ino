#include <WiFi.h>
#include <FastCRC.h>
#include <FirebaseESP32.h>
#include <DHT.h>

// --- Wi-Fi credentials ---
#define WIFI_SSID "SLT_4GLTE"
#define WIFI_PASSWORD "39749584"

// Firebase project details - Updated to match your database
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

// Define pins
#define LED_PIN 2
#define DHT_PIN 4
#define DHT_TYPE DHT11

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  
  // Initialize the LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Ensure LED is off initially
  
  // Initialize DHT sensor
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  Serial.println("Firebase initialized");
}

void loop() {
  // Read DHT11 sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Check if readings are valid
  if (!isnan(humidity) && !isnan(temperature)) {
    // Update DHT11 data in Firebase
    if (Firebase.setFloat(fbdo, "/dht11/humidity", humidity)) {
      Serial.print("Humidity updated: ");
      Serial.println(humidity);
    } else {
      Serial.print("Failed to update humidity: ");
      Serial.println(fbdo.errorReason());
    }
    
    if (Firebase.setFloat(fbdo, "/dht11/temperature", temperature)) {
      Serial.print("Temperature updated: ");
      Serial.println(temperature);
    } else {
      Serial.print("Failed to update temperature: ");
      Serial.println(fbdo.errorReason());
    }
  } else {
    Serial.println("Failed to read DHT11 sensor");
  }
  
  // Read the LED status from Firebase
  if (Firebase.getString(fbdo, "/led/status")) {
    if (fbdo.dataType() == "string") {
      String ledStatus = fbdo.stringData();
      Serial.print("LED Status from Firebase: ");
      Serial.println(ledStatus);
      
      if (ledStatus == "ON") {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED turned ON");
      } else if (ledStatus == "OFF") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED turned OFF");
      }
    }
  } else {
    Serial.print("Failed to get LED status: ");
    Serial.println(fbdo.errorReason());
  }
  
  // Wait for 2 seconds before the next cycle
  delay(2000);
}
