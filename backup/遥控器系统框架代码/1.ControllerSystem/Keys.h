#pragma once

#include <Arduino.h>
#include "controller_keys.h"

#include <Wire.h>		    // I2C��
#include <MPU6050_tockn.h>  // �����ǿ�
#include <MCP23017.h>       // IO��չ��
#include <Bounce2.h>

// ��ֵ�ṹ�嶨��
struct KVS
{
	// 4��ǰ�˰���
	bool L_up = 1;
	bool L_down = 1;
	bool R_up = 1;
	bool R_down = 1;

	// ��λ����ҡ��  ------------------ int8_t ���з���char�� -128 �� 127
	int8_t L_knob = 0;
	int8_t LX = 0;
	int8_t LY = 0;
	int8_t R_knob = 0;
	int8_t RX = 0;
	int8_t RY = 0;

	// 2�����ذ���
	bool board_L = 1;
	bool board_R = 1;

	// 8�����ܰ���
	bool up = 1;
	bool down = 1;
	bool left = 1;
	bool right = 1;
	bool o = 1;
	bool x = 1;
	bool a = 1;
	bool b = 1;

	// 4�����˿���
	bool switch_L1 = 1;
	bool switch_L2 = 1;
	bool switch_R1 = 1;
	bool switch_R2 = 1;

	// ����������
	int8_t angleX = 0;  // -128 �� 127
	int8_t angleY = 0;
};

class MCP_bounce
{
public:
	MCP_bounce(uint8_t pin, bool default_state);   // ���캯��������IO�ں�Ĭ��״̬
	bool pressed();     // ����
	bool read();        // ��ȡ����״̬
	bool on();          // �� ����read() != _default_state;�����ڲ��˿��أ�on()��off()��ֱ��
	bool off();         // �ر� ����read() != _default_state;�����ڲ��˿��أ�on()��off()��ֱ��

private:
	uint8_t _pin;
	bool _default_state;
	bool _isPressed_flag = false;
};




class Keys
{
public:
	// MCP IO��չ�����������󣬷���Ϊ��д
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

	// Bounce2�ⰴ������
	Bounce2::Button L_up;
	Bounce2::Button L_down;
	Bounce2::Button R_up;
	Bounce2::Button R_down;
	Bounce2::Button board_L;
	Bounce2::Button board_R;

	MPU6050 mpu6050;
	
	// ��ֵ�ṹ��
	KVS kvs;

	// ���캯���г�ʼ�� MPU6050 ����,	��MCP23017��������	
	Keys();

	// ��ʼ������
	void init(int conID);

	// �������£���kvs�ṹ��
	void kvs_update();

	// ���ڴ�ӡ��������
	void ShowInSerial();

	// �������ԣ��������Ч�������������¡��ͷ�
	void dounnceTest();

	// �����������£�ǰ��4������+����2������
	void bounce_update();

private:

	// ң�������
	int _conID = 0;

	// ҡ�˾�����λֵ
	int _zero = 15;
};

