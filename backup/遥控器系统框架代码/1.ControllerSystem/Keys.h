#pragma once

#include <Arduino.h>
#include "controller_keys.h"

#include <Wire.h>		    // I2C库
#include <MPU6050_tockn.h>  // 陀螺仪库
#include <MCP23017.h>       // IO扩展库
#include <Bounce2.h>

// 键值结构体定义
struct KVS
{
	// 4个前端按键
	bool L_up = 1;
	bool L_down = 1;
	bool R_up = 1;
	bool R_down = 1;

	// 电位器和摇杆  ------------------ int8_t 是有符号char型 -128 到 127
	int8_t L_knob = 0;
	int8_t LX = 0;
	int8_t LY = 0;
	int8_t R_knob = 0;
	int8_t RX = 0;
	int8_t RY = 0;

	// 2个板载按键
	bool board_L = 1;
	bool board_R = 1;

	// 8个功能按键
	bool up = 1;
	bool down = 1;
	bool left = 1;
	bool right = 1;
	bool o = 1;
	bool x = 1;
	bool a = 1;
	bool b = 1;

	// 4个拨杆开关
	bool switch_L1 = 1;
	bool switch_L2 = 1;
	bool switch_R1 = 1;
	bool switch_R2 = 1;

	// 陀螺仪数据
	int8_t angleX = 0;  // -128 到 127
	int8_t angleY = 0;
};

class MCP_bounce
{
public:
	MCP_bounce(uint8_t pin, bool default_state);   // 构造函数，传入IO口和默认状态
	bool pressed();     // 单击
	bool read();        // 读取按键状态
	bool on();          // 打开 返回read() != _default_state;，用于拨杆开关，on()和off()更直观
	bool off();         // 关闭 返回read() != _default_state;，用于拨杆开关，on()和off()更直观

private:
	uint8_t _pin;
	bool _default_state;
	bool _isPressed_flag = false;
};




class Keys
{
public:
	// MCP IO扩展按键消抖对象，方法为自写
	MCP_bounce up;
	MCP_bounce down;
	MCP_bounce left;
	MCP_bounce right;
	MCP_bounce o;
	MCP_bounce x;
	MCP_bounce a;
	MCP_bounce b;
	MCP_bounce L1;
	MCP_bounce L2;
	MCP_bounce R1;
	MCP_bounce R2;

	// Bounce2库按键对象
	Bounce2::Button L_up;
	Bounce2::Button L_down;
	Bounce2::Button R_up;
	Bounce2::Button R_down;
	Bounce2::Button board_L;
	Bounce2::Button board_R;

	MPU6050 mpu6050;
	
	// 键值结构体
	KVS kvs;

	// 构造函数中初始化 MPU6050 对象,	和MCP23017按键对象	
	Keys();

	// 初始化设置
	void init(int conID);

	// 按键更新，到kvs结构体
	void kvs_update();

	// 串口打印按键数据
	void ShowInSerial();

	// 按键测试，检测消抖效果，单击、按下、释放
	void dounnceTest();

	// 按键消抖更新，前端4个按键+板载2个按键
	void bounce_update();

private:

	// 遥控器编号
	int _conID = 0;

	// 摇杆纠正零位值
	int _zero = 15;
};

