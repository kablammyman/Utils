#include "BitmapFont.h"

void BitmapFont::CreateFontFromRGBA(RGBA *data, int charW, int charH)
{
	//256x320 is img size for stock allegro font
	font = new TileImage(data, 257,321, charW, charH);
	font->ConvertPixelArrayIntoBlocks();
}

