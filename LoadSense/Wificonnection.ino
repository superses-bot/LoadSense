#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <M5Core2.h>

// WiFi settings
const char* ssid = "Andrew";
const char* password = "andrew123";

// WebSocket server settings
const char* websockets_server = "wss://ws.postman-echo.com/raw:8080";

using namespace websockets;

bool Availability_check = false;

WebsocketsClient client;

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}


void sendSignalToServer() {
  String message = "{\"machinenumber\": \"availability\",}";
  client.send(message);
}

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
  delay(5000);
  M5.Lcd.clear();
  client.onEvent(onEventsCallback);
  client.connect(websockets_server);
}

void loop() {
  client.poll();
  M5.update();
  if (Availability_check) {
    sendSignalToServer();
    Availability_check = false;  // Reset the boolean variable
  }
}





// Written with help from https://iotdesignpro.com/projects/websocket-server-with-esp32-and-arduino-ide