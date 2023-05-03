#include <M5Core2.h>
#define THRESHOLD_VIBRATION 90
#define THRESHOLD_ACCELERATION 0.5


float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

bool available = false;
bool open_flag = false;

RTC_TimeTypeDef targettime;
RTC_TimeTypeDef currenttime;


double average_vibration(void){ // calculates average vibration over a period of 5 seconds
  M5.Rtc.GetTime(&currenttime); // gets and stores current time
  int currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds;
  int targettimeseconds = currenttimeseconds + 5; // sets target time to be at a point in time 5 seconds away from current time
  double sum = 0;
  while ((targettimeseconds) > (currenttimeseconds)){ // loops for 5 seconds
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    open_check();
    if(open_flag){
      return (-1);
    }else{M5.Rtc.GetTime(&currenttime);
      currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds;
      accZ = abs(accZ);
      sum += accZ;
    }
  }
  sum = (sum/5);
  return(sum);
}


bool Is_shaking(float accX, float accY, float accZ){ // checks if average vibration over the 5 second period is above the threshold value
  double avg = average_vibration();
  if (avg > THRESHOLD_VIBRATION){
    M5.Lcd.setCursor(100,0);
    M5.Lcd.printf("  VIBRATING  ");
    M5.Lcd.setCursor(100,40);
    M5.Lcd.printf("%5.4f", avg);
    return true;
  }else if((avg < THRESHOLD_VIBRATION)&&(avg != -1)){
    M5.Lcd.setCursor(100,0);
    M5.Lcd.printf("NOT VIBRATING");
        M5.Lcd.setCursor(100,40);
    M5.Lcd.printf("%5.4f", avg);
    return false;
  }else if (avg == -1){
    available = true;
    return false;
  }
}

void open_check(){
  M5.IMU.getAccelData(&accX, &accY,&accZ);
  if (accZ < - THRESHOLD_ACCELERATION){
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(100,150);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("closed");
    open_flag = false;
    delay(800);
  }else if(accZ > THRESHOLD_ACCELERATION){
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(100,150);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf(" open ");
    open_flag = true;
    available = true;
    delay(800);
  }
}

bool Availability_check(void){ // checks if machine is available - classified as available if machine's average vibration does not exceed threshold value for 5 minutes
  M5.Rtc.GetTime(&currenttime); // gets and stores current time
  int currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds;
  int targettimeseconds = currenttimeseconds + 60; // sets target time to be at a point in time 5 minutes away from current time
  while ((targettimeseconds) > (currenttimeseconds)){ // loops for 1 minutes
    M5.IMU.getAccelData(&accX, &accY,&accZ);
    open_check();
    if (open_flag){
      return(true);
    }
    if (Is_shaking(accX, accY, accZ)){
      return(false); // exits loop, tells us the machine is being used, so it is not available
    }
    M5.Rtc.GetTime(&currenttime);
    currenttimeseconds = currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds;
  }
  return(true); // since the average vibration did not exceed the threshold for the enitrety of the 5 minutue period, the machine is available
}


void setup() {
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
 
  currenttime.Hours = 0;
  currenttime.Minutes = 0;
  currenttime.Seconds = 0;

  M5.Rtc.SetTime(&currenttime);
}


void loop() {
  M5.IMU.getAccelData(&accX, &accY, &accZ);  // Stores the accelerometer data to the relevant variable

  if ((Is_shaking(accX, accY, accZ))&&(open_flag == false)) {
    available = false;
  }else{
    available = Availability_check();
  }
  if (available){
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(100,100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("  AVAILABLE  ");
  }else{
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(100,100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("NOT AVAILABLE");
  }
}
