#include "Buzzer.h"

// 初始化配置
void Buzzer::init()
{
	ledcSetup(0, 3000, 12);       // 初始化ledc (pwm通道号，频率，比特数)
	ledcWrite(0, 0);			  // 占空比初始化为0，即低电平，不响
	ledcAttachPin(PIN_BUZZER, 0); // 将引脚和pwm通道连接 (pin, PWM_ch)	
}


// 开启蜂鸣器  蜂鸣器类型  0:无源-黑（默认） 1:有源-壮  
void Buzzer::on()
{
	ledcWrite(0, pow(2, 10));     //占空比50%，按已设置好的频率（3000）蜂鸣
}


// 停止蜂鸣
void Buzzer::off()
{
	ledcWrite(0, 0);		// 占空比初始化为0，即低电平，不响
}

// 改变频率
void Buzzer::changeFreq(uint32_t freq)
{
	//ledcWriteTone(0, freq);
	ledcChangeFrequency(0, freq, 12);
}

// 改变频率测试
void Buzzer::changeFreqTest()
{
	ledcWrite(0, pow(2, 10));
	while (digitalRead(PIN_L_UP) || digitalRead(PIN_R_UP))
	{
		changeFreq(analogRead(PIN_LX));
		delay(50);
	}
}




// 蜂鸣器发声音频率
// 1 - 200Hz声音很小
// 200 - 300有声音
// 400嘟
// 500滴
// 600音调变高
// 700音调变高
// 800音调变高
// 2730Hz适合做滴的一声
// 3000最剌耳, 声音大
