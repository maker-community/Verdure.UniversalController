/*
 Name:		_1.ino
 Created:	2023/8/4 8:42:26
 Author:	卜开元
*/


#include <Wire.h>
#include "MCP23017.h"
#include <MPU6050_tockn.h>


MPU6050 mpu6050(Wire);
MCP23017 mcp;

void setup() {

    Serial.begin(115200);

    Wire.setPins(41, 40);  // 设置I2C引脚（sda, scl）。 一共只有两个I2C资源，另一个【Wire1】
    Wire.begin();

    mpu6050.begin();
    //mpu6050.calcGyroOffsets(true);  // 校准(若硬件不是标准的水平位置安装，用于根据当前位置初始化零点)

    mcp.begin(7);           // 7:0x27, 6:0x26, 5:0x25, 4:0x24, 3:0x23, 2:0x22, 1:0x21, 0:0x20

    mcp.pinMode(0, INPUT);  // 配置扩展板的IO为输入模式（不能用INPUT_PULL!）
    mcp.pinMode(1, INPUT);
    mcp.pinMode(2, INPUT);  //也可以是输出模式 mcp.pinMode(1, OUTPUT);
    mcp.pinMode(3, INPUT);
    mcp.pinMode(4, INPUT);
    mcp.pinMode(5, INPUT);
    mcp.pinMode(6, INPUT);
    mcp.pinMode(7, INPUT);

    mcp.pullUp(0, HIGH);    // 上拉需要单独使用pullUp函数（内部上拉电阻）
    mcp.pullUp(1, HIGH);
    mcp.pullUp(2, HIGH);
    mcp.pullUp(3, HIGH);
    mcp.pullUp(4, HIGH);
    mcp.pullUp(5, HIGH);
    mcp.pullUp(6, HIGH);
    mcp.pullUp(7, HIGH);

    Serial.println("设置完成");
}



void loop() {

    if (!mcp.digitalRead(0)) { Serial.println("0按下"); }
    if (!mcp.digitalRead(1)) { Serial.println("1按下"); }
    if (!mcp.digitalRead(2)) { Serial.println("2按下"); }
    if (!mcp.digitalRead(3)) { Serial.println("3按下"); }
    if (!mcp.digitalRead(4)) { Serial.println("4按下"); }
    if (!mcp.digitalRead(5)) { Serial.println("5按下"); }
    if (!mcp.digitalRead(6)) { Serial.println("6按下"); }
    if (!mcp.digitalRead(7)) { Serial.println("7按下"); }

    mpu6050.update();                   // 必须要频繁的更新
    Serial.print("angleX : ");
    Serial.print(mpu6050.getAngleX());  
    Serial.print("\t\tangleY : "); 
    Serial.println(mpu6050.getAngleY());

    delay(1);
}
