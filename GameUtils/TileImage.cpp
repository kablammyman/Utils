#include "TileImage.h"

TileImage::TileImage(RGBA * data, int imgW, int imgH, int tileW, int tileH)
{
	iWidth = imgW;
	iHeight = imgH;
	BLOCK_WIDTH = tileW;
	BLOCK_HEIGHT = tileH;
	NUM_PIXELS_BLOCK = BLOCK_WIDTH * BLOCK_HEIGHT;
	int numPixels = iWidth *iHeight;

	pixelData = new RGBA[numPixels];
	//memcpy(pixelData, data, iWidth *iHeight*sizeof(RGBA));

	for(int i = 0; i < numPixels; i++)
		pixelData[i] = RGBA(data[i].r, data[i].g, data[i].b, data[i].a);

	//put the pixels in blocks
	int numBlocksX = iWidth / BLOCK_WIDTH;
	int numBlocksY = iHeight / BLOCK_HEIGHT;
	int numBlocks = numBlocksX*numBlocksY;

	for (int i = 0; i < numBlocks; i++)
	{
		Tile temp (BLOCK_WIDTH, BLOCK_HEIGHT);
		imgBlocks.push_back(temp);
	}

	ConvertPixelArrayIntoBlocks();
}

TileImage::TileImage(PIXMAP * data, int imgW, int imgH, int tileW, int tileH)
{
	TileImage(data->pixels, imgW, imgH, tileW, tileH);
}

TileImage::TileImage()
{
	pixelData = NULL;
	imgBlocks.clear();
	iWidth = 0;
	iHeight = 0;
	BLOCK_WIDTH = 4;
	BLOCK_HEIGHT = 4;
	NUM_PIXELS_BLOCK = BLOCK_WIDTH * BLOCK_HEIGHT;
}

TileImage::~TileImage()
{
	if (pixelData != NULL)
		delete[] pixelData;

	pixelData = NULL;
}

//---------------------------------------------------------------------------------------------------
void TileImage::GetBlockCords(unsigned int a_iPixelX, unsigned int a_iPixelY, int & blockX, int & blockY)
{
	
	if (a_iPixelX > iWidth)
		a_iPixelX = iWidth;
	if (a_iPixelY > iHeight)
		a_iPixelY = iHeight;
	

	// remove the bottom 2 bits to get the block coordinates 
	blockX = (a_iPixelX & 0xFFFFFFFC);
	blockY = (a_iPixelY & 0xFFFFFFFC);
}

//---------------------------------------------------------------------------------------------------
void TileImage::ConvertPixelArrayIntoBlocks()
{
	//if we already have some block data, then exit
	//even if the block data is somehow invalid
	if(imgBlocks.empty())
		return;
	
	//if there is no pixel data, then exit
	if(pixelData == NULL)
		return;

	RGBA *pucPixel;

	//put the pixels in blocks
	int numBlocksX = iWidth / BLOCK_WIDTH;
	//int numBlocksY = iHeight / BLOCK_HEIGHT;
	int blockX, blockY;


	// convert pixels from RGBA* to ColorFloatRGBA
	for (unsigned int uiV = 0; uiV <  iHeight; ++uiV)
	{
		// reset coordinate for each row
		pucPixel = &pixelData[uiV * iWidth];

		// read each row
		for (unsigned int uiH = 0; uiH <  iWidth; ++uiH)
		{
			blockX = uiH / BLOCK_WIDTH;
			blockY = uiV / BLOCK_HEIGHT;
			int index = blockY * numBlocksX + blockX;
			imgBlocks[index].AddPixel(pucPixel);
			imgBlocks[index].AddBlockCords(blockX, blockY);
			pucPixel++;
		}
	}
}
//---------------------------------------------------------------------------------------------------

void TileImage::GetBlockCordsFromPixelCords(unsigned int pixelX, unsigned int pixelY, int & blockX, int & blockY)
{
	blockX = 0;
	blockY = 0;

	for (unsigned int i = 0; i < iWidth; i += BLOCK_WIDTH)
	{
		
		if (i <= pixelX && pixelX < i + BLOCK_WIDTH)
		{
			break;
		}
		blockX++;
	}
	for (unsigned int j = 0; j < iHeight; j += BLOCK_HEIGHT)
	{
		
		if (j <= pixelY && pixelY < j + BLOCK_HEIGHT)
		{
			break;
		}
		blockY++;
	}
}
//---------------------------------------------------------------------------------------------------
Tile * TileImage::GetBlock(unsigned int blockX, unsigned int blockY)
{
	int numBlocksX = iWidth / BLOCK_WIDTH;
	//int numBlocksY = iHeight / BLOCK_HEIGHT;

	return &imgBlocks[blockY * numBlocksX + blockX];
}
//---------------------------------------------------------------------------------------------------
Tile * TileImage::GetBlock(unsigned int index)
{
	int numBlocksX = iWidth / BLOCK_WIDTH;
	int numBlocksY = iHeight / BLOCK_HEIGHT;

	if(index < (numBlocksX*numBlocksY))
		return &imgBlocks[index];
	//if we get here, we are trying to print a character we dont have
	return &imgBlocks[0];
}
//---------------------------------------------------------------------------------------------------
void TileImage::DrawTileToPixmap(PIXMAP *dest, Tile  *tile, int x, int y,bool masked)
{
	RGBA *startPixel;
	for (unsigned int uiV = 0; uiV < BLOCK_HEIGHT; ++uiV)
	{
		// reset coordinate for each row
		RGBA* curPix = tile->pixels[uiV * BLOCK_WIDTH];
		startPixel = &dest->pixels[(y + uiV) * dest->w + x];

		for (unsigned int uiH = 0; uiH < BLOCK_WIDTH; ++uiH)
		{
			
			if (masked && pixelMaskColor == *curPix)
				*startPixel++;
			else
				*startPixel++ = RGBA(curPix->r, curPix->g, curPix->b, curPix->a);

			curPix++;// next RGBA
		}
	}
}
//---------------------------------------------------------------------------------------------------
unsigned char*  TileImage::GetPNGDataFromBlockImage()
{
	unsigned char *rawData = new unsigned char[(iWidth * iHeight) * 4];
	unsigned char *runner = rawData;

	int numBlocksX = GetNumBlocksX();
	int numBlocksY = GetNumBlocksY();


	for (int uiV = 0; uiV < numBlocksY; uiV++)
	{
		for (int uiH = 0; uiH < numBlocksX; uiH++)
		{
			Tile * curBlock = &imgBlocks[uiV * numBlocksX +  uiH];
			
			for (int y = 0; y < BLOCK_HEIGHT ; y++)
			{
				int pixY = uiV * BLOCK_HEIGHT + y;
				int pixX = uiH * BLOCK_WIDTH;
				runner = &rawData[(pixY * iWidth + pixX) * 4];

				for (int x = 0; x < BLOCK_WIDTH; x++)
				{
					int i = (y * BLOCK_WIDTH + x);
					*runner++ = curBlock->pixels[i]->r;
					*runner++ = curBlock->pixels[i]->g;
					*runner++ = curBlock->pixels[i]->b;
					*runner++ = curBlock->pixels[i]->a; 
				}
			}
			
		}
	}
	//lodepng_encode32_file(filename.c_str(), rawData, iWidth, iHeight);
	//delete[] rawData;
	return rawData;
}
//---------------------------------------------------------------------------------------------------
unsigned char* TileImage::GetBlockCopy(unsigned int index)
{
	unsigned char *rawData = new unsigned char[(BLOCK_WIDTH * BLOCK_HEIGHT) * 4];
	unsigned char *runner = rawData;
	
	Tile curBlock = imgBlocks[index];
	for (int i = 0; i < NUM_PIXELS_BLOCK; i++)
	{
		RGBA *curPixel = curBlock.pixels[i];
		*runner++ = curPixel->r;
		*runner++ = curPixel->g;
		*runner++ = curPixel->b;
		*runner++ = curPixel->a;
	}
	//lodepng_encode32_file(filename.c_str(), rawData, BLOCK_WIDTH, BLOCK_HEIGHT);
	//delete[] rawData;
	return rawData;
}
//---------------------------------------------------------------------------------------------------
unsigned char* TileImage::GetBlockCopy(unsigned int x, unsigned int y)
{
	int numBlocksX = iWidth / BLOCK_WIDTH;
	int index = y * numBlocksX + x;

	return GetBlockCopy(index);
}