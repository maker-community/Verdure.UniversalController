


#include <TFT_eSPI.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <MCP23017.h>

#include "rm67162.h"
#include "controller_keys.h"


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

MPU6050 mpu6050(Wire);
MCP23017 mcp;

int value_L_up = 1;
int value_L_down = 1;
int value_R_up = 1;
int value_R_down = 1;

int value_L_knob = 0;
int value_LX = 0;
int value_LY = 0;
int value_R_knob = 0;
int value_RX = 0;
int value_RY = 0;

int value_board_L = 1;
int value_board_R = 1;

uint8_t fun_up = 1;
uint8_t fun_down = 1;
uint8_t fun_left = 1;
uint8_t fun_right = 1;
uint8_t fun_o = 1;
uint8_t fun_x = 1;
uint8_t fun_a = 1;
uint8_t fun_b = 1;

uint8_t switch_L1 = 1;
uint8_t switch_L2 = 1;
uint8_t switch_R1 = 1;
uint8_t switch_R2 = 1;

int angleX = 485;
int angleY = 120;


void keys_update();
void keys_test_ui();


void setup()
{
	rm67162_init();						// 屏幕初始化
	lcd_setRotation(0);					// 0-3  1是横向
	sprite.createSprite(240, 536);		// 改方向后这里长款也要同步改
	sprite.setSwapBytes(1);

	Serial.begin(9600);

	Wire.setPins(41, 40);  // 设置I2C引脚（sda, scl）。 一共只有两个I2C资源，另一个【Wire1】
	Wire.begin();

	mpu6050.begin();        // 自动与默认地址通信
	mcp.begin(7);           // 7:0x27, 6:0x26, 5:0x25, 4:0x24, 3:0x23, 2:0x22, 1:0x21, 0:0x20

	mcp.pinMode(MCP_PIN_L1, INPUT);      // 配置扩展板的IO为输入模式（不能用INPUT_PULL!）
	mcp.pinMode(MCP_PIN_L2, INPUT);		 //也可以是输出模式 mcp.pinMode(1, OUTPUT);
	mcp.pinMode(MCP_PIN_R1, INPUT);
	mcp.pinMode(MCP_PIN_R2, INPUT);
	mcp.pinMode(MCP_PIN_UP, INPUT);
	mcp.pinMode(MCP_PIN_DOWN, INPUT);
	mcp.pinMode(MCP_PIN_LEFT, INPUT);
	mcp.pinMode(MCP_PIN_RIGHT, INPUT);
	mcp.pinMode(MCP_PIN_O, INPUT);
	mcp.pinMode(MCP_PIN_X, INPUT);
	mcp.pinMode(MCP_PIN_A, INPUT);
	mcp.pinMode(MCP_PIN_B, INPUT);

	mcp.pullUp(MCP_PIN_L1, LOW);       // 上/下拉需要单独使用pullUp函数（内部上/下拉电阻）
	mcp.pullUp(MCP_PIN_L2, LOW);
	mcp.pullUp(MCP_PIN_R1, LOW);
	mcp.pullUp(MCP_PIN_R2, LOW);
	mcp.pullUp(MCP_PIN_UP, HIGH);
	mcp.pullUp(MCP_PIN_DOWN, HIGH);
	mcp.pullUp(MCP_PIN_LEFT, HIGH);
	mcp.pullUp(MCP_PIN_RIGHT, HIGH);
	mcp.pullUp(MCP_PIN_O, HIGH);
	mcp.pullUp(MCP_PIN_X, HIGH);
	mcp.pullUp(MCP_PIN_A, HIGH);
	mcp.pullUp(MCP_PIN_B, HIGH);

	pinMode(PIN_L_UP, INPUT_PULLUP);  // 46
	pinMode(PIN_L_DOWN, INPUT_PULLUP);
	pinMode(PIN_R_UP, INPUT_PULLUP);
	pinMode(PIN_R_DOWN, INPUT_PULLUP);

	pinMode(PIN_BOARD_L, INPUT_PULLUP);
	pinMode(PIN_BOARD_R, INPUT_PULLUP);

	keys_test_ui();
	//mpu6050.calcGyroOffsets(true);
}





void loop()
{
	keys_update();


	// 前端4个按键
	value_L_up == 0 ? sprite.fillSmoothCircle(35, 75, 12, TFT_GREEN, TFT_BLACK) : sprite.fillSmoothCircle(35, 75, 12, TFT_BLACK, TFT_BLACK);
	value_L_down == 0 ? sprite.fillSmoothCircle(80, 70, 7, TFT_GREEN, TFT_BLACK) : sprite.fillSmoothCircle(80, 70, 7, TFT_BLACK, TFT_BLACK);
	value_R_down == 0 ? sprite.fillSmoothCircle(150, 70, 7, TFT_GREEN, TFT_BLACK) : sprite.fillSmoothCircle(150, 70, 7, TFT_BLACK, TFT_BLACK);
	value_R_up == 0 ? sprite.fillSmoothCircle(205, 75, 12, TFT_GREEN, TFT_BLACK) : sprite.fillSmoothCircle(205, 75, 12, TFT_BLACK, TFT_BLACK);

	// 电位旋钮
	sprite.fillRect(36, 111, 73, 18, TFT_BLACK);
	sprite.fillRect(131, 111, 73, 18, TFT_BLACK);
	sprite.fillRect(value_L_knob - 2, 111, 4, 18, TFT_CYAN);
	sprite.fillRect(value_R_knob - 2, 111, 4, 18, TFT_CYAN);

	// 4个拨杆开关
	if (switch_L1 == 1) {
		sprite.fillRect(21, 151, 18, 13, TFT_PINK);
		sprite.fillRect(21, 166, 18, 13, TFT_BLACK);
	}
	else {
		sprite.fillRect(21, 151, 18, 13, TFT_BLACK);
		sprite.fillRect(21, 166, 18, 13, TFT_PINK);
	}

	if (switch_L2 == 1) {
		sprite.fillRect(61, 151, 18, 13, TFT_PINK);
		sprite.fillRect(61, 166, 18, 13, TFT_BLACK);
	}
	else {
		sprite.fillRect(61, 151, 18, 13, TFT_BLACK);
		sprite.fillRect(61, 166, 18, 13, TFT_PINK);
	}

	if (switch_R1 == 1) {
		sprite.fillRect(161, 151, 18, 13, TFT_PINK);
		sprite.fillRect(161, 166, 18, 13, TFT_BLACK);
	}
	else {
		sprite.fillRect(161, 151, 18, 13, TFT_BLACK);
		sprite.fillRect(161, 166, 18, 13, TFT_PINK);
	}

	if (switch_R2 == 1) {
		sprite.fillRect(201, 151, 18, 13, TFT_PINK);
		sprite.fillRect(201, 166, 18, 13, TFT_BLACK);
	}
	else {
		sprite.fillRect(201, 151, 18, 13, TFT_BLACK);
		sprite.fillRect(201, 166, 18, 13, TFT_PINK);
	}

	// 摇杆
	sprite.fillRect(11, 201, 98, 98, TFT_BLACK);
	sprite.fillRect(131, 201, 98, 98, TFT_BLACK);
	sprite.fillSmoothCircle(10 + value_LX, 200 + value_LY, 8, TFT_CYAN, TFT_BLACK);
	sprite.fillSmoothCircle(130 + value_RX, 200 + value_RY, 8, TFT_CYAN, TFT_BLACK);

	// 板载按键
	value_board_L == 0 ? sprite.fillRect(81, 321, 18, 8, TFT_WHITE) : sprite.fillRect(81, 321, 18, 8, TFT_BLACK);
	value_board_R == 0 ? sprite.fillRect(141, 321, 18, 8, TFT_WHITE) : sprite.fillRect(141, 321, 18, 8, TFT_BLACK);

	// 功能按键
	fun_up == 0 ? sprite.fillRect(41, 361, 18, 18, TFT_SILVER) : sprite.fillRect(41, 361, 18, 18, TFT_BLACK);
	fun_down == 0 ? sprite.fillRect(41, 401, 18, 18, TFT_SILVER) : sprite.fillRect(41, 401, 18, 18, TFT_BLACK);
	fun_left == 0 ? sprite.fillRect(11, 381, 18, 18, TFT_SILVER) : sprite.fillRect(11, 381, 18, 18, TFT_BLACK);
	fun_right == 0 ? sprite.fillRect(71, 381, 18, 18, TFT_SILVER) : sprite.fillRect(71, 381, 18, 18, TFT_BLACK);
	fun_x == 0 ? sprite.fillRect(181, 361, 18, 18, TFT_SILVER) : sprite.fillRect(181, 361, 18, 18, TFT_BLACK);
	fun_a == 0 ? sprite.fillRect(181, 401, 18, 18, TFT_SILVER) : sprite.fillRect(181, 401, 18, 18, TFT_BLACK);
	fun_o == 0 ? sprite.fillRect(151, 381, 18, 18, TFT_SILVER) : sprite.fillRect(151, 381, 18, 18, TFT_BLACK);
	fun_b == 0 ? sprite.fillRect(211, 381, 18, 18, TFT_SILVER) : sprite.fillRect(211, 381, 18, 18, TFT_BLACK);

	// 陀螺仪
	mpu6050.update();
	if (mpu6050.getAngleX() > -45 && mpu6050.getAngleX() < 45) {
		angleX = map(mpu6050.getAngleX(), -45, 45, 446, 524);
	}
	/*else {
		angleX = 485;
	}*/
	if (mpu6050.getAngleY() > -45 && mpu6050.getAngleY() < 45) {
		angleY = map(mpu6050.getAngleY(), -45, 45, 81, 159);
	}
	/*else {
		angleY = 120;
	}*/

	sprite.fillRect(76, 441, 88, 88, TFT_BLACK);
	sprite.fillRect(angleY - 5, angleX - 5, 10, 10, TFT_VIOLET);

	// 把内容推到屏幕显示。 改方向后这里长款也要同步改
	lcd_PushColors(0, 0, 240, 536, (uint16_t*)sprite.getPointer());

	delay(1);
}




