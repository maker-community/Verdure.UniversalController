
#include <Wire.h>
#include "QGPMaker_MotorShield.h"

QGPMaker_MotorShield AFMS = QGPMaker_MotorShield(); //创建驱动器对象
QGPMaker_Servo *Servo6 = AFMS.getServo(6); 
QGPMaker_Servo *Servo7 = AFMS.getServo(7); 

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("DC Motor test!");

  AFMS.begin(50);
  
}

void loop() {
  Servo6->writeServo(90); //3#舵机转到10度位置
  Servo7->writeServo(85); //3#舵机转到120度位置
  delay(1000);



}
