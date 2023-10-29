
// ----------------ESP32通用开发板的【接收端】-----------------------------------------------------------------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h> // 声明调用Servo.h库 
#define SERVO_PIN 6//宏定义舵机控制引脚
Servo myservo;   //创建一个舵机类，命名为myservo

RF24 radio(9, 10); // CE, CSN   频率默认10000000Hz（10MHz）

const byte address[6] = "00001";

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

void setup() {
   Serial.begin(9600);
   myservo.attach(SERVO_PIN); 
   radio.begin();
   radio.openReadingPipe(0, address);  // 接收端打开通信通道
   radio.setPALevel(RF24_PA_MIN);      // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
   radio.startListening();             // 开始监听，即设置为接收端
   Serial.println("我是接收端");
}

void loop() {
   delay(100);
   if (radio.available()) {            // 是否有可用的数据（是否收到数据）
       radio.read(&tank_kvs, sizeof(tank_kvs));
       Serial.println("通讯正常");
       if(tank_kvs.LX>=0){
        Serial.println("控制舵机正");
        myservo.write(tank_kvs.LX); 
       }
       else{
        Serial.println("控制舵机反");
        myservo.write((90/100)*tank_kvs.LX+90); 
       }
   }
}