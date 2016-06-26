#pragma once
#include <string>
#include <Windows.h>


class ClipboardUtil
{
	public:
	
	static std::string GetClipboardStringData();
	static HBITMAP GetClipboardImageData();
};