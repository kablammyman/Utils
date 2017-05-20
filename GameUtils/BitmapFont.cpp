#include "BitmapFont.h"
#include "StockFontData.h"

/* i made the bin file froma 2 color bmp code after loading a png file with the allegro stock font
the bin is 1 bit per pixel. I then put that bin file into bin2c.exe to get stockFont_data
*/

//BitmapFont StockFont::stockFont((RGBA*)stockFont_bin,256,128,8,8);
//---------------------------------------------------------------------------------------
BitmapFont::BitmapFont(RGBA *data, int w, int h, int cW, int cH)
{
	charW = cW;
	charH = cH;

	font = new TileImage(data, w, h, cW, cH);
	font->ConvertPixelArrayIntoBlocks();
}
//---------------------------------------------------------------------------------------
void BitmapFont::CalcPixmapLineSize(int numChars, unsigned int &w, unsigned int &h)
{
	w = (numChars * (charW+2));
	h = charW;
}
//---------------------------------------------------------------------------------------
TileImage *BitmapFont::GetFont()
{
	return font;
}
//---------------------------------------------------------------------------------------
/*void StockFont::Draw(PIXMAP *dest, ScreenText *st)
{
	unsigned int x = st->GetPosX();
	unsigned int y = st->GetPosY();
	unsigned int w,h;
	std::string text = st->GetText();

	stockFont.CalcPixmapLineSize((int)text.size(),w,h);
	if (x+w > dest->w)
	{
		for (size_t i = 0; i < text.size(); i++)
		{
			text.pop_back();
			stockFont.CalcPixmapLineSize((int)text.size(), w, h);
			if (x+w < dest->w)
				break;
		}
	}
	for (int i = 0; i < text.size(); i++)
	{
		unsigned char* letter = stockFont.GetFont()->SaveBlockAsPNG((int)text[i]);
		dest->CopyPixels(letter, 8, 8,x+(i*9), y);
	}
}*/
//---------------------------------------------------------------------------------------
StockBitmapFont::StockBitmapFont()
{
	charW = 8;
	charH = 8;
	RGBA *binData = Read1bbpBin();
	font = new TileImage(binData, 256, 128, charW, charH);
}
//---------------------------------------------------------------------------------------
void StockBitmapFont::Create1bppBin(unsigned char *pngData)
{
	int iWidth, iHeight;
	//int error = lodepng_decode_file(&pix, (unsigned int*)&iWidth, (unsigned int*)&iHeight, "D:\\source\\GameUtilsTest\\allegFont2.png", LCT_RGBA, 8);

	FILE *ptr_file = fopen("stockFont.bin", "wb");

	int iBytesPerPixel = 4;

	int reset = sizeof(unsigned char) * 8;
	unsigned char *pucPixel;

	for (unsigned int uiV = 0; uiV < iHeight; ++uiV)
	{
		// reset coordinate for each row
		unsigned char* pucPixel = &pngData[uiV * iWidth * iBytesPerPixel];
		unsigned int row = 0;

		// read each row of orig image
		//reverse the colors...the orig img has whtie background black text
		//i want black bacground with white text
		int count = reset;
		for (unsigned int uiH = 0; uiH < iWidth; ++uiH)
		{

			if (pucPixel[0] > 0 || pucPixel[1] > 0 || pucPixel[2])
			{
				row <<= 1;
			}
			else
			{
				if (row == 0)
					row = 1;
				else
				{
					row <<= 1;
					row++;
				}
			}
			pucPixel += iBytesPerPixel;
			count--;
			if (count == 0)
			{
				uint32_t szBytesWritten = fwrite(&row, 1, sizeof(unsigned char), ptr_file);
				count = reset;
				row = 0;
			}
		}

	}

	fclose(ptr_file);
}
//---------------------------------------------------------------------------------------
RGBA * StockBitmapFont::Read1bbpBin()
{
	RGBA *data = new RGBA[256 * 128];
	RGBA *curPixel = data;
	size_t i = 0;
	unsigned char temp;
	while (i < stockFont_bin_size)
	{
		temp = stockFont_bin[i];

		//i have to revers the bits for some reason...am i reading them in backward?
		unsigned char r = temp;
		int s = sizeof(temp) * CHAR_BIT - 1;
		for (temp >>= 1; temp; temp >>= 1)
		{
			r <<= 1;
			r |= temp & 1;
			s--;
		}

		r <<= s;
		for (int j = 0; j < sizeof(unsigned char) * 8; j++)
		{
			if (((1 << 1) & r))
				*curPixel++ = RGBA(255, 255, 255, 255);
			else
				*curPixel++ = RGBA(0, 0, 0, 255);

			r >>= 1;
		}
		i++;
	}

	return data;
}
//---------------------------------------------------------------------------------------
void StockBitmapFont::Draw(PIXMAP *dest, std::string text, int x, int y)
{
	unsigned int w, h;

	CalcPixmapLineSize((int)text.size(), w, h);
	if (x + w > dest->w)
	{
		for (size_t i = 0; i < text.size(); i++)
		{
			text.pop_back();
			CalcPixmapLineSize((int)text.size(), w, h);
			if (x + w < dest->w)
				break;
		}
	}

	for (int i = 0; i < (int)text.size(); i++)
	{
		unsigned char* letter = GetFont()->SaveBlockAsPNG((int)text[i]);
		dest->CopyPixels(letter, 8, 8, x + (i * charW), y);
	}
}