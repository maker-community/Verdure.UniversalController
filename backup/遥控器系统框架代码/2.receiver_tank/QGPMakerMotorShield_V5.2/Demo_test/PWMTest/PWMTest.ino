
#include <Wire.h>
#include "QGPMaker_MotorShield.h"

// Create the motor shield object with the default I2C address
QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("DC Motor test!");

  AFMS.begin();  // create with the default frequency 1.6KHz

}

void loop() {

//PWM输出
  AFMS.setPWM(0, 140);
  delay(3000);
  AFMS.setPWM(0, 4000);
  delay(3000);

//开关输出
  AFMS.setPin(2, HIGH);
  delay(3000);
  AFMS.setPWM(2, LOW);
  delay(3000);
}
