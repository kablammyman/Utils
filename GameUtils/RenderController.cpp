#include "RenderController.h"


void RenderController::Init(int sw, int sh)
{
	bufferWidth = sw;
	bufferHeight = sh;

	screenBuffer = new PIXMAP(bufferWidth , bufferHeight);
	screenBuffer->Clear();
}
//---------------------------------------------------------------------------------------
RenderController::~RenderController()
{
	//since we have a list of pointers, we have to delete those items ourslves. smart pointers would avoid this as extra step
	for(size_t i = 0; i < renderList.size(); i++)
		delete renderList[i];

	renderList.clear();
	KillScreenBuffer();
}
//---------------------------------------------------------------------------------------
void RenderController::KillScreenBuffer()
{
	if (screenBuffer)
	{
		screenBuffer->Destroy();
		screenBuffer = 0;
	}
}
//---------------------------------------------------------------------------------------
void RenderController::AddToRenderList(RenderObject *r)
{
	renderList.push_back(r);
}
//---------------------------------------------------------------------------------------
void RenderController::DrawAllRenderObjectsToBuffer()
{
	for (size_t i = 0; i < renderList.size(); i++)
		renderList[i]->Draw(screenBuffer);
	
	//for (size_t i = 0; i < textList.size(); i++)
	//	textprintf_ex(screenBuffer, font, textList[i].x, textList[i].y, textList[i].color, 0, "%s", textList[i].text.c_str());

	textList.clear();
}
//---------------------------------------------------------------------------------------
PIXMAP *RenderController::GetScreenBuffer()
{
	return screenBuffer;
}
//---------------------------------------------------------------------------------------
/*void RenderController::BlitToScreen(bool clearBuffer)
{
	if(clearBuffer)
		clear(screenBuffer);
	
	drawAllRenderObjectsToBuffer();
	
	blit(screenBuffer, screen, 0, 0, 0, 0, bufferWidth, bufferHeight); //blit finial product to the screen, makes it slow but smooth
}*/
//---------------------------------------------------------------------------------------
RenderObject * RenderController::GetRenderObject(size_t index)
{
    return renderList[index];
}
//---------------------------------------------------------------------------------------
void RenderController::UpdateAllRenderObjects()
{
	for(size_t i = 0; i < renderList.size(); i++)
	renderList[i]->Update();
}
//---------------------------------------------------------------------------------------
void RenderController::ResizeScreenBuffer(int screenW, int screenH)
{
	KillScreenBuffer();
	Init(screenW, screenH);
	DrawAllRenderObjectsToBuffer();
}

void RenderController::DrawText(ScreenText text)
{
	textList.push_back(text);
}

/*
void clearScreen()
{
	if(bmp)
	{
		destroy_PIXMAP(bmp);
		bmp = 0;
	}
	clear(screen);
}

*/