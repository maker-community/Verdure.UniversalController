#include "LED.h"
#include "controller_keys.h"
#include <Arduino.h>


// ≥ı ºªØ…Ë÷√
void LED::init()
{
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, LOW);
}

//  LED¡¡
void LED::on()
{
	digitalWrite(PIN_LED, HIGH);
}

// LED√
void LED::off()
{
	digitalWrite(PIN_LED, LOW);
}
