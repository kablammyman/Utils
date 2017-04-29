#pragma once
#include <string> // memcpy

#include <cstring>

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
		pixels = 0;
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
		int srcX = 0,srcY = 0;
		int width = ((x + w) > dest.w) ? dest.w : w;
		int height = ((y + h) > dest.h) ? dest.h : h;
		int row = 0;

		for (unsigned int uiV = y; uiV < (srcY+height); ++uiV)
		{
			// reset coordinate for each row
			pucPixel = &dest->pixels[(uiV * dest.w + srcX) * sizeof(RGB)];
				
			// *pfrgbaPixel++ = ColorFloatRGBA::ConvertFromRGBA8(pucPixel[0], pucPixel[1],pucPixel[2], pucPixel[3]);
			
			//memcpy(dest->pixels, pixels, GetSize());
			//memcpy( &dst[dstIdx], &src[srcIdx], numElementsToCopy * sizeof( Element ) );
			memcpy(pucPixel, &pixels[row], width * sizeof( RGB ) );
			row += w;
			
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
void SourceImage::Read(int a_iPixelX, int a_iPixelY)
	{
		
		int iBitsPerPixel;
		//if stb_iamge is available, only use it to load files other than png
		char *fileExt = strrchr(m_pstrFilename, '.');

		if (strcmp(fileExt, ".png") != 0)
		{
			paucPixels = stbi_load(m_pstrFilename, &iWidth, &iHeight, &iBitsPerPixel, 4);

			if (paucPixels == nullptr)
			{
				printf("stb_image error %s\n", stbi_failure_reason());
				assert(0);
				exit(1);
			}
		}
#endif


		//the pixel cords for the top left corner of the block
		int iBlockX = 0;
		int iBlockY = 0;
		if (a_iPixelX > -1 && a_iPixelY > -1)
		{
			// in 1 block mode, we basically will have an img thats 4x4
			m_uiWidth = 4;
			m_uiHeight = 4;

			if(a_iPixelX > iWidth)
				a_iPixelX = iWidth;
			if (a_iPixelY > iHeight)
				a_iPixelY = iHeight;

			// remove the bottom 2 bits to get the block coordinates 
			iBlockX = (a_iPixelX & 0xFFFFFFFC);
			iBlockY = (a_iPixelY & 0xFFFFFFFC);
		}
		else
		{
			m_uiWidth = iWidth;
			m_uiHeight = iHeight;
		}

		m_pafrgbaPixels = new ColorFloatRGBA[m_uiWidth * m_uiHeight];
		assert(m_pafrgbaPixels);

		int iBytesPerPixel = bool16BitImage ? 8 : 4;
		unsigned char *pucPixel;	// = &paucPixels[(iBlockY * iWidth + iBlockX) * iBytesPerPixel];
		ColorFloatRGBA *pfrgbaPixel = m_pafrgbaPixels;

		// convert pixels from RGBA* to ColorFloatRGBA
		

#if USE_STB_IMAGE_LOAD
		stbi_image_free(paucPixels);
#else
		free(paucPixels);
#endif
	}
