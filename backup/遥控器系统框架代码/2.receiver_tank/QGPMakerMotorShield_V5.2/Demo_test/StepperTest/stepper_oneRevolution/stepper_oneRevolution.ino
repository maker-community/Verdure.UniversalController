
/*
  电机旋转一周
 */

#include <Arduino.h>
#include "QGPMaker_MotorShield.h"

QGPMaker_MotorShield JFMS = QGPMaker_MotorShield(); //初始化驱动器

const int stepsPerRevolution = 200;  // 步进电机转一周的步数，一般42电机的步数是200步

QGPMaker_StepperMotor *stepper1 = JFMS.getStepper(stepsPerRevolution, 1); //获取1号步进电机（对应M1,M2接口）
QGPMaker_StepperMotor *stepper2 = JFMS.getStepper(stepsPerRevolution, 2); //获取2号步进电机（对应M3,M4接口）


void setup() {
  Serial.begin(9600);
  JFMS.begin();
  stepper1->setSpeed(60);  //RPM速度
  stepper2->setSpeed(6);   //RPM速度
}

void loop() {
  stepper1->step(200, FORWARD,DOUBLE); //电机正转200步（一周）
  stepper2->step(200, FORWARD, MICROSTEP);//电机向前转200步（一周）
  delay(1000);
  stepper1->step(200, BACKWARD,DOUBLE); //电机反转200步（一周）
  stepper2->step(200, BACKWARD, MICROSTEP);
  delay(1000);
}
