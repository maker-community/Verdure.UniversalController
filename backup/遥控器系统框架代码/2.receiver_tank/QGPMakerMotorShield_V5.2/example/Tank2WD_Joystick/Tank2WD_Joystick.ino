#include <Wire.h>
#include "PS2X_lib.h"
#include "QGPMaker_MotorShield.h"

QGPMaker_MotorShield  AFMS = QGPMaker_MotorShield();
PS2X ps2x;
QGPMaker_DCMotor *DCMotor_3 = AFMS.getMotor(3);
QGPMaker_DCMotor *DCMotor_4 = AFMS.getMotor(4);

void setup(){
  AFMS.begin(50);

  int error = 0;
  do{
    error = ps2x.config_gamepad(13,11,10,12, true, true);
    if(error == 0){
      break;
    }else{
      delay(100);
    }
  }while(1);
  for(size_t i = 0; i < 50; i++)
  {
        ps2x.read_gamepad(false, 0);
        delay(10);
  }

}

void loop(){
  ps2x.read_gamepad(false, 0);
  delay(3);
  if (ps2x.Button(PSB_CROSS)) {
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);

  }
  if (ps2x.Analog(PSS_LY) > 130) {
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_3->run(FORWARD);

  } else if (ps2x.Analog(PSS_LY) < 125) {
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 125, 0, 30, 255)));
    DCMotor_3->run(BACKWARD);
  } else {
    DCMotor_3->setSpeed(0);
    DCMotor_3->run(RELEASE);

  }
  if (ps2x.Analog(PSS_RY) > 130) {
    DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RY), 130, 255, 30, 255)));
    DCMotor_4->run(FORWARD);

  } else if (ps2x.Analog(PSS_RY) < 125) {
    DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RY), 125, 0, 30, 255)));
    DCMotor_4->run(BACKWARD);
  } else {
    DCMotor_4->setSpeed(0);
    DCMotor_4->run(RELEASE);

  }

}
