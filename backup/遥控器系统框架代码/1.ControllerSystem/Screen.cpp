#include "Screen.h"


// 初始化配置
void Screen::init()
{
	rm67162_init();						// 屏幕初始化
	lcd_setRotation(0);					// 0-3  1是横向
	spr.createSprite(240, 536, 2);		// 改方向后这里长款也要同步改  - 2 bytes per pixel for 16 bit color depth (565 RGB format)
	spr.setSwapBytes(1);
	spr.fillSprite(TFT_BLACK);			// 清屏
}



