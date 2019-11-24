#pragma once
#include "RWLevel.h"
#include "RWController.h"
#include "TG_Logic.hpp"

class TGLevel : public RWLevel {
public:
	void load() override
	{
		srand(time(0));
		game = new TGLogic(3 + rand() % 5);
		lag = new LAGOMETER();
		typecolors = new RGBA[16]{ RGBA(0x30, 0x30, 0x30), RGBA(0x64, 0x64, 0xF1) };
		p_zero = FVECTOR2
		(
			0.75f * (getConfigString(ScreenSize_X) - getConfigString(ScreenSize_Y)),
			getConfigString(ScreenSize_Y) * 0.125f
		);
		delta_w = (getConfigString(ScreenSize_Y) * 0.75f) / (float)game->get_n();
		delta_h = (getConfigString(ScreenSize_Y) * 0.75f) / (float)game->get_n();
		p_start = FVECTOR2
		(
			p_zero.x + (delta_w * 0.f),
			p_zero.y + (delta_h * 0.5f)
		);
		game->switch_nodebug();
	}
	void update() override
	{
		lag->setTimeFrom();
	}
	void render() override
	{
		clscr(RGBA(0x20, 0x20, 0x20));
		// Секция UI:
		text(L"Счёт:\n", FVECTOR2(25.f, 25.f), Colors::greenYellow, Fonts::roboto);
		// Секция игровой зоны:
		for (u_short y = 0; y < game->get_n(); y++)
		{
			for (u_short x = 0; x < game->get_n(); x++)
			{
				ellipse
				(
					TWDELLIPSE
					(
						p_start + FVECTOR2(x * delta_w, y * delta_h), 
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
	float				delta_w, delta_h;
};