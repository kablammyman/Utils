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
void  PIXMAP::PutPixel(RGBA pix, int x, int y)
{
	pixels[y * w + x] = pix;
}
//---------------------------------------------------------------------------------------
RGBA  PIXMAP::GetPixel(int x, int y)
{
	return pixels[y * w + x];
}
//---------------------------------------------------------------------------------------
void PIXMAP::DrawRect(int _x, int _y, int _w, int _h,RGBA color)
{
	//top and bottom h line
	for (int i = _x; i < _x + _w; i++)
	{
		PutPixel(color, i, _y);
		PutPixel(color, i, _y+_h);
	}
	
	//left and right v lines
	for (int i = _y; i < _y + _h; i++)
	{
		PutPixel(color, _x, i);
		PutPixel(color, _x+_w, i);
	}
}
//---------------------------------------------------------------------------------------
void PIXMAP::Clip(unsigned int width, unsigned int height,  int destX,  int destY, unsigned int destW, unsigned int destH, int & numHorizPixelsToDraw, int & vertSPanOfPix)
{
	numHorizPixelsToDraw = width;
	vertSPanOfPix = height;

	//the bmp is easily inside the parent bmp
	if (destX >= 0 && destY >= 0 && destX + width > 0 && destY + height > 0)
	{
		
		return;
	}

	//if (destX < 0 || destY < 0 || destX + width > destW || destY + height > destH)
	//first lets deal with the X side off the left side of the screen/bmp
	if (destX < 0)
	{
		numHorizPixelsToDraw = (destX + width);//this should give me the number of visible pixels
	}
	//now lets look at if the pixesl will go off teh right side of screen/bmp
	else if(destX+width > destW)
	{
		numHorizPixelsToDraw = (width - destX);
	}

	//now look at the Y diminsion
	if (destY < 0)
	{
		vertSPanOfPix = (destY + height);//this should give me the number of visible pixels
	}
	//now lets look at if the pixesl will go off teh right side of screen/bmp
	else if(destX+width > destW)
	{
		numHorizPixelsToDraw = (height - destY);
	}
}
//---------------------------------------------------------------------------------------
void PIXMAP::Blit(PIXMAP * dest, int _x, int _y)
{
	
	int srcX = 0;
	//int srcY = 0;
	
	int vertSPanOfPix;
	int numHorizPixelsToDraw;
	//int numVertPixelsToDraw = h;


	Clip(w, h, _x, _y, dest->w, dest->h, numHorizPixelsToDraw, vertSPanOfPix);

	//part of this image will be off screen, part will be on screen
	if (_x < 0 && _x+w > 0)
	{
		//flip the neg start pixel location to pos
		srcX = _x*=-1;
		_x = 0;
	}
		
	for (unsigned int uiV = _y; uiV < vertSPanOfPix; ++uiV)
	{
		// reset coordinate for each row
		RGBA *startPixel = &dest->pixels[uiV * dest->w + _x];

		std::memcpy(startPixel, &pixels[srcX], numHorizPixelsToDraw * sizeof(RGBA));
		srcX += w;
	}
}
//---------------------------------------------------------------------------------------
void PIXMAP::CopyPixels(unsigned char *src, int srcW, int srcH, int x, int y)
{
	if (x > w || y > h)
		return;

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
void PIXMAP::CopyPixels(RGBA *src, int srcW, int srcH, int x, int y)
{
	if (x > w || y > h)
		return;

	RGBA *startPixel;
	for (unsigned int uiV = 0; uiV < srcH; ++uiV)
	{
		// reset coordinate for each row
		RGBA* curPix = &src[uiV * srcW];
		startPixel = &pixels[(y + uiV) * w + x];

		for (unsigned int uiH = 0; uiH < srcW; ++uiH)
		{
			*startPixel++ = RGBA(curPix->r, curPix->g, curPix->b, curPix->a);
			curPix++;// next RGBA
		}
	}
}
//---------------------------------------------------------------------------------------
void PIXMAP::Clear()
{
	std::memset(pixels, 0, GetSize());
}
//---------------------------------------------------------------------------------------
unsigned char * PIXMAP::GetRawPixelCopy()
{
	unsigned char *retPixels = new unsigned char[w * h * 4];
	std::memcpy(retPixels, pixels, GetSize());
	return retPixels;
}
//---------------------------------------------------------------------------------------
PIXMAP* PIXMAP::GetScaledCopy(unsigned int Width, unsigned int Height)
{
	PIXMAP *dest = new PIXMAP(Width, Height);
	double _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(w));
	double _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(h));

	for (int y = 0; y < h; y++) //Run across all Y pixels.
		for (int x = 0; x < w; x++) //Run across all X pixels.
			for (int o_y = 0; o_y < _stretch_factor_y; ++o_y) //Draw _stretch_factor_y pixels for each Y pixel.
				for (int o_x = 0; o_x < _stretch_factor_x; ++o_x) //Draw _stretch_factor_x pixels for each X pixel.
				{
					unsigned int destX = (_stretch_factor_x * x) + o_x;
					unsigned int destY = (_stretch_factor_y * y) + o_y;
					RGBA *curPixel = &pixels[y * w + x];
					dest->pixels[destY * Width + destX] = *curPixel;
				}
	return dest;
}
//---------------------------------------------------------------------------------------
void PIXMAP::Scale(unsigned int Width, unsigned int Height)
{
	PIXMAP *scaledCopy = new PIXMAP(Width, Height);
	double _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(w));
	double _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(h));

	for (int y = 0; y < h; y++) //Run across all Y pixels.
		for (int x = 0; x < w; x++) //Run across all X pixels.
			for (int o_y = 0; o_y < _stretch_factor_y; ++o_y) //Draw _stretch_factor_y pixels for each Y pixel.
				for (int o_x = 0; o_x < _stretch_factor_x; ++o_x) //Draw _stretch_factor_x pixels for each X pixel.
				{
					unsigned int destX = (_stretch_factor_x * x) + o_x;
					unsigned int destY = (_stretch_factor_y * y) + o_y;
					RGBA *curPixel = &pixels[y * w + x];
					scaledCopy->pixels[destY * Width + destX] = *curPixel;
				}
	Destroy();
	w = Width;
	h = Height;
	pixels = new RGBA[w*h];
	std::memcpy(pixels, scaledCopy->pixels,GetSize());
	delete scaledCopy;
}
//---------------------------------------------------------------------------------------
void PIXMAP::DrawScaledCopy(PIXMAP *dest, int xPos, int yPos, unsigned int Width, unsigned int Height)
{
	int numHorizPixelsToDraw = w, vertSPanOfPix = h;

	//if (xPos > dest->w || yPos > dest->h)
	//	return;

	int startX = 0;
	int startY = 0;
	
	Clip(w, h, xPos, yPos, dest->w, dest->h, numHorizPixelsToDraw, vertSPanOfPix);
	if (xPos < 0)
	{
		if (xPos + Width < 0)
			return;//way off screen
		else if (xPos < 0 && xPos + Width > 0)
		{
			//flip the neg start pixel location to pos
			startX = xPos *= -1;
			xPos = 0;
		}
	}

	if (yPos < 0)
	{
		if (yPos + Height < 0)
			return;//way off screen
		else if (yPos < 0 && yPos + Height > 0)
		{
			//flip the neg start pixel location to pos
			startY = yPos *= -1;
			yPos = 0;
		}
	}

	double _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(w));
	double _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(h));
	int newPixLoc = 0;
	unsigned int destX;
	unsigned int destY;
	RGBA *curPixel;
	RGBA debugPixel;
	debugPixel.r = 255;
	debugPixel.g = 0;
	debugPixel.b = 255;
	debugPixel.a = 255;
	int pixSize = w*h;
	for (int y = startY; y < vertSPanOfPix; y++) //Run across all Y pixels.
	{
		for (int x = startX; x < numHorizPixelsToDraw; x++) //Run across all X pixels.
		{
			for (int o_y = 0; o_y < _stretch_factor_y; ++o_y) //Draw _stretch_factor_y pixels for each Y pixel.
			{
				for (int o_x = 0; o_x < _stretch_factor_x; ++o_x) //Draw _stretch_factor_x pixels for each X pixel.
				{
					destX = xPos + ((_stretch_factor_x * x) + o_x);
					destY = yPos + ((_stretch_factor_y * y) + o_y);

					//if (destX > dest->w || destY > dest->h || destX < 0 || destY < 0)
					//	continue;
					
					if ((y * w + x) > (w*h))
						continue;

					curPixel = &pixels[y * w + x];
					if (curPixel != nullptr)
					{
						newPixLoc = destY * dest->w + destX;
						if (newPixLoc < dest->GetNumPixels() - 1)
							dest->pixels[newPixLoc] = *curPixel;
						//std::memcpy(curPixel, &dest->pixels[destY * dest->w + destX], _stretch_factor_x * sizeof(RGBA));
					}
					else
					{
						dest->pixels[newPixLoc] = debugPixel;
					}
				}
			}
		}
	}
}
void PIXMAP::DrawScaledCopyForScreenSaver(PIXMAP *dest, int xPos, int yPos, unsigned int Width, unsigned int Height)
{
	int numHorizPixelsToDraw = w, vertSPanOfPix = h;
	
	//if (xPos > dest->w || yPos > dest->h)
	//	return;

	int startX = 0;
	int startY = 0;

	Clip(w, h, xPos, yPos, dest->w, dest->h, numHorizPixelsToDraw, vertSPanOfPix);
	if (xPos < 0)
	{
		if (xPos + Width < 0)
			return;//way off screen
		else if (xPos < 0 && xPos+Width > 0)
		{
			//flip the neg start pixel location to pos
			startX = xPos*=-1;
			xPos = 0;
		}
	}
	
	if (yPos < 0)
	{
		if (yPos + Height < 0)
			return;//way off screen
		else if (yPos < 0 && yPos+Height > 0)
		{
			//flip the neg start pixel location to pos
			startY = yPos*=-1;
			yPos = 0;
		}
	}

	double _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(numHorizPixelsToDraw));
	double _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(vertSPanOfPix));
	//double _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(w));
	//double _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(h));
	int newPixLoc = 0;
	unsigned int destX;
	unsigned int destY;
	RGBA *curPixel;

	for (int y = startY; y < vertSPanOfPix; y++) //Run across all Y pixels.
		for (int x = startX; x < numHorizPixelsToDraw; x++) //Run across all X pixels.
			for (int o_y = 0; o_y < _stretch_factor_y; ++o_y) //Draw _stretch_factor_y pixels for each Y pixel.
				for (int o_x = 0; o_x < _stretch_factor_x; ++o_x) //Draw _stretch_factor_x pixels for each X pixel.
				{
					//unsigned int destX =  xPos + (_stretch_factor_x * x);
					//unsigned int destY =  yPos + (_stretch_factor_y * y);
					destX = xPos + ((_stretch_factor_x * x) + o_x);
					destY = yPos + ((_stretch_factor_y * y) + o_y);
					
					if(destX > dest->w || destY > dest->h || destX < 0 || destY < 0)
						continue;
					
					curPixel = &pixels[y * w + x];
					
					newPixLoc = destY * dest->w + destX;
					if(newPixLoc < dest->GetNumPixels()-1)
						dest->pixels[newPixLoc] = *curPixel;
						//std::memcpy(curPixel, &dest->pixels[destY * dest->w + destX], _stretch_factor_x * sizeof(RGBA));
				}
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
