#include "LED.h"
#include "controller_keys.h"
#include <Arduino.h>


// 初始化设置
void LED::init()
{
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, LOW);
}

//  LED亮
void LED::on()
{
	digitalWrite(PIN_LED, HIGH);
}

// LED灭
void LED::off()
{
	digitalWrite(PIN_LED, LOW);
}
