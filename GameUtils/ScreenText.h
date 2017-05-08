#pragma once

#include "GraphicsPrimitives.h"

#include <string>

class ScreenText
{
	int x, y;
	RGBA color;
	int size;
	std::string text;
public:
	ScreenText(std::string t);
	ScreenText(std::string t,int _x,int _y);
	ScreenText(std::string t,int _x, int _y, RGBA _color);

	void SetText(std::string t);
	std::string GetText();
	void SetColor(RGBA _color);
	RGBA GetColor();

	void SetPos(int _x, int _y);
	int GetPosX() { return x; }
	int GetPosY() { return y; }
	void Draw(PIXMAP *dest);
	
};