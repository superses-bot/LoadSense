#include <M5Core2.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "Phonemanjee";
const char* password = "hatim1234";
const char* server = "loadsense.hosting.nyu.edu";
const int port = 443;
const char* m5stack_id = "M5Stack_1"; 

void setup() {
  M5.begin();
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("Connecting to Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }

  M5.Lcd.clear();
  M5.Lcd.println("Connected to Wi-Fi.");
}

void loop() {
  // Replace this with the appropriate condition for sending the message
  bool messageToSend = true;

  WiFiClientSecure client;

  // Disable SSL/TLS certificate verification
  client.setInsecure();

  if (client.connect(server, port)) {
    M5.Lcd.println("Connected to server.");

    // Prepare the GET request
    String request = "GET /receive_message.php?message=";
    request += messageToSend ? "true" : "false";
    request += "&id=" + String(m5stack_id); // Add the identifier to the request
    request += " HTTP/1.1\r\n";
    request += "Host: " + String(server) + "\r\n";
    request += "Connection: close\r\n\r\n";

    // Send the GET request
    client.print(request);

    // Wait for the response and print it to the M5Stack display
    while (client.connected()) {
      String response = client.readStringUntil('\n');
      M5.Lcd.println(response);
    }

    client.stop();
    M5.Lcd.println("Disconnected from server.");
  } else {
    M5.Lcd.println("Connection to server failed.");
  }

  // Wait some time before sending the next message
  // Adjust this delay as needed
  delay(10000);
}
