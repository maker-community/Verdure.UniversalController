#pragma once

#include "controller_keys.h"
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>




class NRF {
public:

	// ���캯��������CE��CSN����,ͬʱ��SPIָ�빹��RF24����
	NRF(int cePin=43, int csnPin=42);

	// ��ʼ��, ����ң������ţ��������߹��ʣ��͵��ߣ�0-3   0��-16dBm 1��-12dBm 2��-6dBm 3��0dBm
	void init(int conID, uint8_t PALevel=0);

	// ����ң������ͨ�� ���Ҳ���ذ���
	void testConToCon();
	
	// RF24����
	RF24 radio;

private:


	
	// ң�������
	int _conID;  

	// ��ַ�б�
	byte _addresses[2][6] = { "00000", "00001" };  // Ϊ˫��ͨ�Ŵ��������ܵ����ַ

};


