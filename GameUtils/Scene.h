#ifndef SCENE_H
#define SCENE_H

#include "RenderController.h"

class Scene
{
protected:
	RenderController renderer;
	
public:
	virtual void Update() {}
	virtual void Draw() {}
	 
	virtual void ChangeScreenSize(int screenW, int screenH)
	{
		renderer.ResizeScreenBuffer(screenW, screenH);
	}
};

#endif //SCENE_H