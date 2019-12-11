#include "pch.h"
#include <string>
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
#define kernelfunc extern "C" _declspec(dllexport) auto __stdcall

namespace Kernel1
{
	enum Kernel1_Errors
	{
		D2D1_CREATE_FACTORY_ERROR = 1,
		D2D1_CREATE_HWNDRENDERTARGET_ERROR,
		D2D1_CREATE_DWRITEFACTORY_ERROR,
		RGBA_COLOR_ERROR,
		DWRITE_TEXTFORMAT_ERROR,
		DWRITE_ALIGNMENT_ERROR,
		ID2D1_PATHGEOMETRY_ERROR,
		ID2D1_GEOMETRYSINK_OPEN_ERROR,
		ID2D1_GEOMETRYSINK_CLOSE_ERROR,
		WINNT_HWND_HADLE_ERROR,
		D2D1_ENDDRAW_ERROR,
		SPRITE_FILENAME_ERROR,
		SPRITE_INSTANCE_ERROR,
		SPRITE_FACTORY_ERROR,
		SPRITE_DECODER_ERROR,
		SPRITE_CONVERTER_ERROR,
		D2D1_DEVCONTEXT_ERROR,
		D2D1_EFFECT_ERROR,
		D2D1_DEVCONTEXT_ENDDRAW_ERROR
	};
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
		struct RW_ERROR
		{
			RW_ERROR() :
				errcode(0),
				definition(L""),
				header(L""),
				showMsgBox(0),
				dropProcess(0),
				msgBoxType(0)
			{

			}
			RW_ERROR(int code, std::wstring def, bool msgbox, bool drop = true) :
				errcode(code),
				showMsgBox(msgbox),
				dropProcess(drop)
			{
				this->createDefinition(errcode, def);
				if (showMsgBox) throwMsgBox();
			}
			~RW_ERROR()
			{
				memset(this, 0, sizeof(*this));
			}
			void createDefinition(int code, std::wstring def)
			{
				try
				{
					this->definition.clear();
					std::wstring out = L"Во время работы движка произошла ошибка. ";
					out += L"Процесс был немедленно остановлен.\n";
					out += L"Отладочная информация:\n";
					out += (L"Код ошибки: " + std::to_wstring(code) + L"\n");
					out += (L"Комментарий: " + def + L"\n");
					out += L"----  ----  ----  ----\n";
					out += L"Нажмите ОК, чтобы закрыть окно.";
					this->definition = out;
					out.erase();
				}
				catch (std::exception &exc)
				{
					MessageBoxA(NULL, exc.what(), "Двойная критическая ошибка", msgBoxType);
					if (dropProcess) exit(errcode);
				}
			}
			void throwMsgBox()
			{
				if (dropProcess)
				{
					header = L"Уведомление о критической ошибке";
					msgBoxType = MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND;
				}
				else
				{
					header = L"Уведомление о некритической ошибке";
					msgBoxType = MB_OK | MB_ICONWARNING;
				}
				MessageBox(NULL, definition.c_str(), header.c_str(), msgBoxType);
				if (dropProcess) exit(errcode);
			}
			int errcode;
			std::wstring definition;
			std::wstring header;
			bool showMsgBox;
			bool dropProcess;
			long msgBoxType;
		};
		_declspec(selectany) PointersLibrary* pLib;
		kernelfunc K1_GraphicsInitialize
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
						auto errorcode = Kernel1::Kernel1_Errors::D2D1_CREATE_DWRITEFACTORY_ERROR;
						std::wstring definition = L"Kernel1 Module >> DWriteCreateFactory(...) function failed: " + std::to_wstring(hr);
						definition += L".\nProcess will be immediately terminated.";
						RW_ERROR err(errorcode, definition, true, false);
						delete pLib;
						exit(Kernel1::Kernel1_Errors::D2D1_CREATE_DWRITEFACTORY_ERROR);
					}
				}
				else
				{
					auto errorcode = Kernel1::Kernel1_Errors::D2D1_CREATE_HWNDRENDERTARGET_ERROR;
					std::wstring definition = L"Kernel1 Module >> D2D1CreateHwndRenderTarget(...) function failed: " + std::to_wstring(hr);
					definition += L".\nProcess will be immediately terminated.";
					RW_ERROR err(errorcode, definition, true, false);
					delete pLib;
					exit(Kernel1::Kernel1_Errors::D2D1_CREATE_HWNDRENDERTARGET_ERROR);
				}
			}
			else
			{
				auto errorcode = Kernel1::Kernel1_Errors::D2D1_CREATE_FACTORY_ERROR;
				std::wstring definition = L"Kernel1 Module >> D2D1CreateFactory(...) function failed: " + std::to_wstring(hr);
				definition += L".\nProcess will be immediately terminated.";
				RW_ERROR err(errorcode, definition, true, false);
				delete pLib;
				exit(Kernel1::Kernel1_Errors::D2D1_CREATE_FACTORY_ERROR);
			}
		}
		kernelfunc K1_GraphicsUninitialize()
		{
			delete pLib;
		}
		kernelfunc K1_GraphicsGetRenderTarget()
		{
			return GET(renderTarget);
		}
		kernelfunc K1_GetHwnd()
		{
			return GET(targetWindow);
		}
		kernelfunc K1_BeginDraw()
		{
			GET(renderTarget)->BeginDraw();
		}
		kernelfunc K1_EndDraw()
		{
			HRESULT hr = 0l;
			hr = GET(renderTarget)->EndDraw();
			if (FAILED(hr))
			{
				auto errorcode = Kernel1::Kernel1_Errors::D2D1_ENDDRAW_ERROR;
				std::wstring definition = L"Kernel1 Module >> EndDraw(...) function failed: " + std::to_wstring(hr);
				definition += L".\nProcess will be immediately terminated.";
				RW_ERROR err(errorcode, definition, true, false);
				delete pLib;
				exit(Kernel1::Kernel1_Errors::D2D1_ENDDRAW_ERROR);
			}
		}
		kernelfunc K1_EraseScreen()
		{
			GET(renderTarget)->Clear();
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

