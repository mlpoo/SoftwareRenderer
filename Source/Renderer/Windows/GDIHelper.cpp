
#include "GDIHelper.h"


void GDIHelper::BufferSwap()
{
	BitBlt(hScreenDC, 0, 0, APP_WIDTH, APP_HEIGHT, hMemoryDC, 0, 0, SRCCOPY);
}

void GDIHelper::SetColor(u8 r, u8 g, u8 b)
{
	currentColor = RGB(b, g, r);
}

void GDIHelper::Clear()
{
	u32* dest = (u32*)pBits;
	DWORD bytecount = APP_WIDTH * APP_HEIGHT * sizeof(u32);
	u32 value = currentColor;
	bytecount /= 4;
	while (bytecount--)
	{
		*dest++ = value;
	}
	return;
}

void GDIHelper::InitGDI(HWND hWnd)
{
	hScreenDC = GetDC(hWnd);
	hMemoryDC = CreateCompatibleDC(hScreenDC);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = APP_WIDTH;
	bmi.bmiHeader.biHeight = -APP_HEIGHT;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	hDIBitmap = CreateDIBSection(hMemoryDC, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	hDefaultBitmap = (HBITMAP)SelectObject(hMemoryDC, hDIBitmap);
}

void GDIHelper::ReleaseGDI(HWND hWnd)
{
	DeleteObject(hDefaultBitmap);
	DeleteObject(hDIBitmap);
	ReleaseDC(hWnd, hScreenDC);
	ReleaseDC(hWnd, hMemoryDC);
}
