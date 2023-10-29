#pragma once
#include "controller_keys.h"
#include <Arduino.h>

class Buzzer
{
public:
	// 初始化
	void init();

	// 开启蜂鸣器
	void on();

	// 停止蜂鸣
	void off();

	// 改变频率
	void changeFreq(uint32_t freq);

	// 改变频率测试 (同时按前端两个上侧按键退出)
	void changeFreqTest();
};

