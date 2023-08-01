// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#include <cassert>

#include "WinApplication.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	WinApplication::CreateInstance();
	
	WinApplication* winApp = WinApplication::GetInstance();
	assert(winApp != nullptr);

	winApp->Run(hInstance, nCmdShow);

	WinApplication::DestroyInstance();

	return 0;
}