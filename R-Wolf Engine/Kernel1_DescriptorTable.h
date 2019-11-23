#pragma once
#include <windows.h>
#include <D2D1.h>
#include <d2d1_1.h>
#include <DWrite.h>
#include <DXGI.h>
#include <dxgitype.h>
#include <wincodec.h>
namespace Kernel1_Descriptor
{
	__declspec(selectany) HINSTANCE hLib = LoadLibrary("Kernel1.dll");
	typedef void (*K1_GraphicsInitialize)(ID2D1Factory*&, IDWriteFactory*&, ID2D1SolidColorBrush*&, ID2D1PathGeometry*&, ID2D1HwndRenderTarget*&, HWND);
	typedef void (*K1_GraphicsUninitialize)();
	typedef ID2D1HwndRenderTarget* (*K1_GraphicsGetRenderTarget)();
	inline void Kernel1_GraphicsInitialize
	(
		ID2D1Factory* &factory,
		IDWriteFactory* &wFactory,
		ID2D1SolidColorBrush* &brush,
		ID2D1PathGeometry* &path,
		ID2D1HwndRenderTarget* &renderTarget,
		HWND targetWindow
	)
	{
		if (hLib)
		{
			K1_GraphicsInitialize func;
			func = (K1_GraphicsInitialize)GetProcAddress(hLib, "K1_GraphicsInitialize");
			if (!func)
			{
				auto except_str = "Unable to find K1_GraphicsInitialize function.\nProcess will be immediately terminated.";
				MessageBox(0, except_str, "Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
				exit(1023);
			}
			else
			{
				func(factory, wFactory, brush, path, renderTarget, targetWindow);
			}
		}
		else
		{
			auto except_str = "Unable to load Kernel1.dll module.\nProcess will be immediately terminated.";
			MessageBox(0, except_str, "Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
			exit(1024);
		}
	}
	inline void Kernel1_GraphicsUninitialize()
	{
		if (hLib)
		{
			K1_GraphicsUninitialize func;
			func = (K1_GraphicsUninitialize)GetProcAddress(hLib, "K1_GraphicsUninitialize");
			if (!func)
			{
				auto except_str = "Unable to find K1_GraphicsUninitialize function.\nProcess will be immediately terminated.";
				MessageBox(0, except_str, "Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
				exit(1023);
			}
			else
			{
				func();
			}
		}
		else
		{
			auto except_str = "Unable to load Kernel1.dll module.\nProcess will be immediately terminated.";
			MessageBox(0, except_str, "Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
			exit(1024);
		}
	}
	inline ID2D1HwndRenderTarget* Kernel1_GraphicsGetRenderTarget()
	{
		if (hLib)
		{
			K1_GraphicsGetRenderTarget func;
			func = (K1_GraphicsGetRenderTarget)GetProcAddress(hLib, "K1_GraphicsGetRenderTarget");
			if (!func)
			{
				auto except_str = "Unable to find K1_GraphicsGetRenderTarget function.\nProcess will be immediately terminated.";
				MessageBox(0, except_str, "Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
				exit(1023);
			}
			else
			{
				return func();
			}
		}
		else
		{
			auto except_str = "Unable to load Kernel1.dll module.\nProcess will be immediately terminated.";
			MessageBox(0, except_str, "Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
			exit(1024);
		}
	}
	inline void Kernel1_Descriptor_Release()
	{
		FreeLibrary(hLib);
	}
}