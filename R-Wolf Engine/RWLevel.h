//	Базовый заголовочный файл полиморфного класса 'Level'
//	Версия 1.2
//	Последнее обновление: 16.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once

#include "RWGraphics.h"
#include "RWTimeQuant.h"
#include "RWStringsParser.h"
#include "RWVectorMath.h"

using namespace RWMath;
using namespace RWBinaryOp;
using namespace RWGraphics;
using namespace RWGraphics::Sprite;

enum RWLevelCheckVars
{
	RWCV_DEVMODE,
	RWCV_TEXTURE_COMPILED,
	RWCV_CAMERA_FREEZED,
	RWCV_CONSOLE_IO_ENABLED,
	RWCV_NON_STANDART_RESOLUTION
};

class RWLevel {
protected:
	int								frameCounter;
	HWND							mainWindow;
	RWTIMEQUANT*					timeQuant;
	LPCWSTR							cursor = L"resources/img/cursor.png";
	LPCWSTR							logoName = L"resources/img/engine_logo.png";
	LAGOMETER*						lag;
	POINT							mPos;
	FVECTOR2						anchor;
	FVECTOR2						mousePos;
	FVECTOR2						mouseMultiplier;
	unsigned char*					checkVars = new unsigned char[1]{ 0 };
public:
	LPCWSTR							preview_pic;
	virtual void load() = 0;
	virtual void unload() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	inline auto drawCursor() 
	{
		ScreenToClient(mainWindow, &mPos);
		GetCursorPos(&mPos);
		mouseMultiplier = FVECTOR2
		(
			(float)mPos.x / (float)GetSystemMetrics(SM_CXSCREEN),
			(float)mPos.y / (float)GetSystemMetrics(SM_CYSCREEN)
		);
		mousePos = FVECTOR2
		(
			mouseMultiplier.x * (float)getConfigString(ScreenSize_X),
			mouseMultiplier.y * (float)getConfigString(ScreenSize_Y)
		);
		picture(cursor, mousePos, 255.0);
	}
	inline auto drawLagometer()
	{
		lag->setLastCallMsec(lag->t.to);
		lagometer(lag);
	}
	inline auto setDevmode()
	{
		cellOperation(checkVars[0], WRITE, RWCV_DEVMODE);
	}
	inline auto openConsole()
	{

	}
};