#pragma once

#include "TileImage.h"

class BitmapFont
{
	TileImage *font;

public:
	void CreateFontFromRGBA(RGBA *data, int charW, int charH);
};