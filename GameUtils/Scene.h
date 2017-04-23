#ifndef SCENE_H
#define SCENE_H

#include "RenderController.h"
#include "Observee.h"

class Scene : public Observee
{
protected:
	RenderController renderer;
	
public:
	virtual void Update() {}
	virtual void Draw() {}
	virtual int ChangeSceneToIndex() {return 0;}//return scene index
	virtual Scene* ChangeScene() {return NULL;}//return scene pointer
	virtual void ChangeScreenSize(int screenW, int screenH)
	{
		renderer.ResizeScreenBuffer(screenW, screenH);
	}
};

#endif //SCENE_H