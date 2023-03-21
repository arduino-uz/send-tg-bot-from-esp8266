#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";

const char* serverName = "actv.ga";
const int httpsPort = 443;

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setInsecure();
}

void loop() {
  if (client.connect(serverName, httpsPort)) {
    Serial.println("Connected to API");

    // Create JSON payload
    DynamicJsonDocument jsonDoc(1024);
    jsonDoc["message"] = "This message from ESP8266";
    jsonDoc["chat_id"] = 1685405430;
    String payload;
    serializeJson(jsonDoc, payload);

    // Make HTTPS request
    client.println("POST /api HTTP/1.1");
    client.println("Host: actv.ga");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.println(payload);

    delay(5000);

    // Read response
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println(response);
    }
    client.stop();
  } else {
    Serial.println("Connection to API failed");
  }
}
