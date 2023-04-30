#include <M5Core2.h>

float accX = 0.0F;  // Define variables for storing inertial sensor data
float accY = 0.0F;  //定义存储惯性传感器相关数据的相关变量
float accZ = 0.0F;
int total;
bool isopen=false;

void setup() {
  M5.begin();     //Init M5Core.  初始化 M5Core
  M5.IMU.Init();  //Init IMU sensor.  初始化惯性传感器
  M5.Lcd.fillScreen(
      BLACK);  //Set the screen background color to black. 设置屏幕背景色为黑色
  M5.Lcd.setTextColor(
      GREEN,
      BLACK);  //Sets the foreground color and background color of the displayed text.  设置显示文本的前景颜色和背景颜色
  M5.Lcd.setTextSize(2.1);  //Set the font size.  设置字体大小
}
void loop() {
  // put your main code here, to run repeatedly:
  M5.IMU.getAccelData(
    &accX, &accY,
      &accZ); 

if(accZ<-0.5){
  isopen = false;
  M5.Lcd.setCursor(5, 110);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.printf("door closed");
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.setTextColor(RED, BLACK);
  delay(1000);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.clear(); 
  }
if(accZ>.5 and !isopen){
  M5.Lcd.setCursor(5, 110);
  M5.Lcd.setTextColor(YELLOW, BLACK);
  M5.Lcd.printf("door open");
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.setTextColor(RED, BLACK);
  M5.Lcd.setTextColor(GREEN, BLACK);
  delay(1000);
  M5.Lcd.clear(); 
  isopen = true;
}}
