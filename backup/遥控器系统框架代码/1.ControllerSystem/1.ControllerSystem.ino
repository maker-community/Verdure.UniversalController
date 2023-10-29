/*
 Name:		_1.ino
 Created:	2023/9/8 15:15:20
 Author:	bilibili-黑人黑科技
*/

// 包含库
//-------------------------------------------------------------------------------------------------------------
#include "NRF.h"
#include "Keys.h"
#include "Screen.h"
#include "Bluetooth.h"
#include "WIFI.h"
#include "LED.h"
#include "Buzzer.h"
#include "controller_keys.h" // 按键引脚定义

#include "chinese_32.h"  // 中文字库



// 包含图标：一级菜单
#include "icons/1nrf.h"
#include "icons/2game.h"
#include "icons/3vs.h"
#include "icons/4info.h"
#include "icons/5ble.h"
#include "icons/6set.h"

// 包含图标：二级菜单
#include "icons/1nrf/1pickup.h"
#include "icons/1nrf/2truck.h"
#include "icons/1nrf/3tank.h"
#include "icons/1nrf/4drone.h"
#include "icons/1nrf/5excavator.h"
#include "icons/1nrf/6ship.h"
#include "icons/2game/2_1snake.h"
#include "icons/2game/2_2brick.h"
#include "icons/2game/2_3plane.h"
#include "icons/2game/2_4num2048.h"
#include "icons/2game/2_5tetris.h"
#include "icons/3vs/3_1_ball.h"
#include "icons/4info/4_1_bilibili.h"
#include "icons/4info/4_2_weather.h"
#include "icons/4info/4_3_stock.h"
#include "icons/6set/6_1_keysTest.h"
#include "icons/6set/6_2_cube.h"


// 全局变量
//-------------------------------------------------------------------------------------------------------------
int ID = 0;		  // 遥控器ID  0:黑 1:壮(默认0)




// 实例化对象
//-------------------------------------------------------------------------------------------------------------
NRF nrf;		 // 通信模块
Keys keys;	     // 按键
Screen screen;   // 屏幕
Bluetooth bt;	 // 蓝牙
WIFI wifi;	     // WiFi
LED led;		 // 板载LED
Buzzer buzzer;   // 蜂鸣器



// 函数声明
//-------------------------------------------- 一级菜单  -------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void menu();			// 0.主菜单     ->   1.NRF遥控  2.本机游戏  3.双人对战  4.网络信息  5.蓝牙手柄  6.系统设置
void NRFControl();		// 1.NRF遥控    ->   1.皮卡 2.货车 3.坦克 4.无人机 5.挖掘机 6.舰船
void localGame();		// 2.本机游戏   ->   1.贪吃蛇 2.打砖块 3.飞机大战 4.2048 5.俄罗斯方块
void VSGame();			// 3.双人对战   ->   1.弹球
void netInfo();			// 4.网络信息   ->   1.哔哩哔哩 2.天气预报 3.股票基金
void btGamepad();		// 5.蓝牙手柄
void systemSet();		// 6.系统设置   ->   1.按键测试 2.陀螺仪立方体

//--------------------------------------------- 二级菜单 -------------------------------------------------------
//---------------------------------------------1.NRF遥控--------------------------------------------------------
void pickup();			// 1.1 皮卡
void truck();			// 1.2 货车
void tank();			// 1.3 坦克
void drone();			// 1.4 无人机
void excavator();		// 1.5 挖掘机
void ship();			// 1.6 舰船

//---------------------------------------------2.本机游戏-------------------------------------------------------
void snake();			// 2.1 贪吃蛇
void brick();			// 2.2 打砖块
void plane();			// 2.3 飞机大战
void num2048();			// 2.4 2048
void tetris();			// 2.5 俄罗斯方块

//----------------------------------------------3.双人对战------------------------------------------------------
void ball();			// 3.1 弹球

//----------------------------------------------4.网络信息------------------------------------------------------
void bilibili();		// 4.1 哔哩哔哩
void weather();			// 4.2 天气预报
void stock();			// 4.3 股票基金

//----------------------------------------------5.蓝牙手柄------------------------------------------------------

//----------------------------------------------6.系统设置------------------------------------------------------
void keysTest();		// 6.1 按键测试
void cube();			// 6.2 陀螺仪立方体


//----------------------------------------------工具方法------------------------------------------------------
int getVolADC();					// 立刻获取电压ADC值
int getVol();						// 立刻获取电压值（mV）
int setGetVolTimer(int time);		// 设置周期性获取电压值（时间ms），返回定时器的ID
void closeGetVolTimer(int timerID); // 关闭周期性获取电压（定时器ID）



void setup() {
	Serial.begin(9600);

	nrf.init(ID, 0);			 // (遥控器ID, 通信功率，0-3)
	keys.init(ID);
	screen.init();
	buzzer.init();
	led.init();

	// 基本功能测试
	{
		//nrf.testConToCon();		   // 连遥控器间的测试连接 √
		//keysTest();				   // 按键测试,显示kvs到小屏幕上 √
		//keys.dounnceTest();		   // 按键消抖测试 √
	}


	screen.spr.loadFont(chinese_32);   // 加载自定义中文字库

	menu();									// 主菜单
}


void loop() {
	//con.keys.kvs_update();         // √
	//con.keys.ShowInSerial();		 // √	
	delay(1000);
}














// ----------------------------------------- 以下是功能的具体实现 -----------------------------------------------

//-------------------------------------------- 一级菜单  -------------------------------------------------------
// 0.主菜单     ->   1.NRF遥控  2.本机游戏  3.双人对战  4.网络信息  5.蓝牙手柄  6.系统设置
void menu()
{
	bool pushFlag = true;	// 需要刷新屏幕的标志
	int menuIndex = 0;		// 菜单索引
	String menuName[6] = { "NRF遥控", "本机游戏", "双人对战", "网络信息", "蓝牙手柄", "系统设置" };	// 菜单名字

	screen.spr.setTextColor(TFT_WHITE);		// 设置字体颜色
	screen.spr.setTextDatum(TC_DATUM);		// 设置字体对齐方式 TC_DATUM:居中对齐(基准坐标为上方的中点)

	while (true)
	{
		// 左键菜单索引-1，右键+1。并把刷新标志改为真
		if (keys.left.pressed())
		{
			menuIndex--;
			pushFlag = true;
			if (menuIndex < 0) { menuIndex = 5; }
		}
		else if (keys.right.pressed())
		{
			menuIndex++;
			pushFlag = true;
			if (menuIndex > 5) { menuIndex = 0; }
		}

		// 当刷新标准为true时，刷新屏幕显示。完事把标记改为假
		if (pushFlag)
		{
			//screen.spr.fillSprite(TFT_BLACK);		// 清屏
			screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);    // 把字体区域清空
			screen.spr.drawString(menuName[menuIndex], 120, 150, 4);	// 显示菜单名字

			screen.spr.fillSmoothRoundRect(20, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothRoundRect(56, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothRoundRect(92, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothRoundRect(128, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothRoundRect(164, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothRoundRect(200, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);

			if (menuIndex == 0) { screen.spr.pushImage(20, 220, 200, 200, image_data_1nrf);  screen.spr.fillSmoothRoundRect(20, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK); }
			if (menuIndex == 1) { screen.spr.pushImage(20, 220, 200, 200, image_data_2game); screen.spr.fillSmoothRoundRect(56, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK); }
			if (menuIndex == 2) { screen.spr.pushImage(20, 220, 200, 200, image_data_3vs);   screen.spr.fillSmoothRoundRect(92, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK); }
			if (menuIndex == 3) { screen.spr.pushImage(20, 220, 200, 200, image_data_4info); screen.spr.fillSmoothRoundRect(128, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK); }
			if (menuIndex == 4) { screen.spr.pushImage(20, 220, 200, 200, image_data_5ble);  screen.spr.fillSmoothRoundRect(164, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK); }
			if (menuIndex == 5) { screen.spr.pushImage(20, 220, 200, 200, image_data_6set);  screen.spr.fillSmoothRoundRect(200, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK); }

			//Serial.println("push"); //用于验证按一下键才刷新屏幕，而不是一直刷新
			lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());	// 将数据显示到屏幕上
			pushFlag = false;
		}

		// 按下o键，进入二级菜单
		if (keys.o.pressed())
		{
			// 基于menuIndex进入不同的二级菜单
			switch (menuIndex)
			{
			case 0:
				NRFControl();		// 进入NRF遥控菜单
				pushFlag = true;	// 退出二级菜单后，需要刷新主菜单的屏幕
				break;
			case 1:
				localGame();		// 进入本机游戏菜单
				pushFlag = true;
				break;
			case 2:
				VSGame();			// 进入双人对战菜单
				pushFlag = true;
				break;
			case 3:
				netInfo();			// 进入网络信息菜单
				pushFlag = true;
				break;
			case 4:
				btGamepad();		// 进入蓝牙手柄菜单
				pushFlag = true;
				break;
			case 5:
				systemSet();		// 进入系统设置菜单
				pushFlag = true;
				break;
			default:
				break;
			}

		}

		delay(10);
	}

}

// 1.NRF遥控    ->   1.皮卡 2.货车 3.坦克 4.无人机 5.挖掘机 6.舰船
void NRFControl()
{
	bool pushFlag = true;	// 需要刷新屏幕的标志
	int nrfIndex = 0;		// 菜单索引
	String nrfName[6] = { "皮卡", "货车", "坦克", "无人机", "挖掘机", "舰船" };	// 菜单名字

	screen.spr.setTextColor(TFT_WHITE);		// 设置字体颜色
	screen.spr.setTextDatum(TC_DATUM);		// 设置字体对齐方式 TC_DATUM:居中对齐(基准坐标为上方的中点)

	while (true)
	{
		if (keys.left.pressed())
		{
			nrfIndex--;
			pushFlag = true;
			if (nrfIndex < 0) { nrfIndex = 5; }
		}
		else if (keys.right.pressed())
		{
			nrfIndex++;
			pushFlag = true;
			if (nrfIndex > 5) { nrfIndex = 0; }
		}

		if (pushFlag)
		{
			//screen.spr.fillSprite(TFT_BLACK);		// 清屏
			screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);    // 把字体区域清空
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			screen.spr.drawString(nrfName[nrfIndex], 120, 150, 4);	// 显示菜单名字

			screen.spr.fillSmoothCircle(30,  456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(66,  456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(102, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(138, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(174, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(210, 456, 6, TFT_VIOLET, TFT_BLACK);

			if (nrfIndex == 0) { screen.spr.pushImage(20, 220, 200, 200, image_data_1_1pickup);    screen.spr.fillSmoothCircle(30,  456, 6, TFT_CYAN, TFT_BLACK);}
			if (nrfIndex == 1) { screen.spr.pushImage(20, 220, 200, 200, image_data_1_2truck);     screen.spr.fillSmoothCircle(66,  456, 6, TFT_CYAN, TFT_BLACK);}
			if (nrfIndex == 2) { screen.spr.pushImage(20, 220, 200, 200, image_data_1_3tank);      screen.spr.fillSmoothCircle(102, 456, 6, TFT_CYAN, TFT_BLACK);}
			if (nrfIndex == 3) { screen.spr.pushImage(20, 220, 200, 200, image_data_1_4drone);     screen.spr.fillSmoothCircle(138, 456, 6, TFT_CYAN, TFT_BLACK);}
			if (nrfIndex == 4) { screen.spr.pushImage(20, 220, 200, 200, image_data_1_5excavator); screen.spr.fillSmoothCircle(174, 456, 6, TFT_CYAN, TFT_BLACK);}
			if (nrfIndex == 5) { screen.spr.pushImage(20, 220, 200, 200, image_data_1_6ship);      screen.spr.fillSmoothCircle(210, 456, 6, TFT_CYAN, TFT_BLACK);}

			//Serial.println("push"); //用于验证按一下键才刷新屏幕，而不是一直刷新
			lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());	// 将数据显示到屏幕上
			pushFlag = false;
		}

		if (keys.o.pressed())
		{
			switch (nrfIndex)
			{
			case 0:
				pickup();
				pushFlag = true;
				break;
			case 1:
				truck();
				pushFlag = true;
				break;
			case 2:
				tank();
				pushFlag = true;
				break;
			case 3:
				drone();
				pushFlag = true;
				break;
			case 4:
				excavator();
				pushFlag = true;
				break;
			case 5:
				ship();
				pushFlag = true;
				break;
			default:
				break;
			}

		}
		else if (keys.x.pressed())
		{
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			Serial.println("返回主菜单");
			break;
		}


		delay(10);
	}


}

// 2.本机游戏   ->   1.贪吃蛇 2.打砖块 3.飞机大战 4.2048 5.俄罗斯方块
void localGame()
{
	bool gameFlag = true;	// 需要刷新屏幕的标志
	int gameIndex = 0;		// 菜单索引
	String gameName[5] = { "贪吃蛇", "打砖块", "飞机大战", "2048", "俄罗斯方块" };	// 菜单名字

	screen.spr.setTextColor(TFT_WHITE);		// 设置字体颜色
	screen.spr.setTextDatum(TC_DATUM);		// 设置字体对齐方式 TC_DATUM:居中对齐(基准坐标为上方的中点)

	while (true)
	{
		if (keys.left.pressed())
		{
			gameIndex--;
			gameFlag = true;
			if (gameIndex < 0) { gameIndex = 4; }
		}
		else if (keys.right.pressed())
		{
			gameIndex++;
			gameFlag = true;
			if (gameIndex > 4) { gameIndex = 0; }
		}

		if (gameFlag)
		{
			//screen.spr.fillSprite(TFT_BLACK);		// 清屏
			screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);    // 把字体区域清空
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			screen.spr.drawString(gameName[gameIndex], 120, 150, 4);	// 显示菜单名字

			screen.spr.fillSmoothCircle(48, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(84, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(120, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(156, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(192, 456, 6, TFT_VIOLET, TFT_BLACK);


			if (gameIndex == 0) { screen.spr.pushImage(20, 220, 200, 200, image_data_2_1snake);   screen.spr.fillSmoothCircle(48, 456, 6, TFT_CYAN, TFT_BLACK);}
			if (gameIndex == 1) { screen.spr.pushImage(20, 220, 200, 200, image_data_2_2brick);   screen.spr.fillSmoothCircle(84, 456, 6, TFT_CYAN, TFT_BLACK); }
			if (gameIndex == 2) { screen.spr.pushImage(20, 220, 200, 200, image_data_2_3plane);   screen.spr.fillSmoothCircle(120, 456, 6, TFT_CYAN, TFT_BLACK); }
			if (gameIndex == 3) { screen.spr.pushImage(20, 220, 200, 200, image_data_2_4num2048); screen.spr.fillSmoothCircle(156, 456, 6, TFT_CYAN, TFT_BLACK); }
			if (gameIndex == 4) { screen.spr.pushImage(20, 220, 200, 200, image_data_2_5tetris);  screen.spr.fillSmoothCircle(192, 456, 6, TFT_CYAN, TFT_BLACK); }

			lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());	// 将数据显示到屏幕上
			gameFlag = false;
		}

		if (keys.o.pressed())
		{
			switch (gameIndex)
			{
			case 0:
				snake();
				gameFlag = true;
				break;
			case 1:
				brick();
				gameFlag = true;
				break;
			case 2:
				plane();
				gameFlag = true;
				break;
			case 3:
				num2048();
				gameFlag = true;
				break;
			case 4:
				tetris();
				gameFlag = true;
				break;
			default:
				break;
			}

		}
		else if (keys.x.pressed())
		{
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			Serial.println("返回主菜单");
			break;
		}


		delay(10);
	}
}

// 3.双人对战   ->   1.弹球
void VSGame()
{
	bool vsFlag = true;
	int vsIndex = 0;
	String vsName[1] = { "弹球", };

	screen.spr.setTextColor(TFT_WHITE);		// 设置字体颜色
	screen.spr.setTextDatum(TC_DATUM);		// 设置字体对齐方式 TC_DATUM:居中对齐(基准坐标为上方的中点)

	while (true)
	{
		if (keys.left.pressed())
		{
			vsIndex--;
			vsFlag = true;
			if (vsIndex < 0) { vsIndex = 0; }
		}
		else if (keys.right.pressed())
		{
			vsIndex++;
			vsFlag = true;
			if (vsIndex > 0) { vsIndex = 0; }
		}

		if (vsFlag)
		{
			//screen.spr.fillSprite(TFT_BLACK);		// 清屏
			screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);    // 把字体区域清空
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			screen.spr.drawString(vsName[vsIndex], 120, 150, 4);	// 显示菜单名字

			screen.spr.fillSmoothCircle(120, 456, 6, TFT_VIOLET, TFT_BLACK);

			if (vsIndex == 0) { screen.spr.pushImage(20, 220, 200, 200, image_data_3_1_ball);  screen.spr.fillSmoothCircle(120, 456, 6, TFT_CYAN, TFT_BLACK);}
			lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());	// 将数据显示到屏幕上
			vsFlag = false;
		}

		if (keys.o.pressed())
		{
			switch (vsIndex)
			{
			case 0:
				ball();
				vsFlag = true;
				break;
			default:
				break;
			}

		}
		else if (keys.x.pressed())
		{
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			Serial.println("返回主菜单");
			break;
		}


		delay(10);
	}
}

// 4.网络信息   ->   1.哔哩哔哩 2.天气预报 3.股票基金
void netInfo()
{
	bool infoFlag = true;
	int infoIndex = 0;
	String infoName[3] = { "哔哩哔哩", "天气预报", "股票基金" };

	screen.spr.setTextColor(TFT_WHITE);		// 设置字体颜色
	screen.spr.setTextDatum(TC_DATUM);		// 设置字体对齐方式 TC_DATUM:居中对齐(基准坐标为上方的中点)

	while (true)
	{
		if (keys.left.pressed())
		{
			infoIndex--;
			infoFlag = true;
			if (infoIndex < 0) { infoIndex = 2; }
		}
		else if (keys.right.pressed())
		{
			infoIndex++;
			infoFlag = true;
			if (infoIndex > 2) { infoIndex = 0; }
		}

		if (infoFlag)
		{
			//screen.spr.fillSprite(TFT_BLACK);		// 清屏
			screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);    // 把字体区域清空
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			screen.spr.drawString(infoName[infoIndex], 120, 150, 4);	// 显示菜单名字

			screen.spr.fillSmoothCircle(84, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(120, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(156, 456, 6, TFT_VIOLET, TFT_BLACK);

			if (infoIndex == 0) { 
				screen.spr.fillRect(0, 400, 240, 20, TFT_BLACK);  
				screen.spr.pushImage(20, 220, 200, 181, image_data_4_1_bilibili); 
				screen.spr.fillSmoothCircle(84, 456, 6, TFT_CYAN, TFT_BLACK);
			}
			if (infoIndex == 1) { screen.spr.pushImage(20, 220, 200, 200, image_data_4_2_weather); screen.spr.fillSmoothCircle(120, 456, 6, TFT_CYAN, TFT_BLACK); }
			if (infoIndex == 2) { screen.spr.pushImage(20, 220, 200, 200, image_data_4_3_stock); screen.spr.fillSmoothCircle(156, 456, 6, TFT_CYAN, TFT_BLACK);}

			lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());	// 将数据显示到屏幕上
			infoFlag = false;
		}

		if (keys.o.pressed())
		{
			switch (infoIndex)
			{
			case 0:
				bilibili();
				infoFlag = true;
				break;
			case 1:
				weather();
				infoFlag = true;
				break;
			case 2:
				stock();
				infoFlag = true;
				break;
			default:
				break;
			}

		}
		else if (keys.x.pressed())
		{
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			Serial.println("返回主菜单");
			break;
		}


		delay(10);
	}
}

// 5.蓝牙手柄
void btGamepad() {}

// 6.系统设置   ->   1.按键测试 2.陀螺仪立方体
void systemSet()
{
	bool setFlag = true;
	int setIndex = 0;
	String setName[2] = { "按键测试", "陀螺仪立方体" };

	screen.spr.setTextColor(TFT_WHITE);		// 设置字体颜色
	screen.spr.setTextDatum(TC_DATUM);		// 设置字体对齐方式 TC_DATUM:居中对齐(基准坐标为上方的中点)

	while (true)
	{
		if (keys.left.pressed())
		{
			setIndex--;
			setFlag = true;
			if (setIndex < 0) { setIndex = 1; }
		}
		else if (keys.right.pressed())
		{
			setIndex++;
			setFlag = true;
			if (setIndex > 1) { setIndex = 0; }
		}

		if (setFlag)
		{
			//screen.spr.fillSprite(TFT_BLACK);		// 清屏
			screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);    // 把字体区域清空
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			screen.spr.drawString(setName[setIndex], 120, 150, 4);	// 显示菜单名字

			screen.spr.fillSmoothCircle(102, 456, 6, TFT_VIOLET, TFT_BLACK);
			screen.spr.fillSmoothCircle(138, 456, 6, TFT_VIOLET, TFT_BLACK);

			if (setIndex == 0) { screen.spr.pushImage(20, 220, 200, 200, image_data_6_1_keysTest); screen.spr.fillSmoothCircle(102, 456, 6, TFT_CYAN, TFT_BLACK);}
			if (setIndex == 1) { screen.spr.pushImage(20, 220, 200, 200, image_data_6_2_cube);  screen.spr.fillSmoothCircle(138, 456, 6, TFT_CYAN, TFT_BLACK);}

			lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());	// 将数据显示到屏幕上
			setFlag = false;
		}

		if (keys.o.pressed())
		{
			switch (setIndex)
			{
			case 0:
				keysTest();
				setFlag = true;
				break;
			case 1:
				cube();
				setFlag = true;
				break;
			default:
				break;
			}

		}
		else if (keys.x.pressed())
		{
			screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
			Serial.println("返回主菜单");
			break;
		}


		delay(10);
	}
}



//--------------------------------------------- 二级菜单 -------------------------------------------------------
//--------------------------------------------- 1.NRF遥控 -------------------------------------------------------
// 1.1 皮卡	
void pickup() { Serial.println("1.1 皮卡"); }

// 1.2 货车
void truck() { Serial.println("1.2 货车"); }

// 1.3 坦克
void tank() 
{ 
	Serial.println("1.3 坦克"); 

	struct TANK_KVS
	{
		// 4个前端按键
		bool light = 0;
		bool power = 0;
		bool trigger = 0;

		// int8_t 是有符号char型 -128 到 127
		int8_t LX = 0;
		int8_t LY = 0;
		int8_t RX = 0;
		int8_t RY = 0;
	};

	TANK_KVS tank_kvs;

	const byte address[6] = "00001";

	nrf.radio.openWritingPipe(address);  // 发送端打开通信通道
	nrf.radio.setPALevel(RF24_PA_MIN);   // 设置通信功率，参数可选：RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	nrf.radio.stopListening();           // 停止监听，即设置为发送端

	screen.spr.fillSprite(TFT_BLACK);
	screen.spr.setTextDatum(TL_DATUM);   // 字体对齐方式 TL_DATUM:左上对齐(基准坐标为左上角)

	while (true)
	{
		keys.kvs_update();

		tank_kvs.LX = keys.kvs.LX;
		tank_kvs.LY = keys.kvs.LY;
		tank_kvs.RX = keys.kvs.RX;
		tank_kvs.RY = keys.kvs.RY;
		tank_kvs.light = keys.kvs.switch_L1;
		tank_kvs.power = keys.kvs.switch_R2;
		tank_kvs.trigger = keys.kvs.R_up;

		//const char text[] = "Hello World"; // 一次最多可以传输32个字节。
		nrf.radio.write(&tank_kvs, sizeof(tank_kvs));
		delay(20);

		screen.spr.fillSprite(TFT_BLACK);
		screen.spr.drawString("LX: " + String(tank_kvs.LX), 30, 20);
		screen.spr.drawString("LY: " + String(tank_kvs.LY), 30, 60);
		screen.spr.drawString("RX: " + String(tank_kvs.RX), 30, 100);
		screen.spr.drawString("RY: " + String(tank_kvs.RY), 30, 140);
		screen.spr.drawString("light: " + String(tank_kvs.light), 30, 180);
		screen.spr.drawString("power: " + String(tank_kvs.power), 30, 220);
		screen.spr.drawString("trigger: " + String(!tank_kvs.trigger), 30, 260);
		screen.spr.drawString("QUIT: X键", 30, 400);

		lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());	// 将数据显示到屏幕上

		if (keys.x.pressed()) { screen.spr.fillSprite(TFT_BLACK); screen.spr.setTextDatum(TC_DATUM); break; }
	}





}

// 1.4 无人机
void drone() { Serial.println("1.4 无人机"); }

// 1.5 挖掘机
void excavator() { Serial.println("1.5 挖掘机"); }

// 1.6 舰船
void ship() { Serial.println("1.6 舰船"); }


//----------------------------------------------2.本机游戏-----------------------------------------------------
// 2.1 贪吃蛇
void snake() { Serial.println("2.1 贪吃蛇"); }

// 2.2 打砖块
void brick() { Serial.println("2.2 打砖块"); }

// 2.3 飞机大战
void plane() { Serial.println("2.3 飞机大战"); }

// 2.4 2048
void num2048() { Serial.println("2.4 2048"); }

// 2.5 俄罗斯方块
void tetris() { Serial.println("2.5 俄罗斯方块"); }


//---------------------------------------------3.双人对战------------------------------------------------------
// 3.1 弹球
void ball() { Serial.println(" 3.1 弹球"); }


//---------------------------------------------4.网络信息------------------------------------------------------
// 4.1 哔哩哔哩
void bilibili() { Serial.println("4.1 哔哩哔哩"); }

// 4.2 天气预报
void weather() { Serial.println("4.2 天气预报"); }

// 4.3 股票基金
void stock() { Serial.println("4.3 股票基金"); }


//---------------------------------------------5.蓝牙手柄------------------------------------------------------




//---------------------------------------------6.系统设置------------------------------------------------------
// 6.1 按键测试
void keysTest()
{
	// 按键测试的固定UI
	{
		// 清屏并绘制框架
		screen.spr.fillSprite(TFT_BLACK);
		screen.spr.drawString("按键测试", 120, 20, 4);

		// 4个前端按键
		screen.spr.drawSmoothCircle(35, 75, 15, TFT_GREEN, TFT_BLACK);  // 抗锯齿的圆形
		screen.spr.drawSmoothCircle(80, 70, 10, TFT_GREEN, TFT_BLACK);
		screen.spr.drawSmoothCircle(150, 70, 10, TFT_GREEN, TFT_BLACK);
		screen.spr.drawSmoothCircle(205, 75, 15, TFT_GREEN, TFT_BLACK);

		// 电位器旋钮
		screen.spr.drawRect(35, 110, 75, 20, TFT_CYAN);
		screen.spr.drawRect(130, 110, 75, 20, TFT_CYAN);

		// 4个拨杆开关
		screen.spr.drawRect(20, 150, 20, 30, TFT_PINK);
		screen.spr.drawRect(60, 150, 20, 30, TFT_PINK);
		screen.spr.drawRect(160, 150, 20, 30, TFT_PINK);
		screen.spr.drawRect(200, 150, 20, 30, TFT_PINK);

		// 摇杆
		screen.spr.drawRect(10, 200, 100, 100, TFT_CYAN);
		screen.spr.drawRect(130, 200, 100, 100, TFT_CYAN);

		// 板载按键
		screen.spr.drawRect(80, 320, 20, 10, TFT_WHITE);
		screen.spr.drawRect(140, 320, 20, 10, TFT_WHITE);

		// 功能键
		screen.spr.drawRect(40, 360, 20, 20, TFT_SILVER);
		screen.spr.drawRect(40, 400, 20, 20, TFT_SILVER);
		screen.spr.drawRect(10, 380, 20, 20, TFT_SILVER);
		screen.spr.drawRect(70, 380, 20, 20, TFT_SILVER);
		screen.spr.drawRect(180, 360, 20, 20, TFT_SILVER);
		screen.spr.drawRect(180, 400, 20, 20, TFT_SILVER);
		screen.spr.drawRect(150, 380, 20, 20, TFT_SILVER);
		screen.spr.drawRect(210, 380, 20, 20, TFT_SILVER);

		// 陀螺仪
		screen.spr.drawRect(75, 440, 90, 90, TFT_VIOLET);

		// 把内容推到屏幕显示。 改方向后这里长款也要同步改	
		lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
	}

	// 实时显示按键状态
	while (true)
	{
		// 按键状态更新
		keys.kvs_update();

		// 前端4个按键
		keys.kvs.L_up == 0 ? screen.spr.fillSmoothCircle(35, 75, 12, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(35, 75, 12, TFT_BLACK, TFT_BLACK);
		keys.kvs.L_down == 0 ? screen.spr.fillSmoothCircle(80, 70, 7, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(80, 70, 7, TFT_BLACK, TFT_BLACK);
		keys.kvs.R_down == 0 ? screen.spr.fillSmoothCircle(150, 70, 7, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(150, 70, 7, TFT_BLACK, TFT_BLACK);
		keys.kvs.R_up == 0 ? screen.spr.fillSmoothCircle(205, 75, 12, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(205, 75, 12, TFT_BLACK, TFT_BLACK);

		// 电位旋钮
		screen.spr.fillRect(36, 111, 73, 18, TFT_BLACK);
		screen.spr.fillRect(131, 111, 73, 18, TFT_BLACK);
		screen.spr.fillRect(map(keys.kvs.L_knob, -100, 100, 37, 108) - 2, 111, 4, 18, TFT_CYAN);
		screen.spr.fillRect(map(keys.kvs.R_knob, -100, 100, 132, 203) - 2, 111, 4, 18, TFT_CYAN);

		// 4个拨杆开关
		if (keys.kvs.switch_L1 == 1)
		{
			screen.spr.fillRect(21, 151, 18, 13, TFT_PINK);
			screen.spr.fillRect(21, 166, 18, 13, TFT_BLACK);
			buzzer.on();
			led.on();
		}
		else
		{
			screen.spr.fillRect(21, 151, 18, 13, TFT_BLACK);
			screen.spr.fillRect(21, 166, 18, 13, TFT_PINK);
			buzzer.off();
			led.off();
		}

		if (keys.kvs.switch_L2 == 1)
		{
			screen.spr.fillRect(61, 151, 18, 13, TFT_PINK);
			screen.spr.fillRect(61, 166, 18, 13, TFT_BLACK);
		}
		else {
			screen.spr.fillRect(61, 151, 18, 13, TFT_BLACK);
			screen.spr.fillRect(61, 166, 18, 13, TFT_PINK);
		}

		if (keys.kvs.switch_R1 == 1) {
			screen.spr.fillRect(161, 151, 18, 13, TFT_PINK);
			screen.spr.fillRect(161, 166, 18, 13, TFT_BLACK);
		}
		else {
			screen.spr.fillRect(161, 151, 18, 13, TFT_BLACK);
			screen.spr.fillRect(161, 166, 18, 13, TFT_PINK);
		}

		if (keys.kvs.switch_R2 == 1) {
			screen.spr.fillRect(201, 151, 18, 13, TFT_PINK);
			screen.spr.fillRect(201, 166, 18, 13, TFT_BLACK);
		}
		else {
			screen.spr.fillRect(201, 151, 18, 13, TFT_BLACK);
			screen.spr.fillRect(201, 166, 18, 13, TFT_PINK);
		}

		// 摇杆
		screen.spr.fillRect(11, 201, 98, 98, TFT_BLACK);
		screen.spr.fillRect(131, 201, 98, 98, TFT_BLACK);
		screen.spr.fillSmoothCircle(10 + map(keys.kvs.LX, -100, 100, 10, 90), 200 + map(keys.kvs.LY, -100, 100, 10, 90), 8, TFT_CYAN, TFT_BLACK);
		screen.spr.fillSmoothCircle(130 + map(keys.kvs.RX, -100, 100, 10, 90), 200 + map(keys.kvs.RY, -100, 100, 10, 90), 8, TFT_CYAN, TFT_BLACK);

		// 板载按键
		keys.kvs.board_L == 0 ? screen.spr.fillRect(81, 321, 18, 8, TFT_WHITE) : screen.spr.fillRect(81, 321, 18, 8, TFT_BLACK);
		keys.kvs.board_R == 0 ? screen.spr.fillRect(141, 321, 18, 8, TFT_WHITE) : screen.spr.fillRect(141, 321, 18, 8, TFT_BLACK);

		// 功能按键
		keys.kvs.up == 0 ? screen.spr.fillRect(41, 361, 18, 18, TFT_SILVER) : screen.spr.fillRect(41, 361, 18, 18, TFT_BLACK);
		keys.kvs.down == 0 ? screen.spr.fillRect(41, 401, 18, 18, TFT_SILVER) : screen.spr.fillRect(41, 401, 18, 18, TFT_BLACK);
		keys.kvs.left == 0 ? screen.spr.fillRect(11, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(11, 381, 18, 18, TFT_BLACK);
		keys.kvs.right == 0 ? screen.spr.fillRect(71, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(71, 381, 18, 18, TFT_BLACK);
		keys.kvs.x == 0 ? screen.spr.fillRect(181, 361, 18, 18, TFT_SILVER) : screen.spr.fillRect(181, 361, 18, 18, TFT_BLACK);
		keys.kvs.a == 0 ? screen.spr.fillRect(181, 401, 18, 18, TFT_SILVER) : screen.spr.fillRect(181, 401, 18, 18, TFT_BLACK);
		keys.kvs.o == 0 ? screen.spr.fillRect(151, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(151, 381, 18, 18, TFT_BLACK);
		keys.kvs.b == 0 ? screen.spr.fillRect(211, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(211, 381, 18, 18, TFT_BLACK);

		// 陀螺仪
		int angleX = map(keys.kvs.angleX, -100, 100, 446, 524);
		int angleY = map(keys.kvs.angleY, -100, 100, 81, 159);
		screen.spr.fillRect(76, 441, 88, 88, TFT_BLACK);
		screen.spr.fillRect(angleY - 5, angleX - 5, 10, 10, TFT_VIOLET);

		// 把内容推到屏幕显示。 改方向后这里长款也要同步改
		lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());


		// 同时按下4个前端按键，退出本功能
		if (keys.b.pressed() && keys.x.pressed())
		{
			screen.spr.fillSprite(TFT_BLACK);									// 清屏
			lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());  // 把内容推到屏幕显示。 改方向后这里长款也要同步改
			break;
		}

		delay(1);
	}

}


// 6.2 陀螺仪立方体
void cube() { Serial.println("6.2 陀螺仪立方体"); }






//--------------------------------------------- 工具方法 -------------------------------------------------------
// 获取电压ADC值
int getVolADC()
{
	return analogRead(PIN_VOL);
}

// 获取实际电压值（mV）
int getVol()
{
	return analogReadMilliVolts(PIN_VOL);
}

// 设置周期性获取电压值（时间ms），返回定时器的ID
//int setGetVolTimer(int time);		

// 关闭周期性获取电压（定时器ID）
//void closeGetVolTimer(int timerID);







