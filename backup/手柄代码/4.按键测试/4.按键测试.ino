

#include <Wire.h>
#include <MPU6050_tockn.h>
#include <MCP23017.h>

#include "controller_keys.h"


MPU6050 mpu6050(Wire);
MCP23017 mcp;


int value_L_up = 1;
int value_L_down = 1;
int value_R_up = 1;
int value_R_down = 1;

int value_L_knob = 0;
int value_R_knob = 0;
int value_LX = 0;
int value_LY = 0;
int value_RX = 0;
int value_RY = 0;

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



void keys_update();


void setup() {

	Serial.begin(9600);

	Wire.setPins(41, 40);  // 设置I2C引脚（sda, scl）。 一共只有两个I2C资源，另一个【Wire1】
	Wire.begin();

	mpu6050.begin();

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

}


void loop() {
	keys_update();
	mpu6050.update();

	Serial.print("摇杆:");
	Serial.print("\tLX:");
	Serial.print(value_LX);
	Serial.print("\tLY:");
	Serial.print(value_LY);
	Serial.print("\tRX:");
	Serial.print(value_RX);
	Serial.print("\tRY:");
	Serial.print(value_RY);

	Serial.print("\t\t4个前端按键:");
	Serial.print(" L上:");
	Serial.print(value_L_up);
	Serial.print(" L下:");
	Serial.print(value_L_down);
	Serial.print(" R下:");
	Serial.print(value_R_down);
	Serial.print(" R上:");
	Serial.print(value_R_up);

	Serial.print("\t4个拨杆:");
	Serial.print(" 左1:");
	Serial.print(switch_L1);
	Serial.print(" 左2:");
	Serial.print(switch_L2);
	Serial.print(" 右1:");
	Serial.print(switch_R1);
	Serial.print(" 右2:");
	Serial.print(switch_R2);

	Serial.print("\t功能键:");
	Serial.print(" 上:");
	Serial.print(fun_up);
	Serial.print(" 下:");
	Serial.print(fun_down);
	Serial.print(" 左:");
	Serial.print(fun_left);
	Serial.print(" 右:");
	Serial.print(fun_right);
	Serial.print(" O:");
	Serial.print(fun_o);
	Serial.print(" X:");
	Serial.print(fun_x);
	Serial.print(" A:");
	Serial.print(fun_a);
	Serial.print(" B:");
	Serial.print(fun_b);

	Serial.print("\t\t陀螺仪 : ");
	Serial.print(" angleX: ");
	Serial.print(mpu6050.getAngleX());
	Serial.print(" angleY : ");
	Serial.println(mpu6050.getAngleY());

	delay(50);
}




void keys_update()
{
	// 前端4个按键
	value_L_up = digitalRead(PIN_L_UP);
	value_L_down = digitalRead(PIN_L_DOWN);
	value_R_up = digitalRead(PIN_R_UP);
	value_R_down = digitalRead(PIN_R_DOWN);

	// 电位旋钮
	value_L_knob = analogRead(PIN_L_KNOB);
	value_R_knob = analogRead(PIN_R_KNOB);

	// 摇杆
	value_LX = map(analogRead(PIN_LX), 0, 4095, -100, 100); //245-4095
	value_LY = map(analogRead(PIN_LY), 4095, 0, -100, 100); //4050-370
	value_RX = map(analogRead(PIN_RX), 4095, 0, -100, 100); //4095-330
	value_RY = map(analogRead(PIN_RY), 0, 4095, -100, 100);  // 200-3940
	//value_LX = analogRead(PIN_LX); //245-3811  2028
	//value_LY = analogRead(PIN_LY); //3934-370  2152
	//value_RX = analogRead(PIN_RX); //3884-330  2107
	//value_RY = analogRead(PIN_RY);  // 200-3736  1968

	// 功能按键
	fun_up = mcp.digitalRead(MCP_PIN_UP);   
	fun_down = mcp.digitalRead(MCP_PIN_DOWN);  
	fun_left = mcp.digitalRead(MCP_PIN_LEFT);   
	fun_right = mcp.digitalRead(MCP_PIN_RIGHT); 
	fun_o = mcp.digitalRead(MCP_PIN_O);
	fun_x = mcp.digitalRead(MCP_PIN_X);
	fun_a = mcp.digitalRead(MCP_PIN_A);
	fun_b = mcp.digitalRead(MCP_PIN_B);

	// 4个拨杆开关
	switch_L1 = mcp.digitalRead(MCP_PIN_L1);   // 左1 --- 左2 --- 右1 --- 右2
	switch_L2 = mcp.digitalRead(MCP_PIN_L2);
	switch_R1 = mcp.digitalRead(MCP_PIN_R1);
	switch_R2 = mcp.digitalRead(MCP_PIN_R2);
}