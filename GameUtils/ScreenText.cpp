#include "ScreenText.h"

using namespace std;

ScreenText::ScreenText(string t)
{
	ScreenText::SetText(t);
	SetPos(0,0);
	color = { 255,255,255,255 };
}


ScreenText::ScreenText(string t, int _x, int _y)
{
	//ScreenText( t );
	x = _x;
	y = _y;
}

ScreenText::ScreenText(std::string t, int _x, int _y, RGBA _color)
{
	ScreenText(t, _x,  _y);
	SetColor(_color);
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