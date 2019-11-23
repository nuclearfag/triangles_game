#include "pch.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cmath>
#include <D2D1.h>
#include <d2d1_1.h>
#include <DWrite.h>
#include <DXGI.h>
#include <dxgitype.h>
#include <wincodec.h>
#define RELEASE(a) if (a) a->Release()
#define WIPE(a) a = 0
#define SET(a) pLib->a = a
#define GET(a) pLib->a

enum Kernel1_Errors
{
	D2D1CreateFactory_FAIL = 8,
	D2D1CreateHwndRenderTarget_FAIL,
	D2D1DWriteCreateFactory_FAIL
};

namespace Kernel1
{
	namespace Graphics
	{
		class PointersLibrary
		{
		public:
			~PointersLibrary()
			{
				memset(this, 0, sizeof(*this));
			}
			PointersLibrary()
			{
				WIPE(factory);
				WIPE(wFactory);
				WIPE(brush);
				WIPE(path);
				WIPE(renderTarget);
				WIPE(targetWindow);
			}
			ID2D1Factory* factory;
			IDWriteFactory* wFactory;
			ID2D1SolidColorBrush* brush;
			ID2D1PathGeometry* path;
			ID2D1HwndRenderTarget* renderTarget;
			HWND targetWindow;
		};
		_declspec(selectany) PointersLibrary* pLib;
		extern "C" _declspec(dllexport) void __stdcall K1_GraphicsInitialize
		(
			ID2D1Factory* &factory,
			IDWriteFactory* &wFactory,
			ID2D1SolidColorBrush* &brush,
			ID2D1PathGeometry* &path,
			ID2D1HwndRenderTarget* &renderTarget,
			HWND targetWindow
		)
		{
			pLib = new PointersLibrary();
			SET(factory);
			SET(wFactory);
			SET(brush);
			SET(path);
			SET(renderTarget);
			HRESULT hr = 0l;
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
			if (SUCCEEDED(hr))
			{
				hr = factory->CreateHwndRenderTarget
				(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties
					(
						targetWindow,
						D2D1::SizeU
						(
							1920u,
							1080u
						)),
					&renderTarget
				);
				if (SUCCEEDED(hr))
				{

					if (SUCCEEDED(hr))
					{
						hr = DWriteCreateFactory
						(
							DWRITE_FACTORY_TYPE_SHARED,
							__uuidof(wFactory),
							reinterpret_cast<IUnknown**>(&wFactory)
						);
					}
					else
					{
						auto except_str = L"DWriteCreateFactory(...) function failed.\nProcess will be immediately terminated.";
						MessageBox(0, except_str, L"Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
						delete pLib;
						exit(Kernel1_Errors::D2D1DWriteCreateFactory_FAIL);
					}
				}
				else
				{
					auto except_str = L"D2D1CreateHwndRenderTarget(...) function failed.\nProcess will be immediately terminated.";
					MessageBox(0, except_str, L"Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
					delete pLib;
					exit(Kernel1_Errors::D2D1CreateHwndRenderTarget_FAIL);
				}
			}
			else
			{
				auto except_str = L"D2D1CreateFactory(...) function failed.\nProcess will be immediately terminated.";
				MessageBox(0, except_str, L"Kernel1 Module Error!", MB_OK | MB_ICONHAND | MB_APPLMODAL);
				delete pLib;
				exit(Kernel1_Errors::D2D1CreateFactory_FAIL);
			}
		}
		extern "C" _declspec(dllexport) void __stdcall K1_GraphicsUninitialize()
		{
			delete pLib;
		}
		extern "C" _declspec(dllexport) ID2D1HwndRenderTarget* __stdcall K1_GraphicsGetRenderTarget()
		{
			return GET(renderTarget);
		}
		namespace Vector
		{

		}
		namespace Sprite
		{

		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

