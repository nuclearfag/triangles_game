//	Базовый заголовочный файл функций работы с конфигурациями
//	Версия 1.2.1
//	Последнее обновление: 27.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once
#include <wchar.h>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

enum ConfigStrings {
	ScreenSize_X = 0,
	ScreenSize_Y,
	Screen_IsFullscreen
};

/// <summary>
/// Возвращает значение запрошенного параметра
/// </summary>
inline auto getConfigString(int arg) {
	wifstream read(L"config.rw");
	int capturedString;
	for (int iter = 0; !read.eof(); iter++) {
		read >> capturedString;
		if (arg == iter) return capturedString;
	}
	return -65535;
	read.close();
}

namespace RWScript
{
	inline wstring getScriptData(LPCWSTR path, LPCWSTR key)
	{
		wifstream read(path);
		wstring buff, cmd, out;
		bool open(false), val(false);
		short ret_type;
		LPCWSTR arg(0);
		buff.resize(128, 0);
		for (short iter = 0; !read.eof(); iter++)
		{
			getline(read, buff, L'\n');
			arg = buff.c_str();
			for (short i = 0; i < wcslen(arg); i++)
			{
				if (arg[i] == L']') 
				{
					val = true;
					open = false;
					continue;
				}
				if (open)
				{
					cmd += arg[i];
				}
				else if (arg[i] == L'[') open = true;
				else if (!open && val)
				{
					if (arg[i] != L';')
					{
						out += arg[i];
					}
					else
					{
						val = false;
					}
				}
				if (!val && !wcscmp(cmd.c_str(), key))
				{
					if (out.c_str()) return (out + L'\n');
					else return L"Empty\n";
				}
			}
		}
		return L"null";
		read.close();
	}
}