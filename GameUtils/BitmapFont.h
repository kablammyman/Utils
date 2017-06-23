#pragma once

#include "TileImage.h"
#include "ScreenText.h"

#include <string>

class BitmapFont
{
protected:	
	TileImage *font;
	int charW,charH;
public:
	BitmapFont() {};
	BitmapFont(RGBA *data, int w, int h, int cW, int cH);
	void CalcPixmapLineSize(int numChars, unsigned int &w, unsigned int &h);
	TileImage *GetFont();
	void SetPixelMask(RGBA color)
	{
		font->SetPixelMask(color);
	}
};

class StockBitmapFont : public BitmapFont
{
	RGBA *fontData;
	RGBA *Read1bbpBin(RGBA color);
public:
	StockBitmapFont(RGBA color = RGBA{ 255,255,255,255 });
	//maksed means to draw the background black pixels or not...false = use black pixels
	void Draw(PIXMAP *dest, std::string text, int x, int y,bool masked = false);
	void Create1bppBin(unsigned char *pngData);
	
};

/*class StockFont
{
public:
	static BitmapFont stockFont;
	static void Draw(PIXMAP *dest, ScreenText *st);
	static void Draw(PIXMAP *dest, std::string st, int x, int y);
};*/