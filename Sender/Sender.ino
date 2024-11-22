#ifdef ESP8266 //nodeMCU
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
WiFiClient client; // Required for ESP8266
#else         //ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#endif

const char* ssid     = "AMD";
const char* password = "   hoda   ";
const char* serverIP = "192.168.1.8"; // Replace with Receiver ESP32's IP
const int serverPort = 80;

#define FLAME_SENSOR_PIN  34
#define GAS_SENSOR_PIN    35

void setup()
{
  Serial.begin(115200);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.println("ESP IP Address: " + WiFi.localIP().toString());
}

void loop()
{
  int flameValue  = 34;//analogRead(FLAME_SENSOR_PIN);
  int gasValue    = 897;//analogRead(GAS_SENSOR_PIN);

  if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi connection
    HTTPClient http;

    String serverPath = String("http://") + serverIP + ":" + serverPort + "/receive";
#ifdef ESP8266
    http.begin(client, serverPath); // Use client for ESP8266
#else
    http.begin(serverPath);
#endif

    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"flame\":" + String(flameValue) + ", \"gas\":" + String(gasValue) + "}";
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.println("Data sent: " + jsonPayload);
    } else {
      Serial.println("Error sending data");
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected");
  }

  delay(2000); // Send data every 2 seconds
}
