#include <Wire.h>
#include "QGPMaker_MotorShield.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


QGPMaker_MotorShield AFMS = QGPMaker_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M3
QGPMaker_DCMotor *mot_Left = AFMS.getMotor(1);
//QGPMaker_DCMotor *mot_right = AFMS.getMotor(2);
//QGPMaker_DCMotor *mot_trigger = AFMS.getMotor(3);
//QGPMaker_DCMotor *mot_power = AFMS.getMotor(4);


RF24 radio(9, 10); // CE, CSN   频率默认10000000Hz（10MHz）
const byte address[6] = "00001";


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("DC Motor test!");

  AFMS.begin();  // create with the default frequency 1.6KHz

  radio.begin();						// 默认使用的VSPI(ESP32)						
  radio.openReadingPipe(0, address);  // 接收端打开通信通道
  radio.setPALevel(RF24_PA_MIN);      // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.startListening();             // 开始监听，即设置为接收端
  
}

void loop() {


  if (radio.available()) {            // 是否有可用的数据（是否收到数据）
    char text[32] = "";              // 一次最多可以传输32个字节。用于接收的变量类型和大小必须与发来的数据一致
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }

       

  //Serial.print("LX: ");
  //Serial.print(kvs.LX);
  //Serial.print("LY: ");
  //Serial.println(kvs.LY);

/*
  if(kvs.LX < -10 )
  {
    mot_Left->run(FORWARD);
    mot_right->run(FORWARD);
    mot_Left->setSpeed(map(kvs.LX,-10,-100,0,255));  
  }

  else if(kvs.LX > 10 )
  {
    mot_Left->run(BACKWARD);
    mot_right->run(BACKWARD);
    mot_Left->setSpeed(map(kvs.LX,10,100,0,255));  
  }

  else
  {
    mot_Left->run(RELEASE);
    mot_right->run(RELEASE);
  }
*/
  delay(50);
}
