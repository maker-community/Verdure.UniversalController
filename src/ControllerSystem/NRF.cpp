#include "NRF.h"


// 构造函数，传入CE和CSN引脚,同时用SPI指针构造RF24对象
NRF::NRF(int cePin, int csnPin) : radio(cePin, csnPin) 
{
	SPIClass* nrf_spi = new SPIClass(1);   // 创建SPI指针（共有两个SPI资源, 0-> QSPI 和 1-> FSPI ）
	nrf_spi->begin(12, 13, 11);		       // 定义FSPI的引脚（CS:10, MOSI:11, CLK:12, MISO:13）
	radio.begin(nrf_spi);                  // 默认使用的FSPI  CS:10, MOSI:11, CLK:12, MISO:13
}


// 初始化, 传入遥控器编号，设置天线功率，低到高：0-3   0：-16dBm 1：-12dBm 2：-6dBm 3：0dBm
void NRF::init(int conID, uint8_t PALevel)
{
	_conID = conID;
	radio.openWritingPipe(_addresses[_conID]);      // 主机打开发送通道 00000 （和从机接收通道相同）
	radio.openReadingPipe(1, _addresses[!_conID]);   // 主机打开接收通道 00001 （和从机发送通道相同）
	radio.setPALevel(PALevel);            // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
}



// 测试遥控器间通信
void NRF::testConToCon(){
	int _s = 0;  // 存要发送的数据
	int _r = 0;  // 存收到的数据

	while (true){

		delay(20);
		radio.startListening();								 // 设置成接收模式
		if (radio.available()) {							 // 若有可以接收的数
			radio.read(&_r, sizeof(_r));                     // 把读到的数据存入_r中
		}

		digitalWrite(PIN_LED, _r); 						     // 用收到的数据控制LED
		//Serial.print("buttonState: ");
		//Serial.println(_r);

		delay(20);
		radio.stopListening();								 // 设置成发送模式
		_s = digitalRead(PIN_BOARD_R);						 // 读按键状态
		radio.write(&_s, sizeof(_s));						 // 发送出去（发给init函数中配置的地址）

		if (digitalRead(PIN_L_UP) == 0 && digitalRead(PIN_R_UP) == 0){		// 同时按下前端按键中，左上和右上，退出测试
			break;
		}
	}
}


