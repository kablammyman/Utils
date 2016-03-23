#pragma once
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <Windows.h>

using namespace cv;

class ClipboardUtil
{
	public:
	
	static std::string GetClipboardStringData();
	static IplImage * hBitmapToIpl(HBITMAP hBmp);
	static Mat GetClipboardImage();
};