#pragma once

#include "GraphicsPrimitives.h"
#include <vector>

struct Tile
{
	int numPixels;
	unsigned blockX, blockY;//where it is inthe image
	int curNumPixelsInBlock;

	std::vector<RGBA *> pixels;

	Tile()
	{
		numPixels = 0;
	};

	Tile(int w, int h)
	{
		curNumPixelsInBlock = 0;
		numPixels = w * h;
	};

	~Tile()
	{
		pixels.clear();
	}

	void AddPixel(RGBA *pixel)
	{
		if(pixels.size() < numPixels)
			pixels.push_back(pixel);

	}
	void AddBlockCords(int x, int y)
	{
		blockX = x;
		blockY = y;
	}
	RGBA * GetPixels()
	{
		return pixels[0];
	}
};
class TileImage
{
	
	RGBA *pixelData;
	RGBA pixelMaskColor;

	unsigned int iWidth;
	unsigned int iHeight;
	std::vector<Tile> imgBlocks;

	
public:
	int BLOCK_WIDTH;
	int BLOCK_HEIGHT;
	int NUM_PIXELS_BLOCK;
	
	
	TileImage(RGBA * data, int imgW, int imgH, int tileW, int tileH);

	TileImage(PIXMAP * data, int imgW, int imgH, int tileW, int tileH);
	
	TileImage();
	~TileImage();
	

	void ConvertPixelArrayIntoBlocks();
	void GetBlockCords(unsigned int a_iPixelX, unsigned int a_iPixelY, int & blockX, int & blockY);
	void GetBlockCordsFromPixelCords(unsigned int pixelX,unsigned int plixelY, int & blockX, int & blockY);
	void SetPixelMask(RGBA color)
	{
		pixelMaskColor = color;
	}
	Tile * GetBlock(unsigned int blockX, unsigned int blockY);
	Tile * GetBlock(unsigned int index);


	inline int GetNumBlocksX() { return iWidth / BLOCK_WIDTH; }
	inline int GetNumBlocksY() {return iHeight / BLOCK_HEIGHT;}
	inline int GetNumBlocks() {return GetNumBlocksX()*GetNumBlocksY();}

	//ignore pixels that are a the masked pixel color
	void DrawTileToPixmap(PIXMAP *dest, Tile  *tile, int x, int y,bool masked = false);
	unsigned char* GetPNGDataFromBlockImage();
	unsigned char* GetBlockCopy(unsigned int index);
	unsigned char* GetBlockCopy(unsigned int x, unsigned int y);
};