//	Базовый заголовочный файл "уровня загрузки"
//	Версия 1.2
//	Последнее обновление: 16.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once
#include "RWLevel.h"
#include "RWController.h"
#include "RWDirectSound.h"
#include "RWTcpIp.h"

class RWLoading : public RWLevel {
private:
	RWGraphics::Animations::ANIM_ROUNDEDLOAD9_GRADIENT anim;
	// Строчный буфер загрузчика уровня
	wstring buff;
	// Переменные - флаги
	bool isSystemReady = true;
	// Переменные с указателями на файлы:
public:
	void load() override;
	void unload() override;
	void update() override;
	void render() override;
};