/*
 Name:		_2.ino
 Created:	2023/9/9 16:01:57
 Author:	bilibili-黑人黑科技
*/



#include <RF24.h>
#include "QGPMaker_MotorShield.h"

RF24 radio(9, 10); // CE, CSN   频率默认10000000Hz（10MHz）
const byte address[6] = "00001";

QGPMaker_MotorShield AFMS = QGPMaker_MotorShield(); //创建驱动器对象
QGPMaker_Servo* Servo6 = AFMS.getServo(6);
QGPMaker_Servo* Servo7 = AFMS.getServo(7);
QGPMaker_DCMotor* mot_Left = AFMS.getMotor(1);
QGPMaker_DCMotor* mot_right = AFMS.getMotor(2);
QGPMaker_DCMotor* mot_trigger = AFMS.getMotor(3);
QGPMaker_DCMotor* mot_power = AFMS.getMotor(4);


struct TANK_KVS
{
	// 4个前端按键
	bool light = 0;
	bool power = 0;
	bool trigger = 1;

	// int8_t 是有符号char型 -128 到 127
	int8_t LX = 0;
	int8_t LY = 0;
	int8_t RX = 0;
	int8_t RY = 0;
};


TANK_KVS tank_kvs;


uint8_t servo_X = 85;
uint8_t servo_Y = 85;

void setup() {
	Serial.begin(9600);
	AFMS.begin(50); 

	radio.begin();										
	radio.openReadingPipe(0, address);  // 接收端打开通信通道
	radio.setPALevel(RF24_PA_MIN);      // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	radio.startListening();             // 开始监听，即设置为接收端

	pinMode(A0, OUTPUT);
}


void loop() {
	if (radio.available()) {           
		radio.read(&tank_kvs, sizeof(tank_kvs));
	}


	// 车辆行进
	if (tank_kvs.LY < -10 && (tank_kvs.LX < -20 || tank_kvs.LX > 20))
	{
		mot_Left->run(FORWARD);
		mot_right->run(BACKWARD);
		if (tank_kvs.LX < -20)
		{
			mot_Left->setSpeed(map(tank_kvs.LY, -10, -100, 11, 255));
			mot_right->setSpeed(0);
		}
		else if (tank_kvs.LX > 20)
		{
			mot_Left->setSpeed(0);
			mot_right->setSpeed(map(tank_kvs.LY, -10, -100, 11, 255));
		}
	}
	else if (tank_kvs.LY > 10 && (tank_kvs.LX < -20 || tank_kvs.LX > 20))
	{
		mot_Left->run(BACKWARD);
		mot_right->run(FORWARD);
		if (tank_kvs.LX < -20)
		{
			mot_Left->setSpeed(map(tank_kvs.LY, 10, 100, 11, 255));
			mot_right->setSpeed(0);
		}
		else if (tank_kvs.LX > 20)
		{
			mot_Left->setSpeed(0);
			mot_right->setSpeed(map(tank_kvs.LY, 10, 100, 11, 255));
		}
	
	}
	else if (tank_kvs.LY < -10)  // 前进
	{
		mot_Left->run(FORWARD);
		mot_right->run(BACKWARD);
		mot_Left->setSpeed(map(tank_kvs.LY, -10, -100, 11, 255));
		mot_right->setSpeed(map(tank_kvs.LY, -10, -100, 11, 255));
	}
	else if (tank_kvs.LY > 10)  // 后退
	{
		mot_Left->run(BACKWARD);
		mot_right->run(FORWARD);
		mot_Left->setSpeed(map(tank_kvs.LY, 10, 100, 11, 255));
		mot_right->setSpeed(map(tank_kvs.LY, 10, 100, 11, 255));
	}
	else if (tank_kvs.LX < -10)  // 左转圈
	{
		mot_Left->run(FORWARD);
		mot_right->run(FORWARD);
		mot_Left->setSpeed(map(tank_kvs.LX, -10, -100, 11, 255));
		mot_right->setSpeed(map(tank_kvs.LX, -10, -100, 11, 255));
	}
	else if (tank_kvs.LX > 10)  // 右转圈
	{
		mot_Left->run(BACKWARD);
		mot_right->run(BACKWARD);
		mot_Left->setSpeed(map(tank_kvs.LX, 10, 100, 11, 255));
		mot_right->setSpeed(map(tank_kvs.LX, 10, 100, 11, 255));
	}
	else
	{
		mot_Left->run(RELEASE);
		mot_right->run(RELEASE);
	}

	// 炮台舵机
	if (tank_kvs.RX < -20) { if (servo_X < 130) { servo_X++;  delay(1); } }
	else if (tank_kvs.RX > 20) { if (servo_X > 40) { servo_X--;  delay(1); } }
	else if (tank_kvs.RY < -20) { if (servo_Y > 60) { servo_Y--;  delay(5); } }
	else if (tank_kvs.RY > 20) { if (servo_Y < 110) { servo_Y++;  delay(5); } }
	Servo6->writeServo(servo_Y);
	Servo7->writeServo(servo_X); 

	// 动力电机
	if (tank_kvs.power){ mot_power -> run(BACKWARD); mot_power -> setSpeed(255); }
	else { mot_power -> run(RELEASE); }
	
	// 送牌电机
	if (!tank_kvs.trigger) { mot_trigger->run(FORWARD); mot_trigger->setSpeed(255); }
	else { mot_trigger -> run(RELEASE); }
	
	// 灯
	if(tank_kvs.light){ digitalWrite(A0, LOW); }
	else { digitalWrite(A0, HIGH); }

	
}