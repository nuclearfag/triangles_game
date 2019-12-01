#pragma once
#pragma warning (disable: 4996)
#include <cstring>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <Windows.h>

typedef unsigned short u_short;
typedef unsigned long long u_int;
typedef const char c_char;

struct DotStats
{
	DotStats() :
		DS(0), IDD(0), IDC(0),
		IDDCD1_4(0), IDDCD5_8(0)
	{

	}
	DotStats(u_short ds, u_short idd, u_short idc, u_int iddcd1, u_int iddcd5) :
		DS(ds), IDD(idd), IDC(idc),
		IDDCD1_4(iddcd1), IDDCD5_8(iddcd5)
	{

	}
	~DotStats()
	{
		memset(this, 0, sizeof(*this));
	}
	u_short						DS, IDD, IDC;
	u_int						IDDCD1_4, IDDCD5_8;
};

enum DSStats
{
	DS_CONNECTED = 1,
	DS_TRIANGLE,
	DS_PENTAGON = 4,
	DS_HEXAGON = 8
};
enum DSColors
{
	DSC_EMPTY,
	DSC_EXIST,
	DSC_CONNECTED_1,
	DSC_CONNECTED_2,
	DSC_CONNECTED_3,
	DSC_CONNECTED_4,
	DSC_CONNECTED_5,
	DSC_CONNECTED_6,
	DSC_CONNECTED_7,
	DSC_CONNECTED_8,
	DSC_ISOLATED,
	DSC_SPECIAL1,
	DSC_SPECIAL2,
	DSC_SPECIAL3,
	DSC_SPECIAL4,
	DSC_SPECIAL5
};

class TGLogic
{
public:
	TGLogic(short size_n) :
		n(size_n),
		log_file("actions.log"),
		buffer(""),
		DS(0), IDD(0), IDC(0),
		IDDCD1_4(0), IDDCD5_8(0),
		LPID(1),
		nodebug(false),
		crashed(false),
		score(0)
	{
		srand(time(0));
		m = new u_short* [n] { 0 };
		gamestats = new string[n * n];
		for (u_short i = 0; i < n; i++) m[i] = new u_short[n]{ 0 };
		try
		{
			log_out.open(log_file, std::ios_base::out | std::ios_base::trunc);
			log_action("Класс инициализирован");
			std::string s = "Инициализирован массив со значением n = ";
			s += std::to_string(n);
			log_action(s);
		}
		catch (std::exception &exc)
		{
			crashed = true;
			std::string msg = "Caught exception in TGLogic(): ";
			msg += exc.what();
			log_action(msg);
			MessageBoxA(0, msg.c_str(), "Exception!", MB_OK | MB_ICONERROR);
		}
		rand_fill(1 + rand() % (n * n));
		process_matrix();
	}
	~TGLogic()
	{
		log_action("Класс деинициализирован");
		for (u_short i = 0; i < n; i++) delete[] m[i];
		delete[] m;
		delete[] gamestats;
		memset(this, 0, sizeof(*this));
	}
	void switch_nodebug() { nodebug = !nodebug; }
	void log_action(std::string action)
	{
		if (nodebug) return;
		else if (crashed) 
			log_out << "Process has crashed -> " << action << ": " << get_time();
		else log_out << action << ": " << get_time();
	}
	char *get_time()
	{
		auto t = std::chrono::system_clock::now();
		std::time_t action_time = std::chrono::system_clock::to_time_t(t);
		return std::ctime(&action_time);
	}
	u_short get_position(u_short x, u_short y) { return (x + n * y); }
	void rand_fill(u_short iterations)
	{
		u_short					arg_x, arg_y;
		std::string s = "Случайное заполнение массива с числом итераций [";
		s += std::to_string(iterations) + "]";
		log_action(s);
		srand(time(0));
		for (u_short i = 0; i < iterations; i++)
		{
			s.clear();
			s += "Итерация " + std::to_string(i) + " -> [x = ";
			arg_x = 0 + rand() % (n - 1);
			arg_y = 0 + rand() % (n - 1);
			s += std::to_string(arg_x) + "; y = " + std::to_string(arg_y) + "]";
			m[arg_x][arg_y] = 1;
			log_action(s);
		}
	}
	void process_matrix()
	{
		log_action("Обработка массива");
		std::string s;
		for (u_short sy = 0; sy < n; sy++)
		{
			for (u_short sx = 0; sx < n; sx++)
			{
				if (m[sx][sy])
				{
					s = "Запись в массив непустого элемента с LPID=";
					s += std::to_string(LPID) + " [ " + std::to_string(sx) + " ; ";
					s += std::to_string(sy) + " ]";
					log_action(s);
					set_stats(DotStats(0, LPID, DSC_EXIST, 0, 0), sx, sy);
					LPID++;
				}
				else 
				{
					s = "Запись в массив пустого элемента без LPID";
					s += " [ " + std::to_string(sx) + " ; ";
					s += std::to_string(sy) + " ]";
					log_action(s);
					set_stats(DotStats(0, 0, DSC_EMPTY, 0, 0), sx, sy); 
				}
			}
		}
	}
	void dot_connect(u_short d1_x, u_short d1_y, u_short d2_x, u_short d2_y)
	{
		DotStats dc[2]
		{
			get_stats(d1_x, d1_y),
			get_stats(d2_x, d2_y)
		};
		dc[0].IDDCD1_4 = get_position(d2_x, d2_y);
		dc[1].IDDCD1_4 = get_position(d1_x, d1_y);
		set_stats(dc[0], d1_x, d1_y);
		set_stats(dc[1], d2_x, d2_y);
	}
	DotStats get_stats(u_short x_pos, u_short y_pos)
	{
		u_short					_DS (0);
		u_short					_IDD (0);
		u_int					_IDDCD1_4 (0);
		u_int					_IDDCD5_8 (0);
		u_short					_IDC (0);
		std::string				s;
		log_action("Обработка запроса \'get_stats()\'");
		try
		{
			buffer = gamestats[x_pos + n * y_pos];
			_DS = static_cast<u_short>(std::stoi(buffer.substr(0, 2)));
			_IDD = static_cast<u_short>(std::stoi(buffer.substr(2, 5)));
			_IDDCD1_4 = std::stoull(buffer.substr(7, 20));
			_IDDCD5_8 = std::stoull(buffer.substr(27, 20));
			_IDC = static_cast<u_short>(std::stoi(buffer.substr(47, 2)));
			s = "Прочитанные данные: DS=";
			s += std::to_string(_DS) + "; IDD=" + std::to_string(_IDD) + "; IDDCD1_4=";
			s += std::to_string(_IDDCD1_4) + "; IDDCD5_8=" + std::to_string(_IDDCD5_8);
			s += "; IDC=" + std::to_string(_IDC);
			log_action(s);
		}
		catch (std::exception &exc)
		{
			crashed = true;
			std::string msg = "Caught exception in get_stats(): ";
			msg += exc.what();
			msg += "\nLast string: " + buffer + " ; (" + to_string(buffer.length()) + " symbols)";
			log_action(msg);
			MessageBoxA(0, msg.c_str(), "Exception!", MB_OK | MB_ICONERROR);
			exit(-1);
		}
		s = "Возвращённые данные: DS=";
		s += std::to_string(_DS) + "; IDD=" + std::to_string(_IDD) + "; IDDCD1_4=";
		s += std::to_string(_IDDCD1_4) + "; IDDCD5_8=" + std::to_string(_IDDCD5_8);
		s += "; IDC=" + std::to_string(_IDC) + " [ ";
		s += std::to_string(x_pos) + " ; " + std::to_string(y_pos) + " ]";
		log_action(s);
		return DotStats(_DS, _IDD, _IDC, _IDDCD1_4, _IDDCD5_8);
	}
	void set_stats(DotStats in, u_short x_pos, u_short y_pos)
	{
		std::string				s;
		std::string				i;
		log_action("Обработка запроса \'set_stats()\'");
		try
		{
			s = "Записанные данные: DS=";
			s += std::to_string(in.DS) + "; IDD=" + std::to_string(in.IDD) + "; IDDCD1_4=";
			s += std::to_string(in.IDDCD1_4) + "; IDDCD5_8=" + std::to_string(in.IDDCD5_8);
			s += "; IDC=" + std::to_string(in.IDC) + " [ ";
			s += std::to_string(x_pos) + " ; " + std::to_string(y_pos) + " ]";
			log_action(s);
			buffer.clear();

			i = std::to_string(in.DS);
			buffer.append(2 - i.size(), '0');
			buffer.append(i);

			i = std::to_string(in.IDD);
			buffer.append(5 - i.size(), '0');
			buffer.append(i);

			i = std::to_string(in.IDDCD1_4);
			buffer.append(20 - i.size(), '0');
			buffer.append(i);

			i = std::to_string(in.IDDCD5_8);
			buffer.append(20 - i.size(), '0');
			buffer.append(i);

			i = std::to_string(in.IDC);
			buffer.append(2 - i.size(), '0');
			buffer.append(i);

			gamestats[x_pos + n* y_pos] = buffer;
		}
		catch (std::exception &exc)
		{
			crashed = true;
			std::string msg = "Caught exception in set_stats(): ";
			msg += exc.what();
			msg += "\nLast string: " + buffer + " ; (" + to_string(buffer.length()) + " symbols)";
			log_action(msg);
			MessageBoxA(0, msg.c_str(), "Exception!", MB_OK | MB_ICONERROR);
			exit(-1);
		}
	}
	inline auto get_matrix() { return this->m; }
	inline auto get_n() { return this->n; }
	inline auto get_debug() { return this->nodebug; }
	std::string					*gamestats;
	u_int						score;
private:
	u_short						**m;
	u_short						n;
	u_short						DS, IDD, IDC;
	u_short						LPID;
	u_int						IDDCD1_4, IDDCD5_8;
	c_char						*log_file;
	std::ofstream				log_out;
	std::string					buffer;
	bool						nodebug;
	bool						crashed;
};