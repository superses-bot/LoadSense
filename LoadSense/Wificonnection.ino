#include <WiFi.h>
#include <WebSocketsClient.h>
#include <M5Core2.h>

// WiFi settings
const char* ssid = "Andrewphone";
const char* password = "Andrewpassword";

// WebSocket server settings
const char* websocketServer = "wss://ws.postman-echo.com/raw";
const int websocketPort = 80;

WebSocketsClient webSocket;

bool Availability_check = false;

void setup() {
  M5.begin(true);
  M5.Lcd.setBrightness(200);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  
  M5.Lcd.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nWiFi connected");

  M5.Lcd.print("Connecting to WebSocket server...");
  webSocket.begin(websocketServer, websocketPort);
  webSocket.onEvent(webSocketEvent);
  while (!webSocket.connected()) {
    webSocket.loop();
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nWebSocket connected");
}

void loop() {
  webSocket.loop();
  M5.update();

  if (Availability_check) {
    sendSignalToServer();
    Availability_check = false; // Reset the boolean variable
  }

}

void sendSignalToServer() {
  String message = "{\"machinenumber\": \"availability\",}";
  webSocket.sendTXT(message);
} 
// Written with help from https://iotdesignpro.com/projects/websocket-server-with-esp32-and-arduino-ide