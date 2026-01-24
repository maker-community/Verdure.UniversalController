
// ----------------ESP32通用开发板的【接收端】-----------------------------------------------------------------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>  // 声明调用Servo.h库
#define SERVO_PIN 6 // 宏定义舵机控制引脚

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000

#define MOTOR_PIN 3 // 九号引脚是紫色的引脚 也就是D3
#define DUOJI_PIN 6 // 舵机引脚

const int ESC_NEUTRAL = 1500; // 根据电调校准的中位值调整
const int ESC_DEADBAND = 5;  // 中位死区，避免抖动
const bool CALIBRATION_BY_POWER = true; // 按住power进入校准模式
const int CALIBRATION_THRESHOLD = 50; // 校准阈值（摇杆超过此值进入最大）
const float THROTTLE_EXPO = 1.7f; // 油门曲线>1更柔和
const float REVERSE_EXPO = 2.2f; // 倒车曲线>1更柔和（比前进更柔和）
const unsigned long SIGNAL_TIMEOUT_MS = 200; // 信号超时阈值（毫秒）
const int SERVO_NEUTRAL_ANGLE = 85; // 舵机中位角度
const float REVERSE_SMOOTH_FACTOR = 0.15f; // 倒车速度平滑系数（0.1-0.3，越小越平滑）
const int REVERSE_MAX_PERCENT = 80; // 倒车最大速度百分比（80=80%最大速度）
int callibrate = 2;

int pos = 0; // 角度存储变量

// 信号监控
unsigned long lastSignalTime = 0; // 最后接收到信号的时间
bool signalLost = true; // 信号丢失标志
unsigned int signalLostCount = 0; // 连续丢失计数

// 倒车平滑过渡
int currentSpeed = ESC_NEUTRAL; // 当前实际速度
int targetSpeed = ESC_NEUTRAL;  // 目标速度

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
      motor.writeMicroseconds(ESC_NEUTRAL);
   }
   
   // 初始化信号监控
   lastSignalTime = millis();
   duoji.write(SERVO_NEUTRAL_ANGLE); // 舵机初始回中
   
   Serial.println("接收端初始化完成，等待信号...");
}

void loop()
{
   unsigned long currentTime = millis();
   
   if (radio.available())
   { // 接收到数据
      radio.read(&tank_kvs, sizeof(tank_kvs));
      
      // 更新信号状态
      lastSignalTime = currentTime;
      if (signalLost) {
         signalLost = false;
         signalLostCount = 0;
         Serial.println("信号恢复");
      }
      
      // 舵机控制
      int angle = map(tank_kvs.LX, 100, -100, 120, 50);  // 将遥控器数据映射到舵机角度范围
      duoji.write(angle);  // 将舵机转动到对应角度

      // 电机控制
      int speed1 = ESC_NEUTRAL;
      bool calibrationMode = CALIBRATION_BY_POWER && tank_kvs.power;
      int ry = constrain(tank_kvs.RY, -100, 100);

      if (calibrationMode)
      {
         // 校准模式：上推(-100)=正向最大, 下推(100)=反向最大
         if (ry < -CALIBRATION_THRESHOLD)
         {
            speed1 = MAX_SIGNAL; // 上推：正向最大
         }
         else if (ry > CALIBRATION_THRESHOLD)
         {
            speed1 = MIN_SIGNAL; // 下推：反向最大
         }
         else
         {
            speed1 = ESC_NEUTRAL; // 中位
         }
      }
      else
      {
         // 正常模式：电调已有倒车保护，代码只负责平滑控制
         if (abs(ry) <= ESC_DEADBAND)
         {
            speed1 = ESC_NEUTRAL;
            targetSpeed = ESC_NEUTRAL;
            currentSpeed = ESC_NEUTRAL;
         }
         else
         {
            // 油门曲线：上推(-100)=正向, 下推(100)=反向
            float x = ry / 100.0f; // -1..1
            float ax = fabs(x);
            float curved;
            
            // 根据方向使用不同的曲线
            if (x < 0) {
               // 前进使用标准曲线
               curved = pow(ax, THROTTLE_EXPO);
            } else {
               // 倒车使用更柔和的曲线
               curved = pow(ax, REVERSE_EXPO);
            }
            
            float y = (x < 0) ? -curved : curved; // -1..1

            // 计算目标速度
            if (y < 0)
            {
               // 上推（负值）：正向
               targetSpeed = ESC_NEUTRAL + (int)((MAX_SIGNAL - ESC_NEUTRAL) * (-y));
               // 前进：直接响应（保持原有灵敏度）
               currentSpeed = targetSpeed;
               speed1 = targetSpeed;
            }
            else
            {
               // 下推（正值）：反向（限制最大速度）
               float reverseRange = (ESC_NEUTRAL - MIN_SIGNAL) * (REVERSE_MAX_PERCENT / 100.0f);
               targetSpeed = ESC_NEUTRAL - (int)(reverseRange * y);
               // 倒车：渐进式速度调整（平滑过渡）
               float speedDiff = targetSpeed - currentSpeed;
               currentSpeed += (int)(speedDiff * REVERSE_SMOOTH_FACTOR);
               speed1 = currentSpeed;
            }
         }
      }

      motor.writeMicroseconds(speed1);
      delay(80);
   }
   else
   {
      // 未接收到数据，检查信号超时
      if (currentTime - lastSignalTime > SIGNAL_TIMEOUT_MS)
      {
         // 信号超时，执行失联保护
         if (!signalLost) {
            signalLost = true;
            Serial.println("警告：信号丢失！");
         }
         
         signalLostCount++;
         
         // 安全停止：电机和舵机都回中
         motor.writeMicroseconds(ESC_NEUTRAL);
         duoji.write(SERVO_NEUTRAL_ANGLE);
         currentSpeed = ESC_NEUTRAL;
         targetSpeed = ESC_NEUTRAL;
         
         // 每隔1秒输出一次警告（避免刷屏）
         if (signalLostCount % 10 == 0) {
            Serial.print("信号丢失持续: ");
            Serial.print((currentTime - lastSignalTime) / 1000.0);
            Serial.println("秒");
         }
      }
      else
      {
         // 信号短暂中断但未超时，保持当前状态或回中（可选）
         motor.writeMicroseconds(ESC_NEUTRAL);
      }
      
      delay(10); // 短延迟，快速检测信号恢复
   }
}