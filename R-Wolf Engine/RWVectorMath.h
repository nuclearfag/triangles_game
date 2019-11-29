//	Базовый заголовочный файл для векторной математики
//	Версия 1.2 Evo, правка 2.
//	Оптимизировано 100% кода
//	Готово к использованию внутри DLL
//	Последнее обновление: 16.09.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once
#include <cmath>

/// <summary>
/// <para>
/// Базовая библиотека вспомогательных математических операций движка
/// </para>
/// </summary>
namespace RWMath 
{
	// Число Пи
	constexpr float PI = 3.141592741F;
	// Значение экспоненты
	constexpr float EXP = 2.718281746F;
	// Число эпсилон
	constexpr float Epsilon = 1E-7F;
	#define radToDeg * 180 / PI		
	#define degToRad * PI / 180
	/// <summary>
	/// <para>
	/// FVECTOR2: 2D-вектор с координатами типа float
	/// </para>
	/// </summary>
	struct FVECTOR2 
	{
		/// <summary>
		/// <para>
		/// Инициализация нулевого вектора в точке (0; 0)
		/// </para>
		/// </summary>
		FVECTOR2() : x(0.f), y(0.f)
		{

		}
		/// <summary>
		/// <para>
		/// Инициализация нулевого вектора в точке (_x; _y)
		/// </para>
		/// </summary>
		FVECTOR2(float _x, float _y) : x(_x), y(_y)
		{

		}
		/// <summary>
		/// <para>
		/// Деструктор объекта
		/// </para>
		/// </summary>
		~FVECTOR2()
		{
			memset(this, 0, sizeof(*this));
		}
		bool operator ==(const FVECTOR2 &vec) 
		{
			return (&vec == this);
		}
		FVECTOR2 operator +(const FVECTOR2& vec)
		{
			return FVECTOR2(this->x + vec.x, this->y + vec.y);
		}
		FVECTOR2 operator -(const FVECTOR2& vec)
		{
			return FVECTOR2(this->x - vec.x, this->y - vec.y);
		}
		FVECTOR2 operator*(const float lambda)
		{
			return FVECTOR2(this->x * lambda, this->y * lambda);
		}
		float x;
		float y;
	};
	/// <summary>
	/// <para>
	/// IVECTOR2: 2D-вектор с координатами типа int
	/// </para>
	/// </summary>
	struct IVECTOR2 
	{
		/// <summary>
		/// <para>
		/// Инициализация нулевого вектора в точке (0; 0)
		/// </para>
		/// </summary>
		IVECTOR2() : x(0), y(0)
		{
			
		}
		/// <summary>
		/// <para>
		/// Инициализация нулевого вектора в точке (_x; _y)
		/// </para>
		/// </summary>
		IVECTOR2(int _x, int _y) : x(_x), y(_y)
		{
			
		}
		/// <summary>
		/// <para>
		/// Деструктор объекта
		/// </para>
		/// </summary>
		~IVECTOR2()
		{
			memset(this, 0, sizeof(*this));
		}
		bool operator ==(const IVECTOR2& vec)
		{
			return (&vec == this);
		}
		bool operator !=(const IVECTOR2& vec)
		{
			return (&vec == this);
		}
		IVECTOR2 operator +(const IVECTOR2& vec)
		{
			return IVECTOR2(this->x + vec.x, this->y + vec.y);
		}
		IVECTOR2 operator -(const IVECTOR2& vec)
		{
			return IVECTOR2(this->x - vec.x, this->y - vec.y);
		}
		IVECTOR2 operator*(const int lambda)
		{
			return IVECTOR2(this->x * lambda, this->y * lambda);
		}
		int x;
		int y;
	};
	enum ReturnTypes
	{
		AS_RADIANS,
		AS_DEGREES,
		IS_FPOINT = 0,
		IS_SPOINT,
		INV_LOGIC = 0,
		INV_BINARY,
	};

