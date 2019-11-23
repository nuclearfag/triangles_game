//	Базовый заголовочный файл со всеми базовыми заголовками и общими функциями движка
//	Версия 1.2
//	Последнее обновление: 16.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once

//	Заголовки WinAPI и Process Status API
#include <Windows.h>
#include <Psapi.h>

//	Заголоки DirectX, Direct2D, DirectWrite и DXGI
//	Прямое использование функций пространства D2D1
#include <D2D1.h>
#include <DXGI.h>
#include <DWrite.h>
#include <DirectxMath.h>
using namespace DirectX;
using namespace D2D1;

//	Стандартные заголовки C++
//	Прямое использование функций пространства STD
#include <wchar.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <typeinfo>
#include <thread>
#include <mutex>
using namespace std;

//	Заголовки движка
#include "RWGlobalDefinitions.h"
#include "RWPhysicsConstants.h"
#include "RWStringsParser.h"
#include "RWVectorMath.h"
#include "RWGraphics.h"
#include "RWController.h"
#include "RWLevel.h"
#include "resource.h"