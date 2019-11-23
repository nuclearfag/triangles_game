//	Базовый исходный файл "уровня загрузки" движка
//	Версия 1.2.1
//	Последнее обновление: 16.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#include "RWLoading.h"
#include "RWHardwareSync.h"

using namespace RWGraphics;
using namespace RWGraphics::Colors;
using namespace RWGraphics::Fonts;
using namespace RWGraphics::Sprite;
using namespace RWGraphics::Animations;
using namespace RWHardwareSync;

void RWLoading::load()
{
	lag = new LAGOMETER();
	checkVars = new unsigned char[1]{ 0 };
	buff = L"# R-Wolf Engine Evo ver. 1.0.0.0.\n# (С) kalterseele, 2018-2019 \n";
	buff += L"# Графические модули инициализированы успешно.\n";
	buff += L"# Инициализация модулей DirectSound и TCP/IP:\n";
	//if (1) buff += L"# Модуль TCP/IP запущен успешно.\n";
	//else buff += L"Возникла ошибка при запуске модуля TCP/IP!";
	if (RWDirectSound::initialize()) buff += L"# Модуль DirectSound запущен...\n";
	else buff += L"Возникла ошибка при запуске модуля DirectSound!";
	if (RWDirectSound::initSecondBuffer(60)) buff += L"# Звуковые буферы успешно созданы.\n";
	else buff += L"Возникла ошибка при запуске модуля DirectSound!";
	buff += L"Движок готов.";
}

void RWLoading::update() 
{
	lag->setTimeFrom();
}

void RWLoading::render() 
{
	clscr(RGBA(0x0E, 0x0E, 0x0E));
	picture(logoName,
		FVECTOR2(
			getConfigString(ScreenSize_X) / 2.0f - 256.0f, 
			getConfigString(ScreenSize_Y) / 2.0f - 256.0f
		),
		255.0);
	roundedLoading(
		FVECTOR2(
			getConfigString(ScreenSize_X) / 2.0f, 
			getConfigString(ScreenSize_Y) / 2.0f + (getConfigString(ScreenSize_Y) / 2.0f + 256.0f) / 2.0f
		), 
		32.0f, 
		RGBA(32, 178, 170), 
		true, 
		anim);
	text(buff, FVECTOR2(10.0f, 10.0f), white, consolas);
	//drawCursor();
	lag->setTimeTo();
	catchSignal(signalBuffer, this);
}

void RWLoading::unload() 
{
	delete lag;
	delete[] checkVars;
}