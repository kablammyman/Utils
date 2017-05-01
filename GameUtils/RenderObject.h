#pragma once
#include "GraphicsPrimitives.h"

class RenderObject
{
protected:
	int x, y, w, h;
	bool ownedBuffer;
	PIXMAP *bmp;
public:
	RenderObject() {
		x = 0; y = 0;
		w = 0; h = 0;
		bmp = 0;
		ownedBuffer = false;
	}
	RenderObject(int _x, int _y, int _w, int _h)
	{
		bmp = 0;
		Init(_x, _y, _w, _h);
	}
	void Init(int _x, int _y, int _w, int _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;

		if (bmp != 0)
		{
			bmp->Destroy();
		}

		if (w < 1 || h < 1)
		{
			bmp = 0;
			return;
		}
		bmp = new PIXMAP(w,h);
		ownedBuffer = true;
		
	}
	~RenderObject()
	{
		if (bmp)
		{
			bmp->Destroy();
		}
	}
	//rule of 3!
	//the copy ctor is needed since we will use vector.push_back...it copies the ob to its array, and destroys the original
	RenderObject(const RenderObject &obj)
	{
		x = obj.x;
		y = obj.y;
		w = obj.w;
		h = obj.h;
		bmp = new PIXMAP(w , h);

		//*bmp = *obj.bmp;// copy the value
		//blit(obj.bmp, bmp, 0, 0, x, y, w, h); //this copies one PIXMAP to another
	}
	//the assignment is needed since we will use vector.push_back...it copies the ob to its array, and destroys the original
	RenderObject& operator=(RenderObject other) {
		x = other.x;
		y = other.y;
		w = other.w;
		h = other.h;
		bmp = other.bmp;
		return *this;
	}
	PIXMAP* getPIXMAP()
	{
		return bmp;
	}
	void SetPIXMAP(PIXMAP*b)
	{
		if (bmp == 0)
		{
			bmp = b;
			ownedBuffer = false;
		}
	}
	virtual void Draw(PIXMAP *dest)
	{
		//blit(bmp, dest, 0, 0, x, y, w, h);
		//stretch_blit(bmp, dest, 0, 0, bmp->w, bmp->h,dest->w-200,30, 300, 40);
		bmp->Blit(dest,x,y);
	}
	void SetX(int _x)
	{
		x = _x;
	}
	void SetY(int _y)
	{
		y = _y;
	}
	void SetPos(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	virtual void Update() {}
};
