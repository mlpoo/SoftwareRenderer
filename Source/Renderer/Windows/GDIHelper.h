// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

// SomWorks :D // GDI : Graphics Device Interface

#pragma once

#include "AppDefinition.h"
#include "WinApplication.h"

class GDIHelper
{
public:

	GDIHelper() = default;
	~GDIHelper() = default;

	void InitGDI(HWND hWnd);
	void ReleaseGDI(HWND hWnd);

	void SetColor(u8 r, u8 g, u8 b);
	void Clear();
	void BufferSwap();

private:

	u32 currentColor;
	u8* pBits;
	HDC	hScreenDC, hMemoryDC;
	HBITMAP hDefaultBitmap, hDIBitmap;

public:

	inline u8* GetpBits() const { return pBits; }
	inline u32 GetCurrentColor() const { return currentColor; }

};