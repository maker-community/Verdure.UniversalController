#pragma once
#include "controller_keys.h"
#include <Arduino.h>

class Buzzer
{
public:
	// ��ʼ��
	void init();

	// ����������
	void on();

	// ֹͣ����
	void off();

	// �ı�Ƶ��
	void changeFreq(uint32_t freq);

	// �ı�Ƶ�ʲ��� (ͬʱ��ǰ�������ϲఴ���˳�)
	void changeFreqTest();
};

