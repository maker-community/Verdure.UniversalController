#pragma once

#include "controller_keys.h"
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>




class NRF {
public:

	// 构造函数，传入CE和CSN引脚,同时用SPI指针构造RF24对象
	NRF(int cePin=43, int csnPin=42);

	// 初始化, 传入遥控器编号，设置天线功率，低到高：0-3   0：-16dBm 1：-12dBm 2：-6dBm 3：0dBm
	void init(int conID, uint8_t PALevel=0);

	// 测试遥控器间通信 按右侧板载按键
	void testConToCon();
	
	// RF24对象
	RF24 radio;

private:


	
	// 遥控器编号
	int _conID;  

	// 地址列表
	byte _addresses[2][6] = { "00000", "00001" };  // 为双向通信创建两个管道或地址

};


