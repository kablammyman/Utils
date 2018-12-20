#pragma once
#include <cstring> // memcpy


#ifdef RGB
#undef RGB
#endif
struct RGB
{
	unsigned char r,g,b;
	unsigned int ConvertToUnsignedInt()
	{
		return (r << 24) | (g << 16) | (b << 8) | 0xFF;
	}
	RGB()
	{
		r =0;
		g = 0;
		b = 0;
	}
	RGB(unsigned int n)
	{
		r = (n >> 24) & 0xFF;
		g = (n >> 16) & 0xFF;
		b = (n >> 8) & 0xFF;
	}
	bool operator==(const RGB&  other)
	{
		if (r == other.r && g == other.g && b == other.b)
			return true;
		return false;
	}
};

struct RGBA 
{
	unsigned char r, g, b;
	unsigned char a;
	RGBA(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	RGBA()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
	RGBA(RGB rgb)
	{
		r = rgb.r;
		g = rgb.g;
		b = rgb.b;
		a = 255;
	}

	RGBA(unsigned int n)
	{
		r = (n >> 24) & 0xFF;
		g = (n >> 16) & 0xFF;
		b = (n >> 8) & 0xFF;
		a = n & 0xFF;
	}
	unsigned int ConvertToUnsignedInt()
	{
		return (r << 24) | (g << 16) | (b << 8) | a;
	}

	bool operator==(const RGBA&  other)
	{
		if(r == other.r && g == other.g && b == other.b && a == other.a)
			return true;
		return false;
	}
	void operator=(const RGBA&  other)
	{
		r = other.r; 
		g = other.g;
		b = other.b; 
		a = other.a;
	}
};
//since everyone and their momma uses BITMAP, i named my struct PIXMAP so theres no name conflicts witout the use of a namespace
struct PIXMAP
{
	unsigned int w, h;
	RGBA *pixels;
	//convert some random data to PIXMAP
	PIXMAP(unsigned int *data, unsigned int _w, unsigned int _h);
	PIXMAP(unsigned char *data, unsigned int _w, unsigned int _h, bool rgbaData = true);
	PIXMAP(unsigned int _w, unsigned int _h);
	~PIXMAP();
	void Destroy();
	inline unsigned int GetSize()
	{
		return  w*h * sizeof(RGBA);
	}
	inline unsigned int GetNumPixels() { return w * h; }
	void Fill(RGBA color);
	void Clip(unsigned int width, unsigned int height, int destX, int destY, unsigned int destW, unsigned int destH, int & numHorizPixelsToDraw, int & vertSPanOfPix);
	void Blit(PIXMAP * dest, int x, int y);
	void Clear();
	void PutPixel(RGBA pix,int x, int y);
	RGBA GetPixel(int x, int y);
	void DrawRect(int _x,int _y,int _w,int _h,RGBA color);
	//takes unsigned char data (pixels) and adds them to the pixmap
	void CopyPixels(unsigned char *src, int srcW, int srcH, int x, int y);
	void CopyPixels(RGBA *src, int srcW, int srcH, int x, int y);
	unsigned char * GetRawPixelCopy();
	PIXMAP* GetScaledCopy(unsigned int Width, unsigned int Height);
	void Scale(unsigned int Width, unsigned int Height);
	void DrawScaledCopy(PIXMAP *dest, int xPos, int yPos, unsigned  int Width, unsigned int Height);
	//specailized scaled method...this was born by trying to get this to work right when part of the image is clipped off the screen
	//this will eventually be moved to the screen saver itself, but for now, its here
	void DrawScaledCopyForScreenSaver(PIXMAP *dest, int xPos, int yPos, unsigned int Width, unsigned int Height);
};
