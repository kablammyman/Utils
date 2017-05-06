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
	int NUM_PIXELS_BLOCK;
	
	
	TileImage(RGBA * data, int imgW, int imgH, int tileW, int tileH);

	TileImage(PIXMAP * data, int imgW, int imgH, int tileW, int tileH);
	
	TileImage();
	~TileImage();
	

	void ConvertPixelArrayIntoBlocks();
	void GetBlockCords(unsigned int a_iPixelX, unsigned int a_iPixelY, int & blockX, int & blockY);
	void GetBlockCordsFromPixelCords(unsigned int pixelX,unsigned int plixelY, int & blockX, int & blockY);

	Tile * GetBlock(int blockX, int blockY);
	Tile * GetBlock(int index);


	inline int GetNumBlocksX() { return iWidth / BLOCK_WIDTH; }
	inline int GetNumBlocksY() {return iHeight / BLOCK_HEIGHT;}
	inline int GetNumBlocks() {return GetNumBlocksX()*GetNumBlocksY();}


	unsigned char* GetPNGDataFromBlockImage();
	unsigned char* SaveBlockAsPNG(int index);
	unsigned char* SaveBlockAsPNG( int x, int y);
};