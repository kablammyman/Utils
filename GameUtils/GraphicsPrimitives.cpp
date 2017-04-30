#include "GraphicsPrimitives.h"



PIXMAP::PIXMAP(unsigned int _w, unsigned int _h)
{
	w = _w;
	h = _h;
	pixels = new RGBA[w*h];
}

PIXMAP::PIXMAP(unsigned int *data, unsigned int _w, unsigned int _h)
{
	w = _w;
	h = _h;
	pixels = new RGBA[w * h];
	int iBytesPerPixel = 4;

	RGBA *curPixel = pixels;


	for (unsigned int uiV = 0; uiV < h; ++uiV)
	{
		// reset coordinate for each row
		unsigned int* pucPixel = &data[uiV * w];

		// read each row of orig image
		for (unsigned int uiH = 0; uiH < w; ++uiH)
		{
			*curPixel++ = RGBA(*pucPixel);
			pucPixel ++;
		}
	}
}

PIXMAP::PIXMAP(unsigned char *data, unsigned int _w, unsigned int _h)
{
	w = _w;
	h = _h;
	int iBytesPerPixel = 4;

	pixels = new RGBA [w * h];


	unsigned char *pucPixel;
	RGBA *curPixel = pixels;


	for (unsigned int uiV = 0; uiV < h; ++uiV)
	{
		// reset coordinate for each row
		unsigned char* pucPixel = &data[uiV * w * iBytesPerPixel];

		// read each row of orig image
		for (unsigned int uiH = 0; uiH < w; ++uiH)
		{
			*curPixel++ = RGBA(pucPixel[0], pucPixel[1],pucPixel[2], pucPixel[3]);
			pucPixel += iBytesPerPixel;
		}
	}
}

PIXMAP::~PIXMAP()
{
	Destroy();
}
void PIXMAP::Destroy()
{
	delete[] pixels;
	pixels = 0;
	w = 0;
	h = 0;
}

void PIXMAP::Fill(RGBA color)
{
	for (unsigned int i = 0; i < w*h; i++)
	{
		pixels[i] = color;
	}
	//memset(pixels, color, w * h * sizeof(RGBA));
	//memset(pixels, 255, 640 * 480 * sizeof(Uint32));
}

void PIXMAP::Blit(PIXMAP * dest, int x, int y)
{
	int srcX = 0;//,srcY = 0;
	int width = x + w;
	bool hclip = false;
	unsigned int height = y + h;

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



void PIXMAP::Clear()
{
	memset(pixels, 0, GetSize());
}