#pragma once
#include "RWLevel.h"
#include "RWController.h"
#include "TG_Logic.hpp"

class TGLevel : public RWLevel {
public:
	inline FVECTOR2 extract_coord(u_short pos)
	{
		u_short n = game->get_n();
		return FVECTOR2
		(
			n * (((float)pos / (float)n) - floor(pos / n)),
			floor(pos / n)
		);
	}
	inline void get_triangles(u_short x, u_short y)
	{
		DotStats A, B, C;
		A = game->get_stats(x, y);
		for (u_short i = 0; i < 8; i++)
		{
			if (!A.IDDCD[i]) return;
			else
			{
				if (!A.IDDCD[i + 1]) return;
				B = game->get_stats
				(
					game->get_n() * (((float)A.IDDCD[i] / (float)game->get_n()) - floor(A.IDDCD[i] / game->get_n())),
					floor(A.IDDCD[i] / game->get_n())
				);
				C = B = game->get_stats
				(
					game->get_n() * (((float)A.IDDCD[i + 1] / (float)game->get_n()) - floor(A.IDDCD[i + 1] / game->get_n())),
					floor(A.IDDCD[i + 1] / game->get_n())
				);
				if (B.IDDCD[i] && B.IDDCD[i + 1])
				{
					if ((B.IDDCD[i] == A.IDDCD[i]) || (B.IDDCD[i + 1] == A.IDDCD[i]))
					{
						if
							(
							(C.IDDCD[i] == B.IDDCD[i]) || (C.IDDCD[i] == A.IDDCD[i]) ||
								(C.IDDCD[i] == B.IDDCD[i + 1]) || (C.IDDCD[i] == A.IDDCD[i + 1]) ||
								(C.IDDCD[i + 1] == B.IDDCD[i]) || (C.IDDCD[i + 1] == A.IDDCD[i]) ||
								(C.IDDCD[i + 1] == B.IDDCD[i + 1]) || (C.IDDCD[i + 1] == A.IDDCD[i + 1])
								)
						{
							// Внимание! Внешнее подключение движка, работать вне проекта не будет.
							RWGraphics::npolygon
							(
								RWGraphics::NPOLYGON
								(
									3,
									{
										
										p_start + (extract_coord(game->get_position(x, y)) * delta),
										p_start + (extract_coord(A.IDDCD[i]) * delta),
										p_start + (extract_coord(A.IDDCD[i + 1]) * delta),
									}
								),
								RWGraphics::Colors::chartreuse,
								1.0f
							);
						}
						else return;
					}
					else return;
				}
				else return;
			}
		}
	}
	void load() override
	{
		srand(time(0));
		isClicked = false;
		game = new TGLogic(5 + rand() % 1);
		lag = new LAGOMETER();
		typecolors = new RGBA[16]{ RGBA(0x30, 0x30, 0x30), RGBA(0x64, 0x64, 0xF1) };
		p_zero = FVECTOR2
		(
			(getConfigString(ScreenSize_X) - getConfigString(ScreenSize_Y)) * 0.75f,
			getConfigString(ScreenSize_Y) * 0.125f
		);
		delta = (getConfigString(ScreenSize_Y) * 0.75f) / (float)game->get_n();
		p_start = p_zero + FVECTOR2(0.45 * delta, 0.75 * delta);
		p_click = IVECTOR2(-1, -1);
	}
	void update() override
	{
		// Выглядит крайне сомнительно, но, сука, работает!
		// Но в любом случае надо будет оптимизировать...
		// Нет, хуйня какая-то - не работает.
		FVECTOR2 test = FVECTOR2();
		FVECTOR2 lo_range = FVECTOR2();
		FVECTOR2 hi_range = FVECTOR2();
		for (u_short y = 0; y < game->get_n(); y++)
		{
			for (u_short x = 0; x < game->get_n(); x++)
			{
				test = p_start + FVECTOR2(delta * x, delta * y);
				lo_range = FVECTOR2(test.x - 10.f, test.x + 10.f);
				hi_range = FVECTOR2(test.y - 10.f, test.y + 10.f);
				// Если произошёл клик в заданной области и она существует в логике игры
				if (GetAsyncKeyState(VK_LBUTTON) &&
					inRange(mousePos, lo_range, hi_range) && game->get_matrix()[x][y])
				{
					// Если не было клика - переключаем флаг и сохраняем позицию
					if (!isClicked) 
					{ 
						invert(isClicked, INV_LOGIC);
						p_click = IVECTOR2(x, y);
					}
					// Если клик есть и он не повторяет последнюю позицию
					else if (!(IVECTOR2(x, y) == p_click) && isClicked)
					{
						// Если расстояния по x или y принадлежат интервалам (0; 2] или (0; 1]
						if
							(
								(abs(x - p_click.x) <= 2 &&	abs(y - p_click.y) <= 1) ||
								(abs(y - p_click.y) <= 2 &&	abs(x - p_click.x) <= 1)
							)
						{
							// Соединяем точки, сбрасываем флаг, выходим из цикла
							game->dot_connect(p_click.x, p_click.y, x, y);
							invert(isClicked, INV_LOGIC);
							break; break;
						}
					}
				}
				// Если нажата правая кнопка мыши - выход из режима соединения
				else if (GetAsyncKeyState(VK_RBUTTON) && isClicked)
				{
					isClicked = false;
				}
			}
		}
		lag->setTimeFrom();
	}
	void render() override
	{
		clscr(RGBA(0x20, 0x20, 0x20));
		// Секция UI:
		//text(L"Счёт:\n", FVECTOR2(25.f, 25.f), Colors::greenYellow, Fonts::roboto);
		if (!game->get_debug())
		{
			text(L"gamestats = \n", FVECTOR2(25.f, 15.f), Colors::orangeRed, Fonts::fixedsys);
			for (u_short x = 0; x < game->get_n(); x++)
			{
				for (u_short y = 0; y < game->get_n(); y++)
				{
					string i = game->gamestats[x + game->get_n() * y];
					text(wstring(i.begin(), i.end()) + L"\n", FVECTOR2(25.f, 35.f + 15.f * (x + game->get_n() * y)), Colors::orangeRed, Fonts::fixedsys);
				}
			}
			text(L"debug mode ON\n", FVECTOR2(450.f, 15.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"isClicked = " + to_wstring(isClicked) + L"\n", FVECTOR2(450.f, 35.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"delta = " + to_wstring(delta) + L"\n", FVECTOR2(450.f, 50.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"p_zero = " + to_wstring(p_zero.x) + L" ; " + to_wstring(p_zero.y) + L"\n", FVECTOR2(450.f, 65.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"p_start = " + to_wstring(p_start.x) + L" ; " + to_wstring(p_start.y) + L"\n", FVECTOR2(450.f, 80.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"p_click = " + to_wstring(p_click.x) + L" ; " + to_wstring(p_click.y) + L"\n", FVECTOR2(450.f, 95.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"n = " + to_wstring(game->get_n()) + L"\n", FVECTOR2(450.f, 110.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"m_pos = " + to_wstring(mousePos.x) + L" ; " + to_wstring(mousePos.y) + L"\n", FVECTOR2(450.f, 125.f), Colors::yellow, Fonts::fixedsys);
			text(L"score = " + to_wstring(game->score) + L"\n", FVECTOR2(450.f, 140.f), Colors::green, Fonts::fixedsys);
		}
		// Секция игровой зоны:
		if (isClicked)
		{
			line
			(
				TWDLINE
				(
					p_start + FVECTOR2
					(
						delta * p_click.x, 
						delta * p_click.y
					),
					mousePos
				),
				2.5f,
				Colors::green
			);
		}
		for (u_short y = 0; y < game->get_n(); y++)
		{
			for (u_short x = 0; x < game->get_n(); x++)
			{
				get_triangles(x, y);
				// Регистр (натуральный) уже содержит в себе позиции своих пар:
				for (u_short i = 0; i < 8; i++)
				{
					if (game->get_stats(x, y).IDDCD[i])
					{
						line
						(
							TWDLINE
							(
								p_start + FVECTOR2
								(
									delta * x,
									delta * y
								),
								p_start + extract_coord(game->get_stats(x, y).IDDCD[i]) * delta
							),
							2.5f,
							Colors::green
						);
						if (!game->get_stats(x, y).IDDCD[i + 1]) break;
					}
					else if (i == 0) break;
				}

			}
		}
		for (u_short y = 0; y < game->get_n(); y++)
		{
			for (u_short x = 0; x < game->get_n(); x++)
			{
				ellipse
				(
					TWDELLIPSE
					(
						p_start + FVECTOR2(delta * x, delta * y), 
						5.f, 5.f
					),
					typecolors[game->get_stats(x, y).IDC], 1.f
				);
			}
		}
		// Секция стандартных элементов движка:
		drawLagometer();
		drawCursor();
		lag->setTimeTo();
	}
	void unload() override
	{
		delete lag;
		delete game;
		delete typecolors;
	}
private:
	TGLogic				*game;
	RGBA				*typecolors;
	FVECTOR2			p_zero;
	FVECTOR2			p_start;
	IVECTOR2			p_click;
	float				delta;
	bool				isClicked;
};