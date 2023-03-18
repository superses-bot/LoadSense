#include <WiFi.h>
#include <WebSocketsClient.h>
#include <M5Core2.h>

// WiFi settings
const char* ssid = "Andrewphone";
const char* password = "Andrewpassword";

// WebSocket server settings
const char* websocketServer = "loadsense.com";
const int websocketPort = 12345;

WebSocketsClient webSocket;

// Your boolean variable
bool sendSignal = true;

void setup() {
  M5.begin(true, true, true);
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

  if (sendSignal) {
    sendSignalToServer();
    sendSignal = false; // Reset the boolean variable
  }

}



void sendSignalToServer() {
  String message = "{\"type\": \"signal\", \"value\": \"your_signal_value_here\"}";
  webSocket.sendTXT(message);
} 
/* Written with help from https://iotdesignpro.com/projects/websocket-server-with-esp32-and-arduino-ide */