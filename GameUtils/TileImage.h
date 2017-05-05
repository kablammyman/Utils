#pragma once

#include "GraphicsPrimitives.h"
#include <vector>

struct Tile
{
	int numPixels;
	unsigned blockX, blockY;//where it is inthe image
	int curNumPixelsInBlock;

	RGBA *pixels[];
	Tile(int w, int h)
	{
		curNumPixelsInBlock = 0;
		numPixels = w * h;
		*pixels = new RGBA[numPixels];
	};

	~Tile()
	{
		delete[] pixels;
	}

	void AddPixel(RGBA *pixel)
	{
		pixels[curNumPixelsInBlock] = pixel;
		if (curNumPixelsInBlock < numPixels - 1)
			curNumPixelsInBlock++;
	}
	void AddBlockCords(int x, int y)
	{
		blockX = x;
		blockY = y;
	}
	
};
class TileImage
{
	
	RGBA *pixelData;

	unsigned int iWidth;
	unsigned int iHeight;
	std::vector<Tile> imgBlocks;

	
public:
	int BLOCK_WIDTH;
	int BLOCK_HEIGHT;
	int NUM_PIXELS;
	
	
	TileImage(RGBA * data, int tileW, int tileH)
	{
		pixelData = data;
		iWidth = 0;
		iHeight = 0;
		BLOCK_WIDTH = tileW;
		BLOCK_HEIGHT = tileH;
		NUM_PIXELS = BLOCK_WIDTH * BLOCK_HEIGHT;

		//put the pixels in blocks
		int numBlocksX = iWidth / BLOCK_WIDTH;
		int numBlocksY = iHeight / BLOCK_HEIGHT;

		for (int i = 0; i < numBlocksX*numBlocksY; i++)
			imgBlocks.push_back(Tile(BLOCK_WIDTH, BLOCK_HEIGHT));
	}
	TileImage()
	{
		pixelData = NULL;
		imgBlocks.clear();
		iWidth = 0;
		iHeight = 0;
		BLOCK_WIDTH = 4;
		BLOCK_HEIGHT = 4;
		NUM_PIXELS = BLOCK_WIDTH * BLOCK_HEIGHT;
	}

	~TileImage()
	{
		if(pixelData != NULL)
			delete[] pixelData;

		pixelData = NULL;
	}
	

	void ConvertPixelArrayIntoBlocks();
	void GetBlockCords(unsigned int a_iPixelX, unsigned int a_iPixelY, int & blockX, int & blockY);
	void GetBlockCordsFromPixelCords(unsigned int pixelX,unsigned int plixelY, int & blockX, int & blockY);

	Tile * GetBlock(int blockX, int blockY);
	Tile * GetBlock(int index);


	inline int GetNumBlocksX() { return iWidth / BLOCK_WIDTH; }
	inline int GetNumBlocksY() {return iHeight / BLOCK_HEIGHT;}
	inline int GetNumBlocks() {return GetNumBlocksX()*GetNumBlocksY();}


	unsigned char* GetPNGDataFromBlockImage();
};