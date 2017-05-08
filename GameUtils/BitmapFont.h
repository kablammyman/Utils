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
};

class StockBitmapFont : public BitmapFont
{
	RGBA *fontData;
public:
	StockBitmapFont();
	void Draw(PIXMAP *dest, std::string text, int x, int y);
	void Create1bppBin(unsigned char *pngData);
	RGBA *Read1bbpBin();
};

class StockFont
{
public:
	static BitmapFont stockFont;
	static void Draw(PIXMAP *dest, ScreenText *st);
	static void Draw(PIXMAP *dest, std::string st, int x, int y);
};