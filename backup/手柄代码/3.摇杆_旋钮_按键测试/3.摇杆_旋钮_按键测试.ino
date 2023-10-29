

#define PS2_X     1
#define PS2_Y     2
#define SW        46


void setup() {
	// 配置衰减器
    //  analogSetAttenuation(ADC_11db);


	// 配置输入模式
	pinMode(PS2_X, INPUT);
	pinMode(PS2_Y, INPUT);
	pinMode(SW, INPUT_PULLUP);
	pinMode(3, OUTPUT);
	digitalWrite(3, HIGH);
	// 配置串口通信波特率
	Serial.begin(9600);

}

void loop() {
	// 读取数值
	Serial.printf("x: %d, y: %d, z: %d\n", analogRead(PS2_X), analogRead(PS2_Y), digitalRead(SW));

	delay(100);

}
