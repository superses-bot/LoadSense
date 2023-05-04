#include <M5Core2.h> //including the relevant libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>

#define THRESHOLD_VIBRATION 70 // symbolic constant that corresponds to the threshold vibration determined through testing
#define THRESHOLD_ACCELERATION 0.3 // symbolic constant that corresponds to the threshold acceleration for door position checks determined through testing

// variables to store accelerometer data from 3 dimensions
float accX = 0.0F; 
float accY = 0.0F;
float accZ = 0.0F;

bool available = false; // status of washing machine - true when available
bool open_flag = false; // status of door position - true when open

// constant variables that store details about the WiFi connection
const char* ssid = "Andrew"; 
const char* password = "andrew123";
// constant variables that store details about the server and port
const char* server = "loadsense.hosting.nyu.edu";
const int port = 443;
const char* m5stack_id = "M5Stack_1"; // client ID

RTC_TimeTypeDef currenttime; // variable that is used to store time

WiFiClientSecure client; // variable that is used for client-server interactions


double average_vibration(void){ // calculates average vibration over a period of 5 seconds
  M5.Rtc.GetTime(&currenttime); // gets and stores current time
  int currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds; // converts time to seconds
  int targettimeseconds = currenttimeseconds + 5; // sets target time to be at a point in time 5 seconds away from current time
  double sum = 0;
  while ((targettimeseconds) > (currenttimeseconds)){ // loops for 5 seconds
    M5.IMU.getAccelData(&accX, &accY, &accZ); // gets and stores accelerometer data
    open_check();
    if(open_flag){ // runs if the door has been opened
      return (-1);
    }else{ // runs if the door remains closed
      M5.Rtc.GetTime(&currenttime);
      currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds; // updates currenttimeseconds to the latest time
      accZ = abs(accZ); // ensures all values of accZ are positive
      sum += accZ;
    }
  }
  sum = (sum/5); // calculates average vibration over a 5 second period
  return(sum);
}


bool Is_shaking(float accX, float accY, float accZ){ // checks if average vibration over the 5 second period is above the threshold value
  double avg = average_vibration(); // runs average vibration and stores value obtained in avg
  if (avg > THRESHOLD_VIBRATION){ // this block runs if machine is vibrating sufficiently
    M5.Lcd.setCursor(0,0);
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.printf("  VIBRATING  ");
    M5.Lcd.setCursor(0,40);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("%5.2f", avg);
    return true;
  }else if((avg < THRESHOLD_VIBRATION)&&(avg != -1)){ // this runs if the machine is not vibrating and it is closed
    M5.Lcd.setCursor(0,0);
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.printf("NOT VIBRATING");
    M5.Lcd.setCursor(0,40);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("%5.2f", avg);
    return false;
  }else if (avg == -1){ // this runs if the machine was opened during average vibration calculation
    available = true;
    return false;
  }
}

void open_check(){ // checks if machine door was opened
  M5.IMU.getAccelData(&accX, &accY,&accZ); // gets and stores accelerometer data
  if (accZ < - THRESHOLD_ACCELERATION){ // this runs when sufficiently large negative acceleration is applied 
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(100,180);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("closed");
    open_flag = false; // door was closed
    delay(800); // waits to mitigate unintended door recoil input
  }else if(accZ > THRESHOLD_ACCELERATION){ // this runs when sufficiently large positive acceleration is applied
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(100,180);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf(" open ");
    open_flag = true; // door was opened
    available = true; // machine is available
    delay(800); // waits to mitigate unintended door recoil input
  }
  // note that if no large acceleration was measured, the function does not change the open_flag variable
}

bool Availability_check(void){ // checks if machine is available - classified as available if machine's average vibration does not exceed threshold value for 5 minutes
  M5.Rtc.GetTime(&currenttime); // gets and stores current time
  int currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds; // converts time to seconds
  int targettimeseconds = currenttimeseconds + 300; // sets target time to be at a point in time 5 minutes away from current time
  while ((targettimeseconds) > (currenttimeseconds)){ // loops for 5 minutes
    M5.IMU.getAccelData(&accX, &accY,&accZ); // gets and stores accelerometer data
    open_check();
    if (open_flag){ // runs if door was opened
      return(true); // machine is available
    } // runs if door remained closed
    if (Is_shaking(accX, accY, accZ)){ // runs if machine vibrates significantly
      return(false); // exits loop, tells us the machine is being used, so it is not available
    }
    M5.Rtc.GetTime(&currenttime); // updates currenttime variable to the latest time
    currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds; // converts to seconds
  }
  return(true); // since the average vibration did not exceed the threshold for the entirety of the 5 minutue period, the machine is classified as available
}


void setup() {
  M5.begin(); // M5 initialization
  M5.IMU.Init(); // IMU initialization

  // screen and print format initialization
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);

  // time initialization
  currenttime.Hours = 0;
  currenttime.Minutes = 0;
  currenttime.Seconds = 0;
  M5.Rtc.SetTime(&currenttime);
  
  // disabling SSL/TLS certificate verification
  client.setInsecure();

  // connecting to WiFi using ssid and password
  M5.Lcd.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { // loops until WiFi is connected
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("Connected to Wi-Fi."); // connection successful
  
  M5.Lcd.clear(); //resets screen to blank
}


void loop() {
  open_check();
  if (open_flag){ // runs when door is open
    open_check(); // checks if door is closed
    // note that the only processes done by the M5 Stack when the door is open is to wait for the door to close, since all open washing machines are available
  }else{ // runs when door is closed
    M5.IMU.getAccelData(&accX, &accY, &accZ);  // gets and stores the accelerometer data
    if ((Is_shaking(accX, accY, accZ))&&(open_flag == false)) { // runs when machine is closed and vibrating
      available = false; // machine is not available
    }else{ 
      available = Availability_check(); // checks if machine remains available for 5 minutes
    }
    if (available){
      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.setCursor(0,100);
      M5.Lcd.setTextSize(2);
      M5.Lcd.printf("  AVAILABLE  ");
    }else{
      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.setCursor(0,100);
      M5.Lcd.setTextSize(2);
      M5.Lcd.printf("NOT AVAILABLE");
    }

    if (client.connect(server, port)) { // this runs when client was able to connect to the server
      M5.Lcd.setCursor(0, 220);
      M5.Lcd.setTextColor(GREEN, BLACK);
      M5.Lcd.print("    Connected to server.    ");

      // preparing the GET request
      String request = "GET /receive_message.php?message=";
      request += available ? "true" :"false"; // appends true or false based on the machine's availability
      request += "&id=" + String(m5stack_id); // adds the identifier
      request += " HTTP/1.1\r\n";
      request += "Host: " + String(server) + "\r\n";
      request += "Connection: close\r\n\r\n";

      client.print(request); // Send the GET request
      client.stop(); // Disconnect from server
    }else{ // runs when client was unable to connect to the server
      M5.Lcd.setCursor(0, 220);
      M5.Lcd.setTextColor(YELLOW, BLACK);
      M5.Lcd.print("Connection to server failed.");
    }
  }
}