// 更新按键
void keys_update()
{
	// 前端4个按键
	value_L_up = digitalRead(PIN_L_UP);
	value_L_down = digitalRead(PIN_L_DOWN);
	value_R_up = digitalRead(PIN_R_UP);
	value_R_down = digitalRead(PIN_R_DOWN);

	// 电位旋钮
	value_L_knob = map(analogRead(PIN_L_KNOB), 0, 4095, 37, 108);
	value_R_knob = map(analogRead(PIN_R_KNOB), 0, 4095, 132, 203);

	// 4个拨杆开关
	switch_L1 = mcp.digitalRead(MCP_PIN_L1);   // 左1 --- 左2 --- 右1 --- 右2
	switch_L2 = mcp.digitalRead(MCP_PIN_L2);
	switch_R1 = mcp.digitalRead(MCP_PIN_R1);
	switch_R2 = mcp.digitalRead(MCP_PIN_R2);

	// 摇杆
	value_LX = (int)map(analogRead(PIN_LX), 0, 4095, 10, 90); //245-4095
	value_LY = (int)map(analogRead(PIN_LY), 0, 4095, 10, 90); //4050-370
	value_RX = (int)map(analogRead(PIN_RX), 4095, 0, 10, 90); //4095-330
	value_RY = (int)map(analogRead(PIN_RY), 4095, 0, 10, 90);  // 200-3940
	//value_LX = analogRead(PIN_LX); //245-3811  2028
	//value_LY = analogRead(PIN_LY); //3934-370  2152
	//value_RX = analogRead(PIN_RX); //3884-330  2107
	//value_RY = analogRead(PIN_RY);  // 200-3736  1968

	// 板载按键
	value_board_L = digitalRead(PIN_BOARD_L);
	value_board_R = digitalRead(PIN_BOARD_R);

	// 功能按键
	fun_up = mcp.digitalRead(MCP_PIN_UP);
	fun_down = mcp.digitalRead(MCP_PIN_DOWN);
	fun_left = mcp.digitalRead(MCP_PIN_LEFT);
	fun_right = mcp.digitalRead(MCP_PIN_RIGHT);
	fun_o = mcp.digitalRead(MCP_PIN_O);
	fun_x = mcp.digitalRead(MCP_PIN_X);
	fun_a = mcp.digitalRead(MCP_PIN_A);
	fun_b = mcp.digitalRead(MCP_PIN_B);
}


// 按键测试的固定UI
void keys_test_ui()
{
	// 清屏并绘制框架
	sprite.fillSprite(TFT_BLACK);
	sprite.drawString("controller test", 40, 20, 4);

	// 4个前端按键
	sprite.drawSmoothCircle(35, 75, 15, TFT_GREEN, TFT_BLACK);  // 抗锯齿的圆形
	sprite.drawSmoothCircle(80, 70, 10, TFT_GREEN, TFT_BLACK);
	sprite.drawSmoothCircle(150, 70, 10, TFT_GREEN, TFT_BLACK);
	sprite.drawSmoothCircle(205, 75, 15, TFT_GREEN, TFT_BLACK);

	// 电位器旋钮
	sprite.drawRect(35, 110, 75, 20, TFT_CYAN);
	//sprite.fillRect(70, 110, 5, 20, TFT_CYAN);
	sprite.drawRect(130, 110, 75, 20, TFT_CYAN);
	//sprite.fillRect(165, 110, 5, 20, TFT_CYAN);

	// 4个拨杆开关
	sprite.drawRect(20, 150, 20, 30, TFT_PINK);
	sprite.drawRect(60, 150, 20, 30, TFT_PINK);
	sprite.drawRect(160, 150, 20, 30, TFT_PINK);
	sprite.drawRect(200, 150, 20, 30, TFT_PINK);

	// 摇杆
	sprite.drawRect(10, 200, 100, 100, TFT_CYAN);
	sprite.drawRect(130, 200, 100, 100, TFT_CYAN);

	// 板载按键
	sprite.drawRect(80, 320, 20, 10, TFT_WHITE);
	sprite.drawRect(140, 320, 20, 10, TFT_WHITE);

	// 功能键
	sprite.drawRect(40, 360, 20, 20, TFT_SILVER);
	sprite.drawRect(40, 400, 20, 20, TFT_SILVER);
	sprite.drawRect(10, 380, 20, 20, TFT_SILVER);
	sprite.drawRect(70, 380, 20, 20, TFT_SILVER);
	sprite.drawRect(180, 360, 20, 20, TFT_SILVER);
	sprite.drawRect(180, 400, 20, 20, TFT_SILVER);
	sprite.drawRect(150, 380, 20, 20, TFT_SILVER);
	sprite.drawRect(210, 380, 20, 20, TFT_SILVER);

	// 陀螺仪
	sprite.drawRect(75, 440, 90, 90, TFT_VIOLET);

	lcd_PushColors(0, 0, 240, 536, (uint16_t*)sprite.getPointer());   // 改方向后这里长款也要同步改
}
