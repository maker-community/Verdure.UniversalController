#include "Buzzer.h"

// ��ʼ������
void Buzzer::init()
{
	ledcSetup(0, 3000, 12);       // ��ʼ��ledc (pwmͨ���ţ�Ƶ�ʣ�������)
	ledcWrite(0, 0);			  // ռ�ձȳ�ʼ��Ϊ0�����͵�ƽ������
	ledcAttachPin(PIN_BUZZER, 0); // �����ź�pwmͨ������ (pin, PWM_ch)	
}


// ����������  ����������  0:��Դ-�ڣ�Ĭ�ϣ� 1:��Դ-׳  
void Buzzer::on()
{
	ledcWrite(0, pow(2, 10));     //ռ�ձ�50%���������úõ�Ƶ�ʣ�3000������
}


// ֹͣ����
void Buzzer::off()
{
	ledcWrite(0, 0);		// ռ�ձȳ�ʼ��Ϊ0�����͵�ƽ������
}

// �ı�Ƶ��
void Buzzer::changeFreq(uint32_t freq)
{
	//ledcWriteTone(0, freq);
	ledcChangeFrequency(0, freq, 12);
}

// �ı�Ƶ�ʲ���
void Buzzer::changeFreqTest()
{
	ledcWrite(0, pow(2, 10));
	while (digitalRead(PIN_L_UP) || digitalRead(PIN_R_UP))
	{
		changeFreq(analogRead(PIN_LX));
		delay(50);
	}
}




// ������������Ƶ��
// 1 - 200Hz������С
// 200 - 300������
// 400�
// 500��
// 600�������
// 700�������
// 800�������
// 2730Hz�ʺ����ε�һ��
// 3000���ݶ�, ������
