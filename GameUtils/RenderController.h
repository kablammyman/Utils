#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>

#include "RenderObject.h"
#include "GraphicsPrimitives.h"
#include "ScreenText.h"
#include "BitmapFont.h"

class RenderController
{
	PIXMAP*screenBuffer;
	int bufferWidth;
	int bufferHeight;
	//to avoid object slicing, the vector is full of pointers to RenderObject and dervied objs
	std::vector<RenderObject *> renderList;
	
	//the textlist was supposed to work like the renderlist. have all thes string you wanted on screen
	//drawn in order, after we are done drawing everything else, so the text wont be drawn over
	//std::vector<ScreenText> textList;

	StockBitmapFont font;
	void KillScreenBuffer();
public:
	void Init(int sw, int sh);
	~RenderController();
	void AddToRenderList(RenderObject *r);
	//void BlitToScreen(bool clearBuffer = true);
	PIXMAP* GetScreenBuffer();
	RenderObject * GetRenderObject(size_t index);
	void UpdateAllRenderObjects();
	void DrawAllRenderObjectsToBuffer();
	void ResizeScreenBuffer(int screenW, int screenH);
	void DrawTextToBuffer(ScreenText text);
};

#endif //RENDERER_H