#include "Screen.h"


// ��ʼ������
void Screen::init()
{
	rm67162_init();						// ��Ļ��ʼ��
	lcd_setRotation(0);					// 0-3  1�Ǻ���
	spr.createSprite(240, 536, 2);		// �ķ�������ﳤ��ҲҪͬ����  - 2 bytes per pixel for 16 bit color depth (565 RGB format)
	spr.setSwapBytes(1);
	spr.fillSprite(TFT_BLACK);			// ����
}



