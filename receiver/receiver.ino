#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "AMD";
const char* password = "   hoda   ";

int flameSensorValue = 0; // Global variable for flame data
int gasSensorValue = 0;   // Global variable for gas data

WebServer server(80);

void handleReceive()
{
  if (server.hasArg("plain"))
  {
    String body = server.arg("plain");
    Serial.println("Received data: " + body);

    // Create a JSON document
    DynamicJsonDocument doc(1024);

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(doc, body);

    // Check for errors
    if (error)
    {
      Serial.println("Failed to parse JSON");
      server.send(400, "text/plain", "Failed to parse JSON");
      return;
    }

    // Extract the values and store them in global variables
    flameSensorValue = doc["flame"]; // Extract flame value
    gasSensorValue = doc["gas"];     // Extract gas value

    // Print extracted values for debugging
    Serial.println("Flame: " + String(flameSensorValue));
    Serial.println("Gas: " + String(gasSensorValue));

    // Respond to the client
    server.send(200, "text/plain", "Data received");
  }
  else
  {
    server.send(400, "text/plain", "No data received");
  }
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.println("Receiver IP Address: " + WiFi.localIP().toString());
  server.on("/receive", HTTP_POST, handleReceive);
  server.begin();
}

void loop() {
  server.handleClient();
}
