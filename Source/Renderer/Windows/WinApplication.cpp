
#include <cassert>

#include "WinApplication.h"
#include "GDIHelper.h"
#include "SoftRenderer.h"


WinApplication* WinApplication::mWinApplication = nullptr;

WinApplication::WinApplication()
	: mHwnd(nullptr)
	, mHInstance(nullptr)
{
	mGDIHelper = std::make_unique<GDIHelper>();
	mSoftRenderer = std::make_unique<SoftRenderer>();
}

void WinApplication::CreateInstance()
{
	assert(mWinApplication == nullptr);
	mWinApplication = new WinApplication();
}

WinApplication* WinApplication::GetInstance()
{
	assert(mWinApplication != nullptr);
	return mWinApplication;
}

void WinApplication::DestroyInstance()
{
	assert(mWinApplication != nullptr);
	delete mWinApplication;
	mWinApplication = nullptr;
}

int WinApplication::Run(HINSTANCE hInstance, int nCmdShow)
{
	mHInstance = hInstance;

	// Initialize the window class
	WNDCLASSEX windowClass = { 0, };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = mHInstance;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszClassName = APP_TITLE;

	if (RegisterClassEx(&windowClass) == 0)
	{
		return 0;
	}
	
	mHInstance = hInstance;

	RECT windowRect = { 0, 0, static_cast<LONG>(APP_WIDTH), static_cast<LONG>(APP_HEIGHT) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	mHwnd = CreateWindow(
		APP_TITLE,
		APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		mHInstance,
		nullptr);

	if (mHwnd == nullptr)
	{
		return 0;
	}

	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);

	if (mSoftRenderer == nullptr)
	{
		return 0;
	}

	mGDIHelper->InitGDI(mHwnd);
	mSoftRenderer->Initialize(mGDIHelper.get());

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// Process any messages in the queue.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		mSoftRenderer->UpdateFrame();
	}

	return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK WinApplication::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return WinApplication::GetInstance()->MessageHandler(hWnd, message, wParam, lParam);
}
