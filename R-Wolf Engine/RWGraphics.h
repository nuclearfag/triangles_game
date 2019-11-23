//	Базовый заголовочный файл графической оболочки движка
//	Версия 1.2 Evo, правка 1.
//	Последнее обновление: 9.09.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once
#define WIN32_LEAN_AND_MEAN
#define method inline auto

// Необходимый список заголовков
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <Psapi.h>
#include <wchar.h>

#include <D2D1.h>
#include <d2d1_1.h>
#include <DWrite.h>
#include <DXGI.h>
#include <DirectXMath.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>

#include "RWTimeQuant.h"
#include "RWStringsParser.h"
#include "RWVectorMath.h"

#define _RW_USE_PHYSICS_CONSTANTS
#include "RWPhysicsConstants.h"

#define _RW_USE_GLOBAL_DEFINITIONS
#include "RWGlobalDefinitions.h"

#include <dxgitype.h>
#include <wincodec.h>

using namespace D2D1;
using namespace std;
using namespace DirectX;

using namespace RWMath;

/// <summary>
/// Тестовая графическая оболочка новой архитектуры
/// </summary>
namespace RWGraphics 
{
	#define RELEASE(a) a->Release()
	// Базовые интерфейсы:

	// ID2D1Factory *factory - указатель на базовый интерфейс D2D1
	DECLSPEC_SELECTANY ID2D1Factory*				factory;
	// IDWriteFactory *wFactory - указатель на базовый интерфейс WriteFactory
	DECLSPEC_SELECTANY IDWriteFactory*				wFactory;
	// IDWriteTextFormat *textFormat - указатель на интерфейс формата текста
	DECLSPEC_SELECTANY IDWriteTextFormat*			textFormat;
	// ID2D1HwndRenderTarget *renderTarget - указатель на базовый интерфейс D2D1 и Windows API - окно (Render Target)
	DECLSPEC_SELECTANY ID2D1HwndRenderTarget*		renderTarget;
	// ID2D1SolidColorBrush *brush - буфер для кистей
	DECLSPEC_SELECTANY ID2D1SolidColorBrush*		brush;
	DECLSPEC_SELECTANY ID2D1RadialGradientBrush*	r_brush;
	DECLSPEC_SELECTANY ID2D1LinearGradientBrush*	l_brush;
	// ID2D1PathGeometry *path - буфер для составных фигур
	DECLSPEC_SELECTANY ID2D1PathGeometry*			path;
	DECLSPEC_SELECTANY ID2D1Effect*					effect;
	DECLSPEC_SELECTANY ID2D1DeviceContext*			dc;
	DECLSPEC_SELECTANY ID2D1Device*					dev;
	
	// Вспомогательные структуры и классы:
	/// <summary>
	/// <para>
	/// Тип, отвечающий за представление цвета в 3- и 4-канальном виде
	/// </para>
	/// <para>
	/// По умолчанию Альфа-канал равен FF (255)
	/// </para>
	/// </summary>
	struct RGBA 
	{
		RGBA() :
			red(0),
			green(0),
			blue(0),
			alpha(0)
		{
			
		}
		RGBA(
			USHORT r, 
			USHORT g, 
			USHORT b, 
			USHORT a = 0xFF
		) :
			red(r),
			green(g),
			blue(b),
			alpha(a)
		{
			if (abs(r) <= 0xFF && abs(g) <= 0xFF && abs(b) <= 0xFF && abs(a) <= 0xFF) 
			{
				this->red = abs(r);
				this->green = abs(g);
				this->blue = abs(b);
				this->alpha = abs(a);
			}
			else 
			{
				if (abs(r) >= 0xFF) this->red = 0xFF;
				if (abs(g) >= 0xFF) this->green = 0xFF;
				if (abs(b) >= 0xFF) this->blue = 0xFF;
				if (abs(a) >= 0xFF) this->alpha = 0xFF;
			}
		}
		~RGBA() 
		{
			memset(this, 0, sizeof(*this));
		}
		USHORT red;
		USHORT green;
		USHORT blue;
		USHORT alpha;
	};

	struct TWDLINE 
	{
		TWDLINE() :
			from(0.f, 0.f),
			to(0.f, 0.f)
		{
			
		}
		TWDLINE(
			FVECTOR2 _from,
			FVECTOR2 _to
		) :
			from(_from),
			to(_to)
		{
			
		}
		~TWDLINE() 
		{
			memset(this, 0, sizeof(*this));
		}
		FVECTOR2 from;
		FVECTOR2 to;
	};

	struct TWDRECT 
	{
		TWDRECT() :
			from(0.f, 0.f),
			to(0.f, 0.f)
		{
			
		}
		TWDRECT(
			FVECTOR2 _from, 
			FVECTOR2 _to
		) :
			from(_from),
			to(_to)
		{
			
		}
		~TWDRECT()
		{
			memset(this, 0, sizeof(*this));
		}
		method height()
		{
			return abs(this->to.y - this->from.y);
		}
		method width()
		{
			return abs(this->to.x - this->from.x);
		}
		FVECTOR2 from;
		FVECTOR2 to;
	};

	struct TWDELLIPSE 
	{
		
		TWDELLIPSE() :
			center(0.f, 0.f),
			radiusX(0.f),
			radiusY(0.f)
		{
			
		}
		TWDELLIPSE(
			FVECTOR2 _center,
			float rX,
			float rY
		) :
			center(_center),
			radiusX(rX),
			radiusY(rY)
		{
			
		}
		~TWDELLIPSE() 
		{
			memset(this, 0, sizeof(*this));
		}
		FVECTOR2 center;
		float radiusX;
		float radiusY;
	};
	
	struct NPOLYGON 
	{
		NPOLYGON() :
			vertices(0)
		{
			this->vertexArray.reserve(this->vertices);
		}
		NPOLYGON(
			int _vertices, 
			vector<FVECTOR2> _varray
		) : 
			vertices(_vertices),
			vertexArray(_varray)
		{

		}
		~NPOLYGON()
		{
			memset(this, 0, sizeof(*this));
		}
		unsigned int vertices;
		vector<FVECTOR2> vertexArray;
	};

	struct FONTPROPERTY 
	{
		FONTPROPERTY() :
			name(L"Consolas"),
			size(12.f),
			alignment(DWRITE_TEXT_ALIGNMENT_LEADING),
			weight(DWRITE_FONT_WEIGHT_NORMAL),
			style(DWRITE_FONT_STYLE_NORMAL),
			stretch(DWRITE_FONT_STRETCH_NORMAL)
		{
			
		}
		FONTPROPERTY(
			LPCWSTR _name, 
			float _size,
			DWRITE_TEXT_ALIGNMENT _align,
			DWRITE_FONT_WEIGHT _weight,
			DWRITE_FONT_STYLE _style,
			DWRITE_FONT_STRETCH _stretch
		) :
			name(_name),
			size(_size),
			alignment(_align),
			weight(_weight),
			style(_style),
			stretch(_stretch)
		{
			
		}
		~FONTPROPERTY()
		{
			memset(this, 0, sizeof(*this));
		}
		LPCWSTR name;
		float size;
		DWRITE_TEXT_ALIGNMENT alignment;
		DWRITE_FONT_WEIGHT weight;
		DWRITE_FONT_STYLE style;
		DWRITE_FONT_STRETCH stretch;
	};

	struct LAGOMETER 
	{
		struct TIME 
		{
			TIME() :
				from(0.f),
				to(0.f)
			{
				
			}
			~TIME()
			{
				memset(this, 0, sizeof(*this));
			}
			float from;
			float to;
		};
		LAGOMETER() :
			sp(),
			lastCallMsec(0)
		{
			float lo = static_cast<float>(getConfigString(ScreenSize_Y)) - 80.0f;
			for (int iter = 0; iter < 251; iter++)
			{
				graphCoord[iter] = FVECTOR2(10.0f + 2.0f * iter, lo);
			}
		}
		~LAGOMETER() 
		{
			memset(this, 0, sizeof(*this));
		}
		method setTimeFrom()
		{
			GetSystemTime(&this->sp);
			this->t.from = sp.wMilliseconds;
		}
		method setTimeTo()
		{
			GetSystemTime(&this->sp);
			this->t.to = sp.wMilliseconds;
		}
		method getDifference()
		{
			return abs(this->t.to - this->t.from);
		}
		method setDifference(int diff = 1)
		{
			this->t.to += diff;
		}
		method setLastCallMsec(int value)
		{
			this->lastCallMsec = value;
		}
		TIME t;
		SYSTEMTIME sp;
		FVECTOR2 graphCoord[251];
		int lastRecordedGraph = -1;
		int lastCallMsec;
	};

	struct RW_ERROR 
	{
		RW_ERROR() : 
			errcode(0), 
			definition(""),
			header(""),
			showMsgBox(0),
			dropProcess(0),
			msgBoxType(0)
		{

		}
		RW_ERROR(int code, string def, bool msgbox, bool drop = true) : 
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
		void createDefinition(int code, string def)
		{
			this->definition.clear();
			string out = "Во время работы движка произошла ошибка. ";
			out += "Процесс был немедленно остановлен.\n";
			out += "Отладочная информация:\n";
			out += ("Код ошибки: " + to_string(code) + "\n");
			out += ("Комментарий: " + def + "\n");
			out += "----  ----  ----  ----\n";
			out += "Нажмите ОК, чтобы закрыть окно.";
			this->definition = out;
			out.erase();
		}
		void throwMsgBox()
		{
			if (dropProcess)
			{
				header = "Уведомление о критической ошибке";
				msgBoxType = MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND;
			}
			else
			{
				header = "Уведомление о некритической ошибке";
				msgBoxType = MB_OK | MB_ICONWARNING;
			}
			MessageBox(NULL, definition.c_str(), header.c_str(), msgBoxType);
			if (dropProcess) exit(errcode);
		}
		int errcode;
		string definition;
		string header;
		bool showMsgBox;
		bool dropProcess;
		long msgBoxType;
	};

	struct LIGHT_ROUND
	{
		RGBA color;
		float intensity;
		FVECTOR2 position;
		float radius;
	};

	// Вспомогательный набор функций
	/// <summary>
	/// Возвращает максимальное количество символов в строке
	/// <para>
	/// [in] : строка для анализа
	/// </para>
	/// <para>
	/// -> максимальное количество символов до переноса в одной строке
	/// </para>
	/// </summary>
	method strwidth(wstring in)
	{
		LPCWSTR buffer = in.c_str();
		int maxSymbols = 1, maxSymbolsInRow = 1;
		for (int i = 0; i < wcslen(buffer); i++) 
		{
			if (buffer[i] != L'\n') maxSymbolsInRow++;
			else 
			{
				if (maxSymbolsInRow > maxSymbols) maxSymbols = maxSymbolsInRow;
				maxSymbolsInRow = 0;
			}
			if ((i == wcslen(buffer) - 1) && (maxSymbols == 0))
				return maxSymbolsInRow;
		}
		return maxSymbols;
	}

	/// <summary>
	/// Возвращает максимальное количество символов в строке
	/// <para>
	/// [in] : строка для анализа
	/// </para>
	/// <para>
	/// -> максимальное количество столбцов
	/// </para>
	/// </summary>
	method strheight(wstring in) 
	{
		LPCWSTR buffer = in.c_str();
		int maxSymbols = 1;
		for (int i = 0; i < wcslen(buffer) - 1; i++) 
		{
			if (buffer[i] == L'\n') maxSymbols++;
		}
		return maxSymbols;
	}

	/// <summary>
	/// <para>
	/// Выполняет преобразование RGBA-записи цвета в D2D1_COLOR_F
	/// </para>
	/// <para>
	/// [color] : инициализированный объект структуры RGBA
	/// </para>
	/// <para>
	/// -> конвертированный в D2D1_COLOR_F тип представления цвета
	/// </para>
	/// </summary>
	method rtod(RGBA color) 
	{
		return ColorF
		(
			color.red / 255.0f,
			color.green / 255.0f,
			color.blue / 255.0f,
			color.alpha / 255.0f
		);
	}

	/// <summary>
	/// <para>
	/// Выполняет обратное rtod() преобразование цвета
	/// </para>
	/// </summary>
	method dtor(D2D1_COLOR_F color) 
	{
		return RGBA
		(
			color.r * 0xFF,
			color.g * 0xFF,
			color.b * 0xFF,
			color.a * 0xFF
		);
	}

	method scrwidth() 
	{
		return static_cast<float>(getConfigString(ScreenSize_X));
	}

	method scrheight() 
	{
		return static_cast<float>(getConfigString(ScreenSize_Y));
	}
	// Вспомогательные пространства имён
	/// <summary>
	/// <para>
	/// Коллекция предопределённых эталонных цветов
	/// </para>
	/// </summary>
	namespace Colors 
	{
		const RGBA white = dtor(ColorF(ColorF::White));
		const RGBA snow = dtor(ColorF(ColorF::Snow));
		const RGBA honeydew = dtor(ColorF(ColorF::Honeydew));
		const RGBA mintCream = dtor(ColorF(ColorF::MintCream));
		const RGBA azure = dtor(ColorF(ColorF::Azure));
		const RGBA aliceBlue = dtor(ColorF(ColorF::AliceBlue));
		const RGBA ghostWhite = dtor(ColorF(ColorF::GhostWhite));
		const RGBA whiteSmoke = dtor(ColorF(ColorF::WhiteSmoke));
		const RGBA seaShell = dtor(ColorF(ColorF::SeaShell));
		const RGBA beige = dtor(ColorF(ColorF::Beige));
		const RGBA oldLace = dtor(ColorF(ColorF::OldLace));
		const RGBA floralWhite = dtor(ColorF(ColorF::FloralWhite));
		const RGBA ivory = dtor(ColorF(ColorF::Ivory));
		const RGBA antiqueWhite = dtor(ColorF(ColorF::AntiqueWhite));
		const RGBA linen = dtor(ColorF(ColorF::Linen));
		const RGBA mistyRose = dtor(ColorF(ColorF::MistyRose));
		const RGBA lavenderBlush = dtor(ColorF(ColorF::LavenderBlush));
		const RGBA gainsboro = dtor(ColorF(ColorF::Gainsboro));
		const RGBA lightGray = dtor(ColorF(ColorF::LightGray));
		const RGBA silver = dtor(ColorF(ColorF::Silver));
		const RGBA darkGray = dtor(ColorF(ColorF::DarkGray));
		const RGBA gray = dtor(ColorF(ColorF::Gray));
		const RGBA dimGray = dtor(ColorF(ColorF::DimGray));
		const RGBA lightSlateGray = dtor(ColorF(ColorF::LightSlateGray));
		const RGBA slateGray = dtor(ColorF(ColorF::SlateGray));
		const RGBA darkSlateGray = dtor(ColorF(ColorF::DarkSlateGray));
		const RGBA black = dtor(ColorF(ColorF::Black));
		const RGBA indianRed = dtor(ColorF(ColorF::IndianRed));
		const RGBA lightCoral = dtor(ColorF(ColorF::LightCoral));
		const RGBA salmon = dtor(ColorF(ColorF::Salmon));
		const RGBA darkSalmon = dtor(ColorF(ColorF::DarkSalmon));
		const RGBA lightSalmon = dtor(ColorF(ColorF::LightSalmon));
		const RGBA crimson = dtor(ColorF(ColorF::Crimson));
		const RGBA red = dtor(ColorF(ColorF::Red));
		const RGBA firebrick = dtor(ColorF(ColorF::Firebrick));
		const RGBA darkRed = dtor(ColorF(ColorF::DarkRed));
		const RGBA pink = dtor(ColorF(ColorF::Pink));
		const RGBA lightPink = dtor(ColorF(ColorF::LightPink));
		const RGBA hotPink = dtor(ColorF(ColorF::HotPink));
		const RGBA deepPink = dtor(ColorF(ColorF::DeepPink));
		const RGBA mediumVioletRed = dtor(ColorF(ColorF::MediumVioletRed));
		const RGBA paleVioletRed = dtor(ColorF(ColorF::PaleVioletRed));
		const RGBA coral = dtor(ColorF(ColorF::Coral));
		const RGBA tomato = dtor(ColorF(ColorF::Tomato));
		const RGBA orangeRed = dtor(ColorF(ColorF::OrangeRed));
		const RGBA darkOrange = dtor(ColorF(ColorF::DarkOrange));
		const RGBA orange = dtor(ColorF(ColorF::Orange));
		const RGBA gold = dtor(ColorF(ColorF::Gold));
		const RGBA yellow = dtor(ColorF(ColorF::Yellow));
		const RGBA lightYellow = dtor(ColorF(ColorF::LightYellow));
		const RGBA lemonChiffon = dtor(ColorF(ColorF::LemonChiffon));
		const RGBA lightGoldenrodYellow = dtor(ColorF(ColorF::LightGoldenrodYellow));
		const RGBA papayaWhip = dtor(ColorF(ColorF::PapayaWhip));
		const RGBA moccasin = dtor(ColorF(ColorF::Moccasin));
		const RGBA peachPuff = dtor(ColorF(ColorF::PeachPuff));
		const RGBA paleGoldenrod = dtor(ColorF(ColorF::PaleGoldenrod));
		const RGBA khaki = dtor(ColorF(ColorF::Khaki));
		const RGBA lavender = dtor(ColorF(ColorF::Lavender));
		const RGBA thistle = dtor(ColorF(ColorF::Thistle));
		const RGBA plum = dtor(ColorF(ColorF::Plum));
		const RGBA violet = dtor(ColorF(ColorF::Violet));
		const RGBA orchid = dtor(ColorF(ColorF::Orchid));
		const RGBA magenta = dtor(ColorF(ColorF::Magenta));
		const RGBA mediumOrchid = dtor(ColorF(ColorF::MediumOrchid));
		const RGBA mediumPurple = dtor(ColorF(ColorF::MediumPurple));
		const RGBA blueViolet = dtor(ColorF(ColorF::BlueViolet));
		const RGBA darkViolet = dtor(ColorF(ColorF::DarkViolet));
		const RGBA darkOrchid = dtor(ColorF(ColorF::DarkOrchid));
		const RGBA darkMagenta = dtor(ColorF(ColorF::DarkMagenta));
		const RGBA purple = dtor(ColorF(ColorF::Purple));
		const RGBA indigo = dtor(ColorF(ColorF::Indigo));
		const RGBA slateBlue = dtor(ColorF(ColorF::SlateBlue));
		const RGBA darkSlateBlue = dtor(ColorF(ColorF::DarkSlateBlue));
		const RGBA cornsilk = dtor(ColorF(ColorF::Cornsilk));
		const RGBA blanchedAlmond = dtor(ColorF(ColorF::BlanchedAlmond));
		const RGBA bisque = dtor(ColorF(ColorF::Bisque));
		const RGBA navajoWhite = dtor(ColorF(ColorF::NavajoWhite));
		const RGBA wheat = dtor(ColorF(ColorF::Wheat));
		const RGBA burlyWood = dtor(ColorF(ColorF::BurlyWood));
		const RGBA tan = dtor(ColorF(ColorF::Tan));
		const RGBA rosyBrown = dtor(ColorF(ColorF::RosyBrown));
		const RGBA sandyBrown = dtor(ColorF(ColorF::SandyBrown));
		const RGBA goldenrod = dtor(ColorF(ColorF::Goldenrod));
		const RGBA darkGoldenrod = dtor(ColorF(ColorF::DarkGoldenrod));
		const RGBA peru = dtor(ColorF(ColorF::Peru));
		const RGBA chocolate = dtor(ColorF(ColorF::Chocolate));
		const RGBA saddleBrown = dtor(ColorF(ColorF::SaddleBrown));
		const RGBA sienna = dtor(ColorF(ColorF::Sienna));
		const RGBA brown = dtor(ColorF(ColorF::Brown));
		const RGBA maroon = dtor(ColorF(ColorF::Maroon));
		const RGBA greenYellow = dtor(ColorF(ColorF::GreenYellow));
		const RGBA chartreuse = dtor(ColorF(ColorF::Chartreuse));
		const RGBA lawnGreen = dtor(ColorF(ColorF::LawnGreen));
		const RGBA lime = dtor(ColorF(ColorF::Lime));
		const RGBA limeGreen = dtor(ColorF(ColorF::LimeGreen));
		const RGBA paleGreen = dtor(ColorF(ColorF::PaleGreen));
		const RGBA lightGreen = dtor(ColorF(ColorF::LightGreen));
		const RGBA mediumSpringGreen = dtor(ColorF(ColorF::MediumSpringGreen));
		const RGBA springGreen = dtor(ColorF(ColorF::SpringGreen));
		const RGBA mediumSeaGreen = dtor(ColorF(ColorF::MediumSeaGreen));
		const RGBA seaGreen = dtor(ColorF(ColorF::SeaGreen));
		const RGBA forestGreen = dtor(ColorF(ColorF::ForestGreen));
		const RGBA green = dtor(ColorF(ColorF::Green));
		const RGBA darkGreen = dtor(ColorF(ColorF::DarkGreen));
		const RGBA yellowGreen = dtor(ColorF(ColorF::YellowGreen));
		const RGBA oliveDrab = dtor(ColorF(ColorF::OliveDrab));
		const RGBA olive = dtor(ColorF(ColorF::Olive));
		const RGBA darkOliveGreen = dtor(ColorF(ColorF::DarkOliveGreen));
		const RGBA mediumAquamarine = dtor(ColorF(ColorF::MediumAquamarine));
		const RGBA darkSeaGreen = dtor(ColorF(ColorF::DarkSeaGreen));
		const RGBA lightSeaGreen = dtor(ColorF(ColorF::LightSeaGreen));
		const RGBA darkCyan = dtor(ColorF(ColorF::DarkCyan));
		const RGBA teal = dtor(ColorF(ColorF::Teal));
		const RGBA cyan = dtor(ColorF(ColorF::Cyan));
		const RGBA lightCyan = dtor(ColorF(ColorF::LightCyan));
		const RGBA paleTurquoise = dtor(ColorF(ColorF::PaleTurquoise));
		const RGBA aquamarine = dtor(ColorF(ColorF::Aquamarine));
		const RGBA turquoise = dtor(ColorF(ColorF::Turquoise));
		const RGBA mediumTurquoise = dtor(ColorF(ColorF::MediumTurquoise));
		const RGBA darkTurquoise = dtor(ColorF(ColorF::DarkTurquoise));
		const RGBA cadetBlue = dtor(ColorF(ColorF::CadetBlue));
		const RGBA steelBlue = dtor(ColorF(ColorF::SteelBlue));
		const RGBA lightSteelBlue = dtor(ColorF(ColorF::LightSteelBlue));
		const RGBA powderBlue = dtor(ColorF(ColorF::PowderBlue));
		const RGBA lightBlue = dtor(ColorF(ColorF::LightBlue));
		const RGBA skyBlue = dtor(ColorF(ColorF::SkyBlue));
		const RGBA lightSkyBlue = dtor(ColorF(ColorF::LightSkyBlue));
		const RGBA deepSkyBlue = dtor(ColorF(ColorF::DeepSkyBlue));
		const RGBA dodgerBlue = dtor(ColorF(ColorF::DodgerBlue));
		const RGBA cornflowerBlue = dtor(ColorF(ColorF::CornflowerBlue));
		const RGBA mediumSlateBlue = dtor(ColorF(ColorF::MediumSlateBlue));
		const RGBA royalBlue = dtor(ColorF(ColorF::RoyalBlue));
		const RGBA blue = dtor(ColorF(ColorF::Blue));
		const RGBA mediumBlue = dtor(ColorF(ColorF::MediumBlue));
		const RGBA darkBlue = dtor(ColorF(ColorF::DarkBlue));
		const RGBA navy = dtor(ColorF(ColorF::Navy));
		const RGBA midnightBlue = dtor(ColorF(ColorF::MidnightBlue));
	}
	/// <summary>
	/// <para>
	/// Коллекция предопределённых шрифтов
	/// </para>
	/// </summary>
	namespace Fonts 
	{
		static FONTPROPERTY consolas = FONTPROPERTY();
		static FONTPROPERTY fixedsys
		(
			L"Fixedsys Excelsior 3.01",
			16.0f,
			DWRITE_TEXT_ALIGNMENT_LEADING,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL
		);
		static FONTPROPERTY roboto
		(
			L"Roboto",
			14.0f,
			DWRITE_TEXT_ALIGNMENT_LEADING,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL
		);
	}
	enum Errcodes 
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
	// Базовый набор функций
	method initialization(HWND windowHandle) 
	{
		// Подготовка переменных к инициализации
		factory = NULL;
		wFactory = NULL;
		brush = NULL;
		path = NULL;
		renderTarget = NULL;
		HRESULT debug = 0L;
		// Инициализация D2D1:
		debug = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
		if (SUCCEEDED(debug)) 
		{
			debug = factory->CreateHwndRenderTarget
			(
				RenderTargetProperties(),
				HwndRenderTargetProperties
				(
					windowHandle,
					SizeU
					(
						getConfigString(ScreenSize_X),
						getConfigString(ScreenSize_Y)
					)
				),
				&renderTarget
			);
			if (SUCCEEDED(debug)) 
			{
				debug = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(wFactory),
					reinterpret_cast<IUnknown**>(&wFactory));
				if (SUCCEEDED(debug)) 
				{
					return true;
				}
				else 
				{
					auto errorcode = Errcodes::D2D1_CREATE_DWRITEFACTORY_ERROR;
					string definition = "D2D1_CREATE_DWRITEFACTORY_ERROR: " + to_string(debug);
					RW_ERROR err(errorcode, definition, 1);
					exit(errorcode);
				}
			}
			else 
			{
				auto errorcode = Errcodes::D2D1_CREATE_HWNDRENDERTARGET_ERROR;
				string definition = "D2D1_CREATE_HWNDRENDERTARGET_ERROR: " + to_string(debug);
				RW_ERROR err(errorcode, definition, 1);
				exit(errorcode);
			}
		}
		else
		{
			auto errorcode = Errcodes::D2D1_CREATE_FACTORY_ERROR;
			string definition = "D2D1_CREATE_FACTORY_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method uninitialization() 
	{
		RELEASE(factory);
		RELEASE(wFactory);
		RELEASE(brush);
		RELEASE(path);
		RELEASE(renderTarget);
		RELEASE(dc);
		RELEASE(effect);
		RELEASE(r_brush);
		RELEASE(l_brush);
	}

	method getRenderTarget() 
	{
		return renderTarget;
	}

	method getEngineHwnd() 
	{
		return renderTarget->GetHwnd();
	}

	method beginDraw() 
	{
		renderTarget->BeginDraw();
	}

	method endDraw() 
	{
		HRESULT debug = 0L;
		debug = renderTarget->EndDraw();
		if (SUCCEEDED(debug)) 
		{
			return true;
		}
		else 
		{
			auto errorcode = Errcodes::D2D1_ENDDRAW_ERROR;
			string definition = "D2D1_ENDDRAW_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method clscr(RGBA color) 
	{
		renderTarget->Clear(rtod(color));
	}

	method matrixRotation(FVECTOR2 point, float angle) 
	{
		renderTarget->SetTransform(Matrix3x2F::Rotation(angle, Point2F(point.x, point.y)));
	}

	method matrixRestore() 
	{
		renderTarget->SetTransform(Matrix3x2F::Identity());
	}

	method line(TWDLINE line, float thickness, RGBA color) 
	{
		HRESULT debug = 0L;
		debug = renderTarget->CreateSolidColorBrush(rtod(color), &brush);
		if (SUCCEEDED(debug)) 
		{
			renderTarget->DrawLine
			(
				Point2F(line.from.x, line.from.y),
				Point2F(line.to.x, line.to.y),
				brush,
				thickness
			);
			RELEASE(brush);
		}
		else 
		{
			auto errorcode = Errcodes::RGBA_COLOR_ERROR;
			string definition = "RGBA_COLOR_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method rect(TWDRECT rect, RGBA color, float thickness, bool fill = true) 
	{
		HRESULT debug = 0L;
		debug = renderTarget->CreateSolidColorBrush(rtod(color), &brush);
		if (SUCCEEDED(debug)) 
		{
			if (fill) 
				renderTarget->DrawRectangle
				(
					RectF(rect.from.x, rect.from.y, rect.to.x, rect.to.y),
					brush,
					thickness
				);
			else
				renderTarget->FillRectangle
				(
					RectF(rect.from.x, rect.from.y, rect.to.x, rect.to.y),
					brush
				);
			brush->Release();
		}
		else 
		{
			auto errorcode = Errcodes::RGBA_COLOR_ERROR;
			string definition = "RGBA_COLOR_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method ellipse(TWDELLIPSE circle, RGBA color, float thickness, bool fill = true) 
	{
		HRESULT debug = 0L;
		debug = renderTarget->CreateSolidColorBrush(rtod(color), &brush);
		if (SUCCEEDED(debug)) 
		{
			if (fill)
				renderTarget->FillEllipse
				(
					Ellipse
					(
						Point2F(circle.center.x, circle.center.y),
						circle.radiusX,
						circle.radiusY
					),
					brush
				);
			else
				renderTarget->DrawEllipse
				(
					Ellipse
					(
						Point2F(circle.center.x, circle.center.y),
						circle.radiusX,
						circle.radiusY
					),
					brush,
					thickness
				);
			RELEASE(brush);
		}
		else 
		{
			auto errorcode = Errcodes::RGBA_COLOR_ERROR;
			string definition = "RGBA_COLOR_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method fade(RGBA color, float percent = 1.0f) 
	{
		color.alpha = 0xFF * abs(percent);
		rect
		(
			TWDRECT
			(
				FVECTOR2(0.0f, 0.0f), 
				FVECTOR2
				(
					scrwidth(), 
					scrheight()
				)
			),
			color,
			1.0f,
			true
		);
	}

	method text(wstring text, FVECTOR2 pos, RGBA color, FONTPROPERTY fp) 
	{
		IDWriteTextFormat* idwtf;
		HRESULT debug = 0L;
		debug = renderTarget->CreateSolidColorBrush(rtod(color), &brush);
		if (SUCCEEDED(debug)) 
		{
			debug = wFactory->CreateTextFormat
			(
				fp.name,
				NULL,
				fp.weight,
				fp.style,
				fp.stretch,
				fp.size,
				L"",
				&idwtf
			);
			if (SUCCEEDED(debug)) 
			{
				debug = idwtf->SetTextAlignment(fp.alignment);
				if (SUCCEEDED(debug)) {
					renderTarget->DrawTextA
					(
						text.c_str(), text.length(), idwtf,
						RectF
						(
							pos.x, pos.y,
							pos.x + fp.size * strwidth(text),
							pos.y + fp.size * strheight(text)
						),
						brush
					);
					idwtf->Release();
					brush->Release();
				}
				else 
				{
					auto errorcode = Errcodes::DWRITE_ALIGNMENT_ERROR;
					string definition = "DWRITE_ALIGNMENT_ERROR: " + to_string(debug);
					RW_ERROR err(errorcode, definition, 1);
					exit(errorcode);
				}
			}
			else 
			{
				auto errorcode = Errcodes::DWRITE_TEXTFORMAT_ERROR;
				string definition = "DWRITE_TEXTFORMAT_ERROR: " + to_string(debug);
				RW_ERROR err(errorcode, definition, 1);
				exit(errorcode);
			}
		}
		else {
			auto errorcode = Errcodes::RGBA_COLOR_ERROR;
			string definition = "RGBA_COLOR_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method npolygon(NPOLYGON polygon, RGBA color, float thickness, bool fill = true) 
	{
		HRESULT debug = 0L;
		debug = renderTarget->CreateSolidColorBrush(rtod(color), &brush);
		ID2D1GeometrySink* idgsk;
		D2D1_FIGURE_BEGIN ddfb;
		if (fill)
			ddfb = D2D1_FIGURE_BEGIN_FILLED;
		else
			ddfb = D2D1_FIGURE_BEGIN_HOLLOW;
		if (SUCCEEDED(debug)) 
		{
			debug = factory->CreatePathGeometry(&path);
			if (SUCCEEDED(debug)) 
			{
				debug = path->Open(&idgsk);
				if (SUCCEEDED(debug)) 
				{
					idgsk->BeginFigure
					(
						Point2F
						(
							polygon.vertexArray[0].x,
							polygon.vertexArray[0].y
						), 
						ddfb
					);
					for (int iter = 1; iter < polygon.vertices; iter++) 
					{
						idgsk->AddLine
						(
							Point2F
							(
								polygon.vertexArray[iter].x,
								polygon.vertexArray[iter].y
							)
						);
					}
					idgsk->EndFigure(D2D1_FIGURE_END_CLOSED);
					debug = idgsk->Close();
					if (SUCCEEDED(debug)) 
					{
						if (fill) 
						{
							renderTarget->FillGeometry(path, brush);
						}
						else 
						{
							renderTarget->DrawGeometry(path, brush, thickness);
						}
						RELEASE(idgsk);
						RELEASE(path);
						RELEASE(brush);
					}
					else 
					{
						auto errorcode = Errcodes::ID2D1_GEOMETRYSINK_CLOSE_ERROR;
						string definition = "ID2D1_GEOMETRYSINK_CLOSE_ERROR: " + to_string(debug);
						RW_ERROR err(errorcode, definition, 1);
						exit(errorcode);
					}
				}
				else 
				{
					auto errorcode = Errcodes::ID2D1_GEOMETRYSINK_OPEN_ERROR;
					string definition = "ID2D1_GEOMETRYSINK_OPEN_ERROR: " + to_string(debug);
					RW_ERROR err(errorcode, definition, 1);
					exit(errorcode);
				}
			}
			else 
			{
				auto errorcode = Errcodes::ID2D1_PATHGEOMETRY_ERROR;
				string definition = "ID2D1_PATHGEOMETRY_ERROR: " + to_string(debug);
				RW_ERROR err(errorcode, definition, 1);
				exit(errorcode);
			}
		}
		else 
		{
			auto errorcode = Errcodes::RGBA_COLOR_ERROR;
			string definition = "RGBA_COLOR_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method lagometer(LAGOMETER* &obj)
	{
		MEMORYSTATUSEX msex;
		PROCESS_MEMORY_COUNTERS pmc;
		msex.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&msex);
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(PROCESS_MEMORY_COUNTERS));
		unsigned long long mem_all = msex.ullAvailPhys / pow(1024, 2);
		unsigned long long mem_cur = pmc.WorkingSetSize / pow(1024, 2);

		float lo = static_cast<float>(getConfigString(ScreenSize_Y)) - 20.0f;

		if (!obj->getDifference()) obj->setDifference();
		
		wstring out = L"Lagometer enabled\nAverage FPS: ";
		out += to_wstring(1000 / obj->getDifference());
		out += L" frames.\n";
		out += L"Random access memory used: ";
		out += to_wstring(mem_cur);
		out += L"Mb of ";
		out += to_wstring(mem_all);
		out += L"Mb.\n";
		wstring up = L"1K FPS.\n";
		wstring down = L"1 FPS.\n";
		text(out, FVECTOR2(10.0f, lo - 175.0f), Colors::yellowGreen, Fonts::consolas);

		if (obj->lastRecordedGraph != 250) 
		{
			obj->graphCoord[obj->lastRecordedGraph + 1] = FVECTOR2
			(10.0f + 2.0f * (obj->lastRecordedGraph + 1), lo - 120.0f / obj->getDifference());
			obj->lastRecordedGraph++;
		}
		else 
		{
			obj->graphCoord[0] = FVECTOR2(10.0f, lo - 120.0f / obj->getDifference());
			obj->lastRecordedGraph = 0;
		}

		for (int iter = 0; iter < 250; iter++) 
		{
			line
			(
				TWDLINE
				(
					FVECTOR2
					(
						obj->graphCoord[iter].x,
						obj->graphCoord[iter].y
					),
					FVECTOR2
					(
						obj->graphCoord[iter + 1].x,
						obj->graphCoord[iter + 1].y
					)
				),
				1.0f,
				Colors::yellow
			);
		}

		line
		(
			TWDLINE
			(
				FVECTOR2
				(
					10.0f, 
					lo - 120.0f
				), 
				FVECTOR2
				(
					10.0f, 
					lo
				)
			), 
			1.0f, 
			Colors::lightGray
		);
		line
		(
			TWDLINE
			(
				FVECTOR2
				(
					510.0f, 
					lo - 120.0f
				), 
				FVECTOR2
				(
					510.0f,
					lo
				)
			), 
			1.0f,
			Colors::lightGray
		);
		line
		(
			TWDLINE
			(
				FVECTOR2
				(
					10.0f, 
					lo - 120.0f
				), 
				FVECTOR2
				(
					510.0f, lo - 120.0f
				)
			), 
			2.0f, 
			Colors::yellowGreen
		);
		line
		(
			TWDLINE
			(
				FVECTOR2
				(
					10.0f, 
					lo - 60.0f
				), 
				FVECTOR2
				(
					510.0f, lo - 60.0f
				)
			), 
			1.0f,
			Colors::lightGray
		);
		line
		(
			TWDLINE
			(
				FVECTOR2
				(
					10.0f, 
					lo
				), 
				FVECTOR2
				(
					510.0f, lo
				)
			), 
			2.0f, 
			Colors::red
		);
	}

	method lightEffect()
	{
		HRESULT debug = 0L;
		debug = dc->CreateEffect(CLSID_D2D1Brightness, &effect);
		if (SUCCEEDED(debug))
		{
			debug = effect->SetValue(D2D1_BRIGHTNESS_PROP_BLACK_POINT, Vector2F(0.0f, 0.2f));
			if (SUCCEEDED(debug))
			{
				dc->BeginDraw();
				dc->DrawImage(effect);
			}
			else
			{
				auto errorcode = Errcodes::D2D1_EFFECT_ERROR;
				string definition = "D2D1_EFFECT_ERROR: " + to_string(debug);
				RW_ERROR err(errorcode, definition, 1);
				exit(errorcode);
			}
		}
		else
		{
			auto errorcode = Errcodes::D2D1_DEVCONTEXT_ERROR;
			string definition = "D2D1_DEVCONTEXT_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	method endEffect()
	{
		HRESULT debug = 0L;
		debug = dc->EndDraw();
		if (SUCCEEDED(debug))
		{
			RELEASE(effect);
		}
		else
		{
			auto errorcode = Errcodes::D2D1_DEVCONTEXT_ERROR;
			string definition = "D2D1_DEVCONTEXT_ERROR: " + to_string(debug);
			RW_ERROR err(errorcode, definition, 1);
			exit(errorcode);
		}
	}

	namespace Sprite 
	{
		method picture(LPCWSTR path, FVECTOR2 pos, float opacity, float size = 1.0f) 
		{
			HRESULT debug = 0L;
			ID2D1Bitmap* bmp = NULL;
			IWICImagingFactory* wicFactory = NULL;
			IWICBitmapDecoder* wicDecoder = NULL;
			IWICBitmapFrameDecode* wicFrame = NULL;
			IWICFormatConverter* wicConverter = NULL;
			// Создание фабрики WIC:
			debug = CoCreateInstance
			(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				(LPVOID*)& wicFactory
			);
			if (SUCCEEDED(debug))
			{
				// Создание декодера:
				debug = wicFactory->CreateDecoderFromFilename
				(
					path,
					NULL,
					GENERIC_READ,
					WICDecodeMetadataCacheOnLoad,
					&wicDecoder
				);
				if (SUCCEEDED(debug))
				{
					// Чтение изображения:
					debug = wicDecoder->GetFrame(0, &wicFrame);
					if (SUCCEEDED(debug))
					{
						// Создание конвертёра:
						debug = wicFactory->CreateFormatConverter(&wicConverter);
						if (SUCCEEDED(debug))
						{
							// Настройка конвертёра:
							debug = wicConverter->Initialize
							(
								wicFrame,
								GUID_WICPixelFormat32bppPBGRA,
								WICBitmapDitherTypeNone,
								NULL,
								0.0,
								WICBitmapPaletteTypeCustom
							);
							if (SUCCEEDED(debug))
							{
								// Конвертация в формат ID2D1Bitmap:
								debug = RWGraphics::getRenderTarget()->CreateBitmapFromWicBitmap
								(
									wicConverter,
									NULL,
									&bmp
								);
								if (SUCCEEDED(debug))
								{
									// Проверка на видимость:
									if 
									(
										pos.x + bmp->GetSize().width > 0 &&
										pos.y + bmp->GetSize().height > 0 &&
										pos.x < getConfigString(ScreenSize_X) &&
										pos.y < getConfigString(ScreenSize_Y)
									)
									{
										// Рисование изображения:
										RWGraphics::getRenderTarget()->DrawBitmap
										(
											bmp,
											RectF
											(
												pos.x, pos.y,
												pos.x + bmp->GetSize().width * size,
												pos.y + bmp->GetSize().height * size
											),
											opacity / 255.f,
											D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
											RectF
											(
												0, 0,
												bmp->GetSize().width, bmp->GetSize().height
											)
										);
									}
								}
								else
								{
									auto errorcode = Errcodes::SPRITE_CONVERTER_ERROR;
									string definition = "SPRITE_CONVERTER_ERROR: " + to_string(debug);
									RW_ERROR err(errorcode, definition, 1);
									exit(errorcode);
								}
							}
							else
							{
								auto errorcode = Errcodes::SPRITE_CONVERTER_ERROR;
								string definition = "SPRITE_CONVERTER_ERROR: " + to_string(debug);
								RW_ERROR err(errorcode, definition, 1);
								exit(errorcode);
							}
						}
						else
						{
							auto errorcode = Errcodes::SPRITE_FACTORY_ERROR;
							string definition = "SPRITE_FACTORY_ERROR: " + to_string(debug);
							RW_ERROR err(errorcode, definition, 1);
							exit(errorcode);
						}
					}
					else
					{
						auto errorcode = Errcodes::SPRITE_DECODER_ERROR;
						string definition = "SPRITE_DECODER_ERROR: " + to_string(debug);
						RW_ERROR err(errorcode, definition, 1);
						exit(errorcode);
					}
				}
				else
				{
					auto errorcode = Errcodes::SPRITE_FILENAME_ERROR;
					string definition = "SPRITE_FILENAME_ERROR: " + to_string(debug);
					RW_ERROR err(errorcode, definition, 1);
					exit(errorcode);
				}
			}
			else
			{
				auto errorcode = Errcodes::SPRITE_INSTANCE_ERROR;
				string definition = "SPRITE_INSTANCE_ERROR: " + to_string(debug);
				RW_ERROR err(errorcode, definition, 1);
				exit(errorcode);
			}
			// Убираем объекты WIC:
			RELEASE(wicFactory);
			RELEASE(wicDecoder);
			RELEASE(wicConverter);
			RELEASE(wicFrame);
			RELEASE(bmp);
		}
		// Текстуризация происходит только в том случае, когда ХОТЯ БЫ ОДИН пиксель объекта виден
		method texturize(LPCWSTR path, FVECTOR2 picSize, FVECTOR2 pos, FVECTOR2 size, float opacity) 
		{
			int x_value = size.x / picSize.x;
			int y_value = size.y / picSize.y;
			for (int x = 0; x < x_value; x++) 
			{
				for (int y = 0; y < y_value; y++) 
				{
					picture
					(
						path,
						FVECTOR2(pos.x + picSize.x * x,
							pos.y + picSize.y * y),
						opacity
					);
				}
			}
		}
		method createBmp(LPCWSTR path, ID2D1Bitmap* &bmp) 
		{
			HRESULT debug = 0L;
			if (bmp) return false;
			IWICImagingFactory* wicFactory = NULL;
			debug = CoCreateInstance
			(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				(LPVOID*)& wicFactory
			);
			IWICBitmapDecoder* wicDecoder = NULL;
			debug = wicFactory->CreateDecoderFromFilename
			(
				path,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&wicDecoder
			);
			IWICBitmapFrameDecode* wicFrame = NULL;
			debug = wicDecoder->GetFrame(0, &wicFrame);
			IWICFormatConverter* wicConverter = NULL;
			debug = wicFactory->CreateFormatConverter(&wicConverter);
			debug = wicConverter->Initialize
			(
				wicFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.0,
				WICBitmapPaletteTypeCustom
			);
			debug = RWGraphics::getRenderTarget()->CreateBitmapFromWicBitmap
			(
				wicConverter,
				NULL,
				&bmp
			);
			RELEASE(wicFactory);
			RELEASE(wicDecoder);
			RELEASE(wicFrame);
			return true;
		}
		method __fastcall fast_picture(ID2D1Bitmap* bmp, FVECTOR2 pos, float opacity, float size = 1.0f) 
		{
			// Проверка на видимость:
			if 
			(
				pos.x + bmp->GetSize().width > 0 &&
				pos.y + bmp->GetSize().height > 0 &&
				pos.x < getConfigString(ScreenSize_X) &&
				pos.y < getConfigString(ScreenSize_Y)
			) 
			{
				// Рисование изображения:
				RWGraphics::getRenderTarget()->DrawBitmap
				(
					bmp,
					RectF
					(
						pos.x, pos.y,
						pos.x + bmp->GetSize().width * size,
						pos.y + bmp->GetSize().height * size
					),
					opacity / 255.f,
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
					RectF
					(
						0, 0,
						bmp->GetSize().width, bmp->GetSize().height
					)
				);
			}
		}
		method __fastcall fast_picture_a(ID2D1Bitmap* bmp, FVECTOR2 pos, short el_n, short el_c, float opacity, float size = 1.f)
		{
			int width = bmp->GetSize().width / el_c;
			// Проверка на видимость:
			if 
			(
				pos.x + (width * el_n) > 0 &&
				pos.y + bmp->GetSize().height > 0 &&
				pos.x < getConfigString(ScreenSize_X) &&
				pos.y < getConfigString(ScreenSize_Y)
			)
			{
				// Рисование изображения:
				RWGraphics::getRenderTarget()->DrawBitmap
				(
					bmp,
					RectF
					(
						pos.x, pos.y,
						pos.x + width * size, pos.y + bmp->GetSize().height * size
					),
					opacity / 255.f,
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
					RectF
					(
						0 + width * (el_n - 1), 0,
						0 + width * el_n, bmp->GetSize().height
					)
				);
			}
		}
		method safePicture(LPCWSTR path, FVECTOR2 pos, float opacity, float size = 1.0f) 
		{
			HRESULT debug = 0L;
			ID2D1Bitmap* bmp = NULL;
			IWICImagingFactory* wicFactory = NULL;
			IWICBitmapDecoder* wicDecoder = NULL;
			IWICBitmapFrameDecode* wicFrame = NULL;
			IWICFormatConverter* wicConverter = NULL;
			UINT sizeX, sizeY;
			// Создание фабрики WIC:
			debug = CoCreateInstance
			(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				(LPVOID*)& wicFactory
			);
			// Создание декодера:
			debug = wicFactory->CreateDecoderFromFilename
			(
				path,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&wicDecoder
			);
			if (FAILED(debug)) 
			{
				auto errorcode = Errcodes::SPRITE_FILENAME_ERROR;
				string definition = "SPRITE_FILENAME_ERROR: " + to_string(debug);
				RW_ERROR err(errorcode, definition, 1);
				exit(errorcode);
			}
			// Чтение изображения:
			debug = wicDecoder->GetFrame(0, &wicFrame);
			wicFrame->GetSize(&sizeX, &sizeY);
			// Если хотя бы один пиксель изображения виден, то оно рендерится
			if 
			(
				(pos.x + sizeX) < getConfigString(ScreenSize_X) &&
				(pos.x + sizeX) > 0 &&
				(pos.x + sizeY) < getConfigString(ScreenSize_Y) &&
				(pos.y + sizeY) > 0
			)
			{
				// Создание конвертёра:
				debug = wicFactory->CreateFormatConverter(&wicConverter);
				// Настройка конвертёра:
				debug = wicConverter->Initialize
				(
					wicFrame,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.0,
					WICBitmapPaletteTypeCustom
				);
				// Конвертация в формат ID2D1Bitmap:
				debug = RWGraphics::getRenderTarget()->CreateBitmapFromWicBitmap
				(
					wicConverter,
					NULL,
					&bmp
				);
				// Рисование изображения:
				RWGraphics::getRenderTarget()->DrawBitmap
				(
					bmp,
					RectF(pos.x, pos.y,
						pos.x + bmp->GetSize().width * size, pos.y + bmp->GetSize().height * size),
					opacity / 255.f,
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
					RectF(0, 0,
						bmp->GetSize().width, bmp->GetSize().height)
				);
			}
			// Убираем объекты WIC:
			RELEASE(wicFactory);
			RELEASE(wicDecoder);
			RELEASE(wicConverter);
			RELEASE(wicFrame);
			RELEASE(bmp);
		}
	}

	// Анимации
	namespace Animations 
	{
		struct ANIM_ROUNDEDLOAD9_GRADIENT 
		{
			FVECTOR2 element[9];
			bool isInitialised = false;
			ANIM_ROUNDEDLOAD9_GRADIENT()
			{
				memset(&element, 0, 9 * sizeof(FVECTOR2));
				this->isInitialised = false;
			}
			ANIM_ROUNDEDLOAD9_GRADIENT(FVECTOR2 center) 
			{
				this->element[0] = center;
				this->element[1] = center;
				this->element[2] = center;
				this->element[3] = center;
				this->element[4] = center;
				this->element[5] = center;
				this->element[6] = center;
				this->element[7] = center;
				this->element[8] = center;
				this->isInitialised = true;
			}
			~ANIM_ROUNDEDLOAD9_GRADIENT() 
			{
				memset(this, 0, sizeof(*this));
			}
		};
		method drawAnimation() 
		{

		}
		method roundedLoading(FVECTOR2 center, float radius, RGBA color, bool isActive,
			ANIM_ROUNDEDLOAD9_GRADIENT &obj) 
		{
			if (!obj.isInitialised) 
			{
				obj = ANIM_ROUNDEDLOAD9_GRADIENT(center);
				for (int iter = 0; iter < 9; iter++) 
				{
					obj.element[iter] = 
						rotation
						(
							FVECTOR2(center.x, center.y),
							-90.0f,
							radius
						);
				}
			}
			if (isActive) 
			{
				for (int iter = 0; iter < 9; iter++) 
				{
					obj.element[iter] =
						rotation
						(
							FVECTOR2
							(
								center.x,
								center.y
							),
							vecAngle
							(
								FVECTOR2
								(
									center.x,
									center.y
								), 
								FVECTOR2
								(
									obj.element[iter].x,
									obj.element[iter].y
								)
							) + 9 - iter,
							radius
						);
				}
			}
			for (int iter = 0; iter < 9; iter++) 
			{
				color.alpha = 255 - 30 * iter;
				ellipse
				(
					TWDELLIPSE
					(
						FVECTOR2(obj.element[iter].x, obj.element[iter].y),
						6.0f, 6.0f
					), color, 1.0f
				);
			}
		}
	}
}