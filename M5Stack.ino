#include <M5Core2.h>
#define THRESHOLD_VIBRATION 8


float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;
bool available = false;


RTC_TimeTypeDef targettime;
RTC_TimeTypeDef currenttime;




bool Is_shaking(float gyroX, float gyroY, float gyroZ){
  if ((gyroX > THRESHOLD_VIBRATION)||(gyroY > THRESHOLD_VIBRATION)||(gyroZ > THRESHOLD_VIBRATION)){
      return true;
  }else{
    return false;
  }
}



bool Availability_check(void){
  M5.Rtc.GetTime(&currenttime); // gets and stores current time
  targettime = currenttime;
  targettime.Minutes = currenttime.Minutes + 10; // calculates target time using current time
  while ((targettime.Hours*60 + targettime.Minutes) > (currenttime.Hours*60 + currenttime.Minutes)){ // loops for 10 minutes
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    if (Is_shaking(gyroX, gyroY, gyroZ)){
      return(false); // exits loop, tells us the machine is being used
    }
    M5.Rtc.GetTime(&currenttime);
  }
  return(true);
}

bool vibration_check(void){
  M5.Rtc.GetTime(&currenttime); // gets and stores current time
  targettime = currenttime;
  targettime.Seconds = currenttime.Seconds + 1; // calculates target time using current time
  while ((targettime.Hours*3600 + targettime.Minutes*60+targettime.Seconds) > (currenttime.Hours*3600 + currenttime.Minutes*60+ currenttime.Seconds)){ // loops for 10 minutes
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    if (Is_shaking(gyroX, gyroY, gyroZ) == false){
      return(false); // exits loop, tells us the machine is being used
    }
    M5.Rtc.GetTime(&currenttime);
  }
  return(true);
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
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);  // Stores the gyroscope data to the relevant variable


  if (vibration_check() == true) {
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.setCursor(100,0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("  VIBRATING  ");
    available = false;
  }else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.setCursor(100,0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("NOT VIBRATING");
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