	/// <summary>
	/// Осуществляет поворот точки по окружности с заданными радиусом и аргументом (deg.)
	/// <para>
	/// [center] : Координаты центра окружности;
	/// </para>
	/// <para>
	/// [arg] : Аргумент угла поворота по часовой стрелке от оси абсцисс;
	/// </para>
	/// <para>
	/// [radius] : Радиус окружности, на которой будел лежать точка;
	/// </para>
	/// <para>
	/// [deg] : Возвращать значение в градусах или радианах. True - градусы;
	/// </para>
	/// <para>
	/// -> координаты точки после поворота по часовой стрелке от оси абсцисс.
	/// </para>
	/// <para>
	/// --! аргумент нуля градусов лежит в точке (x = radius; y = 0) !--
	/// </para>
	/// </summary>
	inline auto rotation(FVECTOR2 center, float arg, float radius, bool deg = AS_DEGREES)
	{
		if (deg)
		return FVECTOR2
		(
			center.x + radius * cos(arg degToRad),
			center.y + radius * sin(arg degToRad)
		);
		else
		return FVECTOR2
		(
			center.x + radius * cos(arg),
			center.y + radius * sin(arg)
		);
	}

	/// <summary>
	/// Вычисляет первую точку пересечения окружностей заданных радиусов
	/// <para>
	/// [pointA] : Координаты центра окружности O.1;
	/// </para>
	/// <para>
	/// [pointB] : Координаты центра окружности O.2;
	/// </para>
	/// <para>
	/// [a] : Радиус окружности O.1;
	/// </para>
	/// <para>
	/// [b] : Радиус окружности O.2;
	/// </para>
	/// <para>
	/// [point] : Выбор первой или второй точки пересечения;
	/// </para>
	/// <para>
	/// -> координаты точки пересечения окружностей
	/// </para>
	/// </summary>
	inline auto intersection(FVECTOR2 pointA, FVECTOR2 pointB, float a, float b, bool point = IS_FPOINT)
	{
		float length = sqrt(pow((pointB.x - pointA.x), 2) + pow((pointB.y - pointA.y), 2));

		if (length > a + b) length = a + b;
		if (length < a && a < b) length = b - a;
		else if (length < b && b < a) length = a - b;

		float alpha, beta, gamma, delta;
		alpha = acosf((pow(length, 2) + pow(a, 2) - pow(b, 2)) / (2 * length * a)) radToDeg;
		beta = acosf((pow(a, 2) + pow(b, 2) - pow(length, 2)) / (2 * a * b)) radToDeg;
		gamma = 180.0f - alpha - beta;
		delta = atan2f(pointB.y - pointA.y, pointB.x - pointA.x) radToDeg;

		if (point)
		{
			return rotation(pointA, alpha + delta, a);
		}
		else
		{
			return rotation(pointA, -alpha + delta, a);
		}
	}

	/// <summary>
	/// Вычисляет угол отклонения вектора от оси абсцисс
	/// <para>
	/// [pointA] : Координаты начала вектора;
	/// </para>
	/// <para>
	/// [pointB] : Координаты конца вектора;
	/// </para>
	/// <para>
	/// -> угол отклонения вектора в градусах
	/// </para>
	/// </summary>
	inline auto vecAngle(FVECTOR2 pointA, FVECTOR2 pointB)
	{
		return atan2f(pointB.y - pointA.y, pointB.x - pointA.x) radToDeg;
	}

	/// <summary>
	/// Переносит вектор на заданное значение
	/// <para>
	/// [root] : Координаты вектора;
	/// </para>
	/// <para>
	/// [vec] : Значение, на которое переместится вектор;
	/// </para>
	/// <para>
	/// -> координаты вектора после переноса
	/// </para>
	/// </summary>
	inline auto vecMove(FVECTOR2 root, FVECTOR2 vec)
	{
		return FVECTOR2
		(
			root.x + vec.x,
			root.y + vec.y
		);
	}

	/// <summary>
	/// Вычисляет длину заданного вектора
	/// <para>
	/// [pointA] : Координаты начала вектора;
	/// </para>
	/// <para>
	/// [pointB] : Координаты конца вектора;
	/// </para>
	/// <para>
	/// -> длина вектора
	/// </para>
	/// </summary>
	inline auto vecLength(FVECTOR2 pointA, FVECTOR2 pointB)
	{
		return sqrt(pow((pointB.x - pointA.x), 2) + pow((pointB.y - pointA.y), 2));
	}

