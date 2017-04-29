#pragma once
#include <string> // memcpy



struct RGB
{
	unsigned char r,g,b;
	unsigned int ConvertToUnsigned()
	{
		return (r << 24) | (g << 16) | (b << 8) | 0xFF;
	}
};
struct RGBA : RGB
{
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
	unsigned int ConvertToUnsigned()
	{
		return (r << 24) | (g << 16) | (b << 8) | a;
	}
};
//since everyone and their momma uses BITMAP, i named my struct PIXMAP so theres no name conflicts witout the use of a namespace
struct PIXMAP
{
	unsigned int w, h;
	RGBA *pixels;
	
	//convert some random data to PIXMAP
	PIXMAP(unsigned int *data, unsigned int _w, unsigned int _h)
	{
		w = _w;
		h = _h;
		pixels = (RGBA*)data;

	}

	PIXMAP(unsigned int _w, unsigned int _h)
	{
		w = _w;
		h = _h;
		pixels = new RGBA[w*h];
		
	}
	~PIXMAP()
	{
		Destroy();
	}
	void Destroy()
	{
		delete[] pixels;
		pixels = 0;
		w = 0;
		h = 0;
	}

	inline unsigned int GetSize()
	{
		return  w*h * sizeof(RGBA);
	}

	void Fill(RGBA color)
	{
		for (unsigned int i = 0; i < w*h; i++)
		{
			pixels[i] = color;
		}
		//memset(pixels, color, w * h * sizeof(RGB));
		//memset(pixels, 255, 640 * 480 * sizeof(Uint32));
	}

	void Blit(PIXMAP * dest, int x, int y)
	{
		int srcX = 0;//,srcY = 0;
		int width = x+w;
		bool hclip = false;
		unsigned int height = y+h;
		
		if ((y + h) > dest->h)
		{
			height = dest->h - (y + h);
		}
		
		if ((x + w) > dest->w)
		{
			width = abs((int)((x + w) - dest->w));
			hclip = true;
		}
		
		for (unsigned int uiV = y; uiV < height; ++uiV)
		{
			// reset coordinate for each row
			RGBA *startPixel = &dest->pixels[uiV * dest->w + x];
			
			if (hclip && uiV > 0)
				srcX = uiV * w;

			memcpy(startPixel, &pixels[srcX], w * sizeof(RGBA));
			srcX += w;
		}
	}

	

	void Clear()
	{
		memset(pixels, 0, GetSize());
	}
};

struct ScreenText
{
	int x, y; 
	RGB color;
	int size;
	const char * text;

	/*void SetText(const char * t, size_t size)
	{
		text = new char[size+1];
		for(size_t i = 0; i < size; i++)
			text[i] = t[i];
		text[size] = '\0';
	}

	~ScreenText()
	{
		if(text)
			delete[] text;
	}*/
};
