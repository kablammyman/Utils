#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>

#include "RenderObject.h"



class RenderController
{
	struct ScreenText
	{
		int x, y, color;
		std::string text;
	};

	PIXMAP*screenBuffer;
	int bufferWidth;
	int bufferHeight;
	//to avoid object slicing, the vector is full of pointers to RenderObject and dervied objs
	std::vector<RenderObject *> renderList;
	std::vector<ScreenText> textList;
	void DrawAllRenderObjectsToBuffer();
	void KillScreenBuffer();
public:
	void Init(int sw, int sh);
	~RenderController();
	void AddToRenderList(RenderObject *r);
	//void BlitToScreen(bool clearBuffer = true);
	PIXMAP* GetScreenBuffer();
	RenderObject * GetRenderObject(size_t index);
	void UpdateAllRenderObjects();
	void ResizeScreenBuffer(int screenW, int screenH);
	void DrawText(int x, int y, int color, std::string text);
};

#endif //RENDERER_H