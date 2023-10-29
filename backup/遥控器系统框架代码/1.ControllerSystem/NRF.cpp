#include "NRF.h"


// ���캯��������CE��CSN����,ͬʱ��SPIָ�빹��RF24����
NRF::NRF(int cePin, int csnPin) : radio(cePin, csnPin) 
{
	SPIClass* nrf_spi = new SPIClass(1);   // ����SPIָ�루��������SPI��Դ, 0-> QSPI �� 1-> FSPI ��
	nrf_spi->begin(12, 13, 11);		       // ����FSPI�����ţ�CS:10, MOSI:11, CLK:12, MISO:13��
	radio.begin(nrf_spi);                  // Ĭ��ʹ�õ�FSPI  CS:10, MOSI:11, CLK:12, MISO:13
}


// ��ʼ��, ����ң������ţ��������߹��ʣ��͵��ߣ�0-3   0��-16dBm 1��-12dBm 2��-6dBm 3��0dBm
void NRF::init(int conID, uint8_t PALevel)
{
	_conID = conID;
	radio.openWritingPipe(_addresses[_conID]);      // �����򿪷���ͨ�� 00000 ���ʹӻ�����ͨ����ͬ��
	radio.openReadingPipe(1, _addresses[!_conID]);   // �����򿪽���ͨ�� 00001 ���ʹӻ�����ͨ����ͬ��
	radio.setPALevel(PALevel);            // ����ͨ�Ź��ʣ�������ѡ��RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
}



// ����ң������ͨ��
void NRF::testConToCon(){
	int _s = 0;  // ��Ҫ���͵�����
	int _r = 0;  // ���յ�������

	while (true){

		delay(20);
		radio.startListening();								 // ���óɽ���ģʽ
		if (radio.available()) {							 // ���п��Խ��յ���
			radio.read(&_r, sizeof(_r));                     // �Ѷ��������ݴ���_r��
		}

		digitalWrite(PIN_LED, _r); 						     // ���յ������ݿ���LED
		//Serial.print("buttonState: ");
		//Serial.println(_r);

		delay(20);
		radio.stopListening();								 // ���óɷ���ģʽ
		_s = digitalRead(PIN_BOARD_R);						 // ������״̬
		radio.write(&_s, sizeof(_s));						 // ���ͳ�ȥ������init���������õĵ�ַ��

		if (digitalRead(PIN_L_UP) == 0 && digitalRead(PIN_R_UP) == 0){		// ͬʱ����ǰ�˰����У����Ϻ����ϣ��˳�����
			break;
		}
	}
}