	/// <summary>
	/// Переносит точку вдоль вектора
	/// <para>
	/// [from] : Начало вектора;
	/// </para>
	/// <para>
	/// [to] : Точка вектора;
	/// </para>
	/// <para>
	/// [stepDistance] : Длина шага;
	/// </para>
	/// <para>
	/// -> нет
	/// </para>
	/// </summary>
	inline auto moveAlongTo(FVECTOR2 &from, FVECTOR2 to, float stepDistance)
	{
		float angle = vecAngle(from, to);
		from.x += stepDistance * cos(angle degToRad);
		from.y += stepDistance * sin(angle degToRad);
	}

	// Новая функция
	inline bool inRange(FVECTOR2 vec, FVECTOR2 x_range, FVECTOR2 y_range)
	{
		return (vec.x >= x_range.x && vec.x <= x_range.y) && 
			(vec.y >= y_range.x && vec.y <= y_range.y);
	}

	/// <summary>
	/// Производит побитовую или логическую инверсию переменной.
	/// <para>
	/// [in] : Входные данные;
	/// </para>
	/// <para>
	/// [invmode] : Логическая или побитовая инверсия. По умолчанию логическая;
	/// </para>
	/// <para>
	/// -> нет
	/// </para>
	/// </summary>
	template<class Any> auto invert(Any& in, bool invmode = INV_LOGIC)
	{
		if (invmode) in = ~in;
		else in = !(static_cast<bool>(in));
	}

	inline bool isEqual(float x, float y)
	{
		return (abs(x - y) < Epsilon);
	}
}

/// <summary>
/// <para>
/// Базовая библиотека побитовых оптимизационных операций движка
/// </para>
/// </summary>
namespace RWBinaryOp
{
	enum BinaryOperations
	{
		OP_READ,
		OP_WRITE
	};
#define READ (bool)OP_READ
#define WRITE (bool)OP_WRITE
	/// <summary>
	/// <para>
	/// Выполнение операции над ячейкой
	/// </para>
	/// <para>
	/// [cell] : ячейка, любая переменная
	/// </para>
	/// <para>
	/// [op] : тип операции, READ или WRITE
	/// </para>
	/// <para>
	/// [reg] : номер регистра от 0 до 8 * sizeof - 1. Иные варианты будут обрезаться до ближайшего допустимого
	/// </para>
	/// <para>
	/// -> Значение регистра в ячейке
	/// </para>
	/// <para>
	/// Операция WRITE недопустима для константных ячеек. Используйте перегруженный аналог функции
	/// </para>
	/// <para>
	/// Операция WRITE инвертирует значение заданного регистра ячейки
	/// </para>
	/// </summary>
	template<class In> inline bool cellOperation(In cell, bool op, unsigned short reg)
	{
		// Инициализация буфера ячейки для операции READ:
		In bc(cell);
		// Приведение номера регистра к допустимому:
		if (reg > (8 * sizeof(In) - 1)) reg = 8 * sizeof(In) - 1;
		// Инициализация контрольного регистра:
		In point = 1 << reg;
		if (op)	// Операция WRITE
		{
			cell = (bc xor point);
			return true;
		}
		else	// Операция READ
		{
			if ((bc xor point) > cell) return false;
			else return true;
			// (bc xor point) не может быть равен cell в силу особенностей операции xor
		}
	}

	template<class In> inline bool cellOperation(const In cell, unsigned short reg)
	{
		In bc(cell);
		In point = 1 << reg;
		if ((bc xor point) > cell) return false;
		else return true;
	}
}

namespace RWPhysics 
{
	struct PHYSOBJ 
	{
		PHYSOBJ() :
			weight(0.f),
			vSpeed(0.f),
			gSpeed(0.f),
			velocity(0.f, 0.f),
			gravity(0.f, 0.f)
		{

		}
		PHYSOBJ(
			float _w,
			float _vS,
			float _gS,
			RWMath::FVECTOR2 v,
			RWMath::FVECTOR2 g
		) :
			weight(_w),
			vSpeed(_vS),
			gSpeed(_gS * _w),
			velocity(v),
			gravity(g)
		{
			
		}
		~PHYSOBJ()
		{
			memset(this, 0, sizeof(*this));
		}
		RWMath::FVECTOR2 velocity;
		RWMath::FVECTOR2 gravity;
		float weight;
		float vSpeed;
		float gSpeed;
	};
}