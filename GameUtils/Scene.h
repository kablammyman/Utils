#ifndef SCENE_H
#define SCENE_H

#include "RenderController.h"
#include "Observee.h"

class Scene : public Observee
{
protected:
	RenderController renderController;
	
public:
	virtual void UpdateScene()
	{
		renderController.UpdateAllRenderObjects();
	}
	virtual void DrawScene()
	{
		renderController.DrawAllRenderObjectsToBuffer();
	}

	//why do these exists?
	//virtual int ChangeSceneToIndex() =0;//return scene index
	//virtual Scene* ChangeScene() =0;//return scene pointer

	void ChangeScreenSize(int screenW, int screenH)
	{
		renderController.ResizeScreenBuffer(screenW, screenH);
	}
	PIXMAP *GetSceneScreenBuffer()
	{
		return renderController.GetScreenBuffer();
	}
	void AddToRenderList(RenderObject *r)
	{
		renderController.AddToRenderList(r);
	}
	void InitRenderController(int sw, int sh)
	{
		renderController.Init(sw,sh);
	}

	void DrawTextOnScene(ScreenText text)
	{
		renderController.DrawTextToBuffer(text);
	}
	void DrawTextOnScene(std::string text, int x, int y)
	{
		renderController.DrawTextToBuffer(text, x, y);
	}
	void ClearScreenBuffer()
	{
		renderController.GetScreenBuffer()->Clear();
	}
};

#endif //SCENE_H