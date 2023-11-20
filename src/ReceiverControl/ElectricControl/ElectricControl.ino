
// ----------------ESP32通用开发板的【接收端】-----------------------------------------------------------------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>  // 声明调用Servo.h库
#define SERVO_PIN 6 // 宏定义舵机控制引脚

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700

#define MOTOR_PIN 3 // 九号引脚是紫色的引脚 也就是D3
#define DUOJI_PIN 6 // 舵机引脚

int idle = (MAX_SIGNAL + MIN_SIGNAL) / 2;
int slow = (MAX_SIGNAL - idle) / 3;
int med = slow * 2;
int fast = MAX_SIGNAL;
int callibrate = 2;

int pos = 0; // 角度存储变量

Servo motor;
Servo duoji;

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

void setup()
{
   Serial.begin(9600);
   motor.attach(MOTOR_PIN);
   duoji.attach(DUOJI_PIN); // 舵机初始化
   radio.begin();
   radio.openReadingPipe(0, address); // 接收端打开通信通道
   radio.setPALevel(RF24_PA_MIN);     // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
   radio.startListening();            // 开始监听，即设置为接收端
                                      // Serial.println("我是接收端");
   if (callibrate != 1)
   {
      motor.writeMicroseconds(idle);
   }
}

void loop()
{
   if (radio.available())
   { // 是否有可用的数据（是否收到数据）
      radio.read(&tank_kvs, sizeof(tank_kvs));
      
      int angle = map(tank_kvs.LX, -100, 100, 180, 0);  // 将遥控器数据映射到舵机角度范围

      duoji.write(angle);  // 将舵机转动到对应角度

      if (tank_kvs.RY < 0)
      {
         // 控制车子前进
         // int ry = abs(tank_kvs.RY);
         // int speed = 250 * (ry / 100) + 1350;
         int ry = abs(tank_kvs.RY);


         int speed1 = map(tank_kvs.RY, -100, 100, 2000, 700);
         //int speed = ry*2 + 1350;
         //Serial.println(speed);
         motor.writeMicroseconds(speed1);
      }
      delay(50);
   }
}