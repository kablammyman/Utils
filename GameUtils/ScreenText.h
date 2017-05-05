#pragma once

#include <string>
#include "TileImage.h"

class ScreenText
{
	int x, y;
	RGBA color;
	int size;
	std::string text;

	ScreenText();
	ScreenText(int _x,int _y,RGBA _color);
	ScreenText(int _x, int _y, RGBA _color,std::string t);

public:
	void SetText(std::string t);
	std::string GetText();
	void SetColor(RGBA _color);
	RGBA GetColor();
	void SetPos(int _x, int _y);
	
};