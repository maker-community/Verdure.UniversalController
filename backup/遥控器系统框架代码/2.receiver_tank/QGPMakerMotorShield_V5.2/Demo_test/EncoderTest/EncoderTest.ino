
#include <Wire.h>
#include "QGPMaker_MotorShield.h"
#include "QGPMaker_Encoder.h"

QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();
QGPMaker_DCMotor *DCMotor_1 = AFMS.getMotor(1);
QGPMaker_DCMotor *DCMotor_2 = AFMS.getMotor(2);
QGPMaker_DCMotor *DCMotor_3 = AFMS.getMotor(3);
QGPMaker_DCMotor *DCMotor_4 = AFMS.getMotor(4);
QGPMaker_Encoder Encoder1(1);
QGPMaker_Encoder Encoder2(2);
QGPMaker_Encoder Encoder3(3);
QGPMaker_Encoder Encoder4(4);

void setup(){
  AFMS.begin(50);
  Serial.begin(9600);
  DCMotor_1->setSpeed(250);
  DCMotor_1->run(FORWARD);
  DCMotor_2->setSpeed(250);
  DCMotor_2->run(FORWARD);
  DCMotor_3->setSpeed(250);
  DCMotor_3->run(FORWARD);
  DCMotor_4->setSpeed(250);
  DCMotor_4->run(FORWARD);
}

void loop(){

  Serial.print(Encoder1.read());
  Serial.print('>');
  Serial.print(Encoder2.read());
  Serial.print('>');
  Serial.print(Encoder3.read());
  Serial.print('>');
  Serial.println(Encoder4.read());
  delay(50);
}
