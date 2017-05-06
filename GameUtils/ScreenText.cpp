#include "ScreenText.h"

using namespace std;

ScreenText::ScreenText()
{
	SetPos(0,0);
	color = { 255,255,255,255 };
	text = "";
}


ScreenText::ScreenText(int _x, int _y, RGBA _color)
{
	ScreenText();
	SetColor(_color);
	x = _x;
	y = _y;
}

ScreenText::ScreenText(int _x, int _y, RGBA _color, std::string t)
{
	ScreenText(_x, _y, _color);
	SetText(t);
}
void ScreenText::SetPos(int _x, int _y)
{
	//add checks to amek sure its not off biotmap later
	x = _x;
	y = _y;
}
void ScreenText::SetText(string t)
{
	text = t;
}

std::string ScreenText::GetText()
{
	return text;
}
void ScreenText::SetColor(RGBA _color)
{
	color = _color;
}
RGBA ScreenText::GetColor()
{
	return color;
}

void ScreenText::Draw(PIXMAP*dest)
{

}