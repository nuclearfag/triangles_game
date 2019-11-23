//	Базовый исходный файл контроллера операций движка
//	Версия 1.2.1
//	Последнее обновление: 20.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#include "RWHeaders.h"

RWLevel* RWController::currentLevel;
bool RWController::isLoading;

/// <summary>
/// <para>
/// Инициализирует контроллер
/// </para>
/// <para>
/// -> нет
/// </para>
/// </summary>
void RWController::initialize() 
{
	isLoading = true;
	currentLevel = 0;
}

void RWController::uninitialize()
{
	isLoading = false;
	currentLevel->unload();
}

/// <summary>
/// <para>
/// Инициализирует и загружает в память уровень 
/// </para>
/// <para>
/// [level] : указатель на класс уровня
/// </para>
/// <para>
/// -> нет
/// </para>
/// </sunnary>
void RWController::loadInitialLevel(RWLevel* level) 
{
	isLoading = true;
	currentLevel = level;
	currentLevel->load();
	isLoading = false;
}

/// <summary>
/// <para>
/// Переключает текущий уровень на заданный
/// </para>
/// <para>
/// [level] : указатель на класс уровня
/// </para>
/// <para>
/// -> нет
/// </para>
/// </sunnary>
void RWController::switchLevel(RWLevel* level) 
{
	isLoading = true;
	currentLevel->unload();
	level->load();
	delete currentLevel;
	currentLevel = level;
	isLoading = false;
}

/// <summary>
/// <para>
/// Делегирует вызов функции рендеринга уровню
/// </para>
/// <para>
/// -> нет
/// </para>
/// </sunnary>
void RWController::render() 
{
	if (isLoading) return;
	currentLevel->render();
}

/// <summary>
/// <para>
/// Делегирует вызов функции апдейта уровню
/// </para>
/// <para>
/// -> нет
/// </para>
/// </sunnary>
void RWController::update() 
{
	if (isLoading) return;
	currentLevel->update();
}