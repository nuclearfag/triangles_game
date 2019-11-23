//	Точка входа в процедуру работы движка
//	Версия 1.2 Evo, правка 1.
//	Последнее обновление: 10.09.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#if (_MSC_VER >= 1915)
#define no_init_all deprecated
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable:4845)   // __declspec(no_init_all) used but dllinitall not set
#endif

#include "Kernel1_DescriptorTable.h"
using namespace Kernel1_Descriptor;

#include "RWHeaders.h"
#include "RWHardwareSync.h"
#include "TG_Level.hpp"

using namespace RWGraphics;
using namespace RWScript;
using namespace RWHardwareSync;

long long __stdcall windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg) 
	{
	case WM_DESTROY:
		RWController::uninitialize();
		delete[] signalBuffer;
		Kernel1_GraphicsUninitialize();
		exit(EXIT_SUCCESS);
		break;
	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_ESCAPE:
			DestroyWindow(GetForegroundWindow());
			exit(EXIT_SUCCESS);
			break;
		case VK_PAUSE:
			CloseWindow(GetForegroundWindow());
			break;
		default:
			throwSignal(wParam, signalBuffer);
			break;
		}
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) 
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wcex.hInstance = hInst;
	wcex.lpfnWndProc = (WNDPROC)windowProc;
	wcex.lpszClassName = "R-Wolf Evo 2D Graphics Engine";
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wcex);

	RECT rc = { 0, 0,
		getConfigString(ScreenSize_X), getConfigString(ScreenSize_Y) };
	AdjustWindowRectEx(&rc, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, true, WS_EX_OVERLAPPEDWINDOW);

	HWND windowHandle = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		wcex.lpszClassName,
		"R-Wolf Evo Engine",
		WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rc.right - rc.left),
		(rc.bottom - rc.top),
		NULL,
		NULL,
		hInst,
		0);

	if (!windowHandle) 
	{
		auto errorcode = WINNT_HWND_HADLE_ERROR;
		string definition = "WINNT_HWND_HANDLE_ERROR";
		RW_ERROR err(errorcode, definition, 1);
		return EXIT_FAILURE;
	}

	if (static_cast<bool>(getConfigString(Screen_IsFullscreen))) 
	{
		SetWindowLong(windowHandle, GWL_STYLE, WS_POPUP);
		SetWindowLong(windowHandle, GWL_EXSTYLE, WS_EX_TOPMOST);
		ShowWindow(windowHandle, SW_SHOWMAXIMIZED);
	}

	else 
	{
		ShowWindow(windowHandle, SW_SHOWNORMAL);
	}

	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	Kernel1_GraphicsInitialize(factory, wFactory, brush, path, renderTarget, windowHandle);

	RWController::initialize();
	RWController::loadInitialLevel(new TGLevel());

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT) 
	{
		SetCursor(NULL);
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		RWController::update();
		beginDraw();
		RWController::render();
		endDraw();
	}
	return EXIT_SUCCESS;
}