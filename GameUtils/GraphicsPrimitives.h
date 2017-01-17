#pragma once

struct RGB
{
	unsigned char r,g,b;
};
struct RGBA : RGB
{
	unsigned char a;
};
//since everyone and their momma uses BITMAP, i named my struct PIXMAP so theres no name conflicts witout the use of a namespace
struct PIXMAP
{
	unsigned int w, h;
	RGB *pixels;
	
	PIXMAP(unsigned int _w, unsigned int _h)
	{
		w = _w;
		h = _h;
		pixels = new RGB[w*h];
		
	}
	~PIXMAP()
	{
		Destroy();
	}
	void Destroy()
	{
		delete[] pixels;
		pixels = NULL;
		w = 0;
		h = 0;
	}

	inline unsigned int GetSize()
	{
		return  w*h * sizeof(RGB);
	}
	void Blit(PIXMAP * dest)
	{
		memcpy(dest->pixels, pixels, GetSize());
	}
	void Blit(PIXMAP * dest, int x, int y)
	{
		
		memcpy(dest->pixels, pixels, GetSize());
	}
	void Clear()
	{
		memset(pixels, 0, GetSize());
	}
};

