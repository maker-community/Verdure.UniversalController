


//--------以下为ESP32-S3的【发送端】代码（用于测试 LILYGO® T-Display-S3 AMOLED 开发板）----------------------------------------

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>  // Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

RF24 radio(43, 42); // CE, CSN   频率默认10000000Hz（10MHz）

const byte address[6] = "00001";	 //表示地址的字节数组,任意5个字母的字符串(只要收发端相同)

void setup() {
	radio.begin();                   // 默认使用的FSPI  CS:10, MOSI:11, CLK:12, MISO:13
	radio.openWritingPipe(address);  // 发送端打开通信通道
	radio.setPALevel(RF24_PA_MIN);   // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	radio.stopListening();           // 停止监听，即设置为发送端
}

void loop() {
	const char text[] = "Hello World"; // 一次最多可以传输32个字节。
	radio.write(&text, sizeof(text));
	delay(1000);
}





// ----------------ESP32通用开发板的【接收端】-----------------------------------------------------------------------
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
//
//RF24 radio(33, 32); // CE, CSN   频率默认10000000Hz（10MHz）
//
//const byte address[6] = "00001";
//
//void setup() {
//    Serial.begin(9600);
//    radio.begin();
//    radio.openReadingPipe(0, address);  // 接收端打开通信通道
//    radio.setPALevel(RF24_PA_MIN);      // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
//    radio.startListening();             // 开始监听，即设置为接收端
//    Serial.println("我是接收端");
//}
//
//void loop() {
//    if (radio.available()) {            // 是否有可用的数据（是否收到数据）
//        char text[32] = "";              // 一次最多可以传输32个字节。用于接收的变量类型和大小必须与发来的数据一致
//        radio.read(&text, sizeof(text));
//        Serial.println(text);
//    }
//}