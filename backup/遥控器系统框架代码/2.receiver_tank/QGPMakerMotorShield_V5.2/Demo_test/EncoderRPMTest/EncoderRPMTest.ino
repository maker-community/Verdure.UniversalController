
#include <Wire.h>
#include "QGPMaker_MotorShield.h"
#include "QGPMaker_Encoder.h"

volatile double pos;
volatile double rpm;
volatile int PPR;
volatile int gearratio;
volatile int CPR;

QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();
QGPMaker_DCMotor *DCMotor_1 = AFMS.getMotor(1);
QGPMaker_Encoder Encoder1(1);

void setup(){
  AFMS.begin(50);
  Serial.begin(9600);
  pos = 0;
  rpm = 0;
  PPR = 12;// 编码器基础脉冲（磁极对）
  gearratio = 90;// 电机齿数比
  CPR = (PPR * 4) * gearratio; // 轮轴每周的脉冲数
  
  DCMotor_1->setSpeed(250);
  DCMotor_1->run(FORWARD);
  
}

void loop(){
  pos = Encoder1.read();
  rpm = 600 * (pos / CPR); 
  Encoder1.write(0);
  
  Serial.println(rpm);
  delay(100);//计算间隔：100ms
}
