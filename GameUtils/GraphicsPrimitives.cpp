#include "GraphicsPrimitives.h"

#include <cstdio>
#include <cstdlib>

PIXMAP::PIXMAP(unsigned int _w, unsigned int _h)
{
	w = _w;
	h = _h;
	pixels = new RGBA[w*h];
}
//---------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------
PIXMAP::PIXMAP(unsigned char *data, unsigned int _w, unsigned int _h, bool rgbaData)
{
	w = _w;
	h = _h;
	int iBytesPerPixel = 4;
	
	if(!rgbaData)
		iBytesPerPixel = 3;

	pixels = new RGBA [w * h];


	unsigned char *pucPixel;
	RGBA *curPixel = pixels;
	unsigned char alpha;

	for (unsigned int uiV = 0; uiV < h; ++uiV)
	{
		// reset coordinate for each row
		unsigned char* pucPixel = &data[uiV * w * iBytesPerPixel];

		// read each row of orig image
		for (unsigned int uiH = 0; uiH < w; ++uiH)
		{
			if(rgbaData)
				alpha = pucPixel[3];
			else alpha = 255;

			*curPixel++ = RGBA(pucPixel[0], pucPixel[1],pucPixel[2], alpha);
			pucPixel += iBytesPerPixel;
		}
	}
}
//---------------------------------------------------------------------------------------
PIXMAP::~PIXMAP()
{
	Destroy();
}
//---------------------------------------------------------------------------------------
void PIXMAP::Destroy()
{
	delete[] pixels;
	pixels = nullptr;
	w = 0;
	h = 0;
}
//---------------------------------------------------------------------------------------
void PIXMAP::Fill(RGBA color)
{
	for (unsigned int i = 0; i < w*h; i++)
	{
		pixels[i] = color;
	}
	//memset(pixels, color, w * h * sizeof(RGBA));
	//memset(pixels, 255, 640 * 480 * sizeof(Uint32));
}
//---------------------------------------------------------------------------------------
void PIXMAP::Blit(PIXMAP * dest, int x, int y)
{
	int srcX = 0;//,srcY = 0;
	
	int totalPixH = (y + h);
	int totalPixW = (x + w);
	
	int vertSPanOfPix = totalPixH;

	int numHorizPixelsToDraw = w;
	//int numVertPixelsToDraw = h;
	bool hclip = false;
	

	if (totalPixH > dest->h)
	{
		//numVertPixelsToDraw = totalPixH - dest->h;
		vertSPanOfPix = dest->h;
	}
	
	if (totalPixW > dest->w)
	{
		numHorizPixelsToDraw = totalPixW - dest->w;
	}

	for (unsigned int uiV = y; uiV < vertSPanOfPix; ++uiV)
	{
		// reset coordinate for each row
		RGBA *startPixel = &dest->pixels[uiV * dest->w + x];

		std::memcpy(startPixel, &pixels[srcX], numHorizPixelsToDraw * sizeof(RGBA));
		srcX += w;
	}
}
//---------------------------------------------------------------------------------------
void PIXMAP::CopyPixels(unsigned char *src, int srcW, int srcH, int x, int y)
{
	RGBA *startPixel;
	for (unsigned int uiV = 0; uiV < srcH; ++uiV)
	{
		// reset coordinate for each row
		unsigned char* curPix = &src[(uiV * srcW)*4];
		startPixel = &pixels[(y+ uiV) * w + x];

		for (unsigned int uiH = 0; uiH <  srcW; ++uiH)
		{
			*startPixel++ = RGBA(curPix[0], curPix[1], curPix[2], curPix[3]);
			curPix += 4;// 4 bytes per pixel
		}
	}
}
//---------------------------------------------------------------------------------------
void PIXMAP::Clear()
{
	std::memset(pixels, 0, GetSize());
}
//---------------------------------------------------------------------------------------
void WriteToTextFile(PIXMAP *screen)
{
	FILE *ptr_file = fopen("example.txt", "w");
	for (int y = 0; y < 480; y++)
	{
		for (int x = 0; x < 640; x++) {
			fprintf(ptr_file, "(%i,%i,%i)", screen->pixels[y * 640 + x].r, screen->pixels[y * 640 + x].g, screen->pixels[y * 640 + x].b);
		}
		fprintf(ptr_file, "\n");
	}
	fclose(ptr_file);
}
