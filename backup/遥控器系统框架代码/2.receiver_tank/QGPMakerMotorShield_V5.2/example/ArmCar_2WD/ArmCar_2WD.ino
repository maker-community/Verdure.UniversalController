#include <Wire.h>
#include "PS2X_lib.h"
#include "QGPMaker_MotorShield.h"

#define TRIGGER_PIN  A0
#define ECHO_PIN     A1
#define SPEED  255

QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();
PS2X ps2x;
//Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

long ARM_MIN[] = {10, 10, 40, 10};
long ARM_MAX[] = {170, 140, 170, 102};

QGPMaker_Servo *Servo1 = AFMS.getServo(1);
QGPMaker_Servo *Servo2 = AFMS.getServo(2);
QGPMaker_Servo *Servo3 = AFMS.getServo(3);
QGPMaker_Servo *Servo4 = AFMS.getServo(4);
QGPMaker_DCMotor *DCMotor_1 = AFMS.getMotor(1);
QGPMaker_DCMotor *DCMotor_3 = AFMS.getMotor(3);
QGPMaker_DCMotor *DCMotor_4 = AFMS.getMotor(4);


//Grab something
void openGripper() {
  Servo4->writeServo(ARM_MIN[3]);
  delay(300);
}

void closeGripper() {
  Servo4->writeServo(ARM_MAX[3]);
  delay(300);
}

void setup() {
  AFMS.begin(50);
  int error = 0;
  do {
    error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
    if (error == 0) {
      break;
    } else {
      delay(100);
    }
  } while (1);
  Servo1->writeServo(90);
  Servo2->writeServo(90);
  Servo3->writeServo(90);
  Servo4->writeServo(60);
}



void loop() {
  ps2x.read_gamepad(false, 0);
  if (ps2x.Button(PSB_PAD_UP)) {
    DCMotor_3->setSpeed(SPEED);
    DCMotor_4->setSpeed(SPEED);
    if (ps2x.Button(PSB_L1)) {   
       DCMotor_3->setSpeed(0);
    }
    if (ps2x.Button(PSB_R1)) {   
       DCMotor_4->setSpeed(0);
    }
    DCMotor_3->run(FORWARD);
    DCMotor_4->run(FORWARD);
  } else if (ps2x.Button(PSB_PAD_DOWN)) {
    DCMotor_3->setSpeed(SPEED);
    DCMotor_4->setSpeed(SPEED);
    if (ps2x.Button(PSB_L1)) {   
       DCMotor_3->setSpeed(0);
    }
    if (ps2x.Button(PSB_R1)) {   
       DCMotor_4->setSpeed(0);
    }
    DCMotor_3->run(BACKWARD);
    DCMotor_4->run(BACKWARD);
  } else if (ps2x.Button(PSB_PAD_LEFT)) {
    DCMotor_3->setSpeed(SPEED);
    DCMotor_3->run(BACKWARD);
    DCMotor_4->setSpeed(SPEED);
    DCMotor_4->run(FORWARD);
  } else if (ps2x.Button(PSB_PAD_RIGHT)) {
    DCMotor_3->setSpeed(SPEED);
    DCMotor_3->run(FORWARD);
    DCMotor_4->setSpeed(SPEED);
    DCMotor_4->run(BACKWARD);
  } else {
    DCMotor_3->setSpeed(0);
    DCMotor_3->run(RELEASE);
    DCMotor_4->setSpeed(0);
    DCMotor_4->run(RELEASE);
  }

  if (ps2x.Button(PSB_CROSS)) {
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);
  }
  if (ps2x.Button(PSB_TRIANGLE)) {
    DCMotor_1->setSpeed(SPEED);
    DCMotor_1->run(FORWARD);
  }else if(ps2x.ButtonReleased(PSB_TRIANGLE)) {
    DCMotor_1->setSpeed(0);
    DCMotor_1->run(RELEASE);
  }
  if (ps2x.ButtonPressed(PSB_CIRCLE)) {   
    openGripper();
  }
  if (ps2x.ButtonPressed(PSB_SQUARE)) { 
    closeGripper() ;
  }
  if (ps2x.Analog(PSS_LX) > 240) {
    if (Servo1->readDegrees() > ARM_MIN[(int)(0)]) {
      Servo1->writeServo((Servo1->readDegrees() - 1));
    }
  } else if (ps2x.Analog(PSS_LX) < 10) {
    if (Servo1->readDegrees() < ARM_MAX[(int)(0)]) {
      Servo1->writeServo((Servo1->readDegrees() + 1));
    }
  }

  if (ps2x.Analog(PSS_LY) > 240) {
    if (Servo2->readDegrees() > ARM_MIN[(int)(1)]) {
      Servo2->writeServo((Servo2->readDegrees() - 1));
    }
  } else if (ps2x.Analog(PSS_LY) < 10) {
    if (Servo2->readDegrees() < ARM_MAX[(int)(1)]) {
      Servo2->writeServo((Servo2->readDegrees() + 1));
    }
  }

  if (ps2x.Analog(PSS_RY) > 240) {
    if (Servo3->readDegrees() > ARM_MIN[(int)(2)]) {
      Servo3->writeServo((Servo3->readDegrees() - 1));
    }
  } else if (ps2x.Analog(PSS_RY) < 10) {
    if (Servo3->readDegrees() < ARM_MAX[(int)(2)]) {
      Servo3->writeServo((Servo3->readDegrees() + 1));
    }
  }

  if (ps2x.Analog(PSS_RX) > 240) {
    if (Servo4->readDegrees() > ARM_MIN[(int)(3)]) {
      Servo4->writeServo((Servo4->readDegrees() - 1));
    }
  } else if (ps2x.Analog(PSS_RX) < 10) {
    if (Servo4->readDegrees() < ARM_MAX[(int)(3)]) {
      Servo4->writeServo((Servo4->readDegrees() + 1));
    }
  }
  delay(10);
}

//void obstacle(){
//  float cmMsec =ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
//  if(cmMsec<5){
//    DCMotor_3->setSpeed(SPEED);
//    DCMotor_3->run(FORWARD);
//    DCMotor_4->setSpeed(SPEED);
//    DCMotor_4->run(BACKWARD);
//    delay(100);
//  }else{
//    DCMotor_3->setSpeed(SPEED);
//    DCMotor_4->setSpeed(SPEED);
//    DCMotor_3->run(FORWARD);
//    DCMotor_4->run(FORWARD);
//  }
//  
//}
