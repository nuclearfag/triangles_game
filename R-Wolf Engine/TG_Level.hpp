#pragma once
#include "RWLevel.h"
#include "RWController.h"
#include "TG_Logic.hpp"

class TGLevel : public RWLevel {
public:
	void load() override
	{
		srand(time(0));
		connecting = false;
		game = new TGLogic(4 + rand() % 3);
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
		game->set_iddc(0, 0, 1, 1);
	}
	void update() override
	{
		// Выглядит крайне сомнительно, но, сука, работает!
		// Но в любом случае надо будет оптимизировать...
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
				// Если произошёл клик в заданной области
				if (GetAsyncKeyState(VK_LBUTTON) &&
					inRange(mousePos, lo_range, hi_range))
				{
					// Если позиция клика не повторяет предыдущую
					if (!(IVECTOR2(x, y) == p_click))
					{
						invert(connecting, INV_LOGIC);	// Отрицание флага connecting
						p_click = IVECTOR2(x, y);		// Присвоение p_click нового значения
						// Если расстояния по x или y принадлежат интервалам (0; 2] или (0; 1]
						if
							(
								connecting &&
								(abs(x - p_click.x) && abs(x - p_click.x) <= 2 &&
								abs(y - p_click.y) && abs(y - p_click.y) <= 1) ||
								(abs(y - p_click.y) && abs(y - p_click.y) <= 2 &&
								abs(x - p_click.x) && abs(x - p_click.x) <= 1)
							)
						{
							invert(connecting, INV_LOGIC);	// Отрицание флага connecting
							game->dot_connect(p_click.x, p_click.y, x, y);	// Соединение точек
						}
						else
						{
							//p_click = IVECTOR2(-1, -1);
						}
					}
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
			text(L"connecting = " + to_wstring(connecting) + L"\n", FVECTOR2(450.f, 35.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"delta = " + to_wstring(delta) + L"\n", FVECTOR2(450.f, 50.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"p_zero = " + to_wstring(p_zero.x) + L" ; " + to_wstring(p_zero.y) + L"\n", FVECTOR2(450.f, 65.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"p_start = " + to_wstring(p_start.x) + L" ; " + to_wstring(p_start.y) + L"\n", FVECTOR2(450.f, 80.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"p_click = " + to_wstring(p_click.x) + L" ; " + to_wstring(p_click.y) + L"\n", FVECTOR2(450.f, 95.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"n = " + to_wstring(game->get_n()) + L"\n", FVECTOR2(450.f, 110.f), Colors::orangeRed, Fonts::fixedsys);
			text(L"m_pos = " + to_wstring(mousePos.x) + L" ; " + to_wstring(mousePos.y) + L"\n", FVECTOR2(450.f, 125.f), Colors::yellow, Fonts::fixedsys);
			text(L"score = " + to_wstring(game->score) + L"\n", FVECTOR2(450.f, 140.f), Colors::green, Fonts::fixedsys);
		}
		// Секция игровой зоны:
		if (connecting)
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
	bool				connecting;
};