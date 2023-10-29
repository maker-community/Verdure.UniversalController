#pragma once

#include <TFT_eSPI.h>
#include "rm67162.h"



class Screen
{
public:

	TFT_eSPI tft = TFT_eSPI();
	TFT_eSprite spr = TFT_eSprite(&tft);

	// ≥ı ºªØ≈‰÷√
	void init();
};



