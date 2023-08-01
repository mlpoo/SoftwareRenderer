// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

#include "GDIHelper.h"
#include "SoftRenderer.h"

class WinApplication
{
public:
	~WinApplication() = default;

	static void CreateInstance();
	static WinApplication* GetInstance();
	static void DestroyInstance();
	
	int Run(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	WinApplication();
	
private:

	static WinApplication* mWinApplication;
	
	HWND mHwnd;
	HINSTANCE mHInstance;
	std::unique_ptr<GDIHelper> mGDIHelper;
	std::unique_ptr<SoftRenderer> mSoftRenderer;

public:

	inline HWND GetHwnd() const { return mHwnd; }
	inline HINSTANCE GetHInstnace() const { return mHInstance; }

};

// Windows callback function
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
