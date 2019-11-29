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
		game = new TGLogic(3 + rand() % 13);
		lag = new LAGOMETER();
		typecolors = new RGBA[16]{ RGBA(0x30, 0x30, 0x30), RGBA(0x64, 0x64, 0xF1) };
		p_zero = FVECTOR2
		(
			(getConfigString(ScreenSize_X) - getConfigString(ScreenSize_Y)) * 0.75f,
			getConfigString(ScreenSize_Y) * 0.125f
		);
		delta = (getConfigString(ScreenSize_Y) * 0.75f) / (float)game->get_n();
		p_start = p_zero + delta;
		prev_click = IVECTOR2(-1, -1);
		game->switch_nodebug();
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
				test = p_start * delta;
				lo_range = FVECTOR2(test.x - 10.f, test.x + 10.f);
				hi_range = FVECTOR2(test.y - 10.f, test.y + 10.f);

				if (GetAsyncKeyState(VK_LBUTTON) &&
					inRange(mousePos, lo_range, hi_range))
				{
					if (IVECTOR2(x, y) != prev_click)
					{
						invert(connecting, INV_LOGIC);
						prev_click = IVECTOR2(x, y);
						if
							(
							(abs(x - p_buffer.x) <= 2 && abs(y - p_buffer.y) <= 1) ||
								(abs(y - p_buffer.y) <= 2 && abs(x - p_buffer.x) <= 1)
							)
						{
							invert(connecting, INV_LOGIC);
							game->dot_connect(p_buffer.x, p_buffer.y, x, y);
						}
						else
						{
							// Вывести предупреждение о недопустимом расстоянии
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
		text(L"Счёт:\n", FVECTOR2(25.f, 25.f), Colors::greenYellow, Fonts::roboto);
		// Секция игровой зоны:
		if (connecting)
		{
			line
			(
				TWDLINE
				(
					p_start * delta,
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
	IVECTOR2			p_buffer;
	IVECTOR2			prev_click;
	float				delta;
	bool				connecting;
};