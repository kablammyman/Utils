#include "ClipboardUtil.h"

std::string ClipboardUtil::GetClipboardStringData()
{
	if (!OpenClipboard(NULL))
	{
		printf("Can't open clipboard");
		return "";
	}

	HANDLE textData = GetClipboardData(CF_TEXT);
	std::string rturnstring((char *)textData);

	CloseClipboard();
	return rturnstring;
}

