//	Базовый заголовочный файл для парсинга XML скриптов
//	Используются технологии TinyXML
//	Версия 1.2.1
//	Последнее обновление: 20.03.2019
//	Не для распространения; декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once
#include <Windows.h>
//#include "tinyxml/tinyxml.h"

class RWScript {
public:
//	TiXmlDocument file;
	LPCSTR docName;
	bool loadFile(LPCSTR);
	void listFile(LPCSTR);
private:

};