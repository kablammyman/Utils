#include "ClipboardUtil.h"

std::string ClipboardUtil::GetClipboardStringData()
{
	if (!OpenClipboard(0))
	{
		printf("Can't open clipboard");
		return "";
	}

	HANDLE textData = GetClipboardData(CF_TEXT);
	std::string rturnstring((char *)textData);

	CloseClipboard();
	return rturnstring;
}

HBITMAP  ClipboardUtil::GetClipboardImageData()
{
	if (!OpenClipboard(0))
	{
		printf("Can't open clipboard");
		return 0;
	}
	HBITMAP hbmp;
	if (IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB) || IsClipboardFormatAvailable(CF_DIBV5))
	{
		hbmp = (HBITMAP)GetClipboardData(CF_BITMAP);

		if (hbmp != 0 && hbmp != INVALID_HANDLE_VALUE)
		{
			printf("Not valid image data");
		}
	}

	CloseClipboard();

	return hbmp;
}