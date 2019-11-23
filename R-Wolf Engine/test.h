#pragma once
#include "RWLevel.h"
#include "RWController.h"
#include "RWGraphics.h"
#include "RWDirectSound.h"
#include "RWTcpIp.h"
#include "RWHardwareSync.h"

using namespace RWMath;
using namespace RWHardwareSync;
using namespace RWGraphics;
using namespace RWGraphics::Colors;
using namespace RWGraphics::Fonts;
using namespace RWGraphics::Animations;

#include "Interface.h"
#include "Marchenwelt_BOM.h"

using namespace Marchenwelt_BOM;
using namespace Marchenwelt_Interface;

class Test : public RWLevel {
private:
	ID2D1Bitmap* map;
	float pacificSpeed;
	float runningSpeed;
public:
	void load() override;
	void unload() override;
	void update(double timeTotal, double timeDelta) override;
	void render() override;
};

void Test::load() {
	frameCounter = 0;
	mainWindow = getEngineHwnd();
	anchor = FVECTOR2(-4096.0f + getConfigString(ScreenSize_X) / 2, -4096.0f + getConfigString(ScreenSize_Y) / 2);
	cameraFreezed = true;
	pacificSpeed = 2.048f - 0.512f;
	runningSpeed = 4.096f;
}

void Test::update(double, double) {
	lag.setTimeFrom();
	switch (cameraFreezed) {
	case true:
		if (captureKey(kW) & captureKey(kShift) & (defaultPlayer.stamina > 0)) {
			anchor = FVECTOR2(
				anchor.x - runningSpeed * cos(vecAngle(actor.pos, mousePos) degToRad),
				anchor.y - runningSpeed * sin(vecAngle(actor.pos, mousePos) degToRad)
			);
			if (defaultPlayer.stamina > 0) defaultPlayer.stamina -= 0.125f;
		}
		else if (captureKey(kW)) {
			anchor = FVECTOR2(
				anchor.x - pacificSpeed * cos(vecAngle(actor.pos, mousePos) degToRad),
				anchor.y - pacificSpeed * sin(vecAngle(actor.pos, mousePos) degToRad)
			);
		}
		if (captureKey(kS)) {
			anchor = FVECTOR2(
				anchor.x + pacificSpeed * cos(vecAngle(actor.pos, mousePos) degToRad),
				anchor.y + pacificSpeed * sin(vecAngle(actor.pos, mousePos) degToRad)
			);
		}
		break;
	case false:
		if (captureKey(kW)) moveAlongTo(actor.pos, FVECTOR2(mPos.x, mPos.y), 5.0f);
		if (captureKey(kS)) moveAlongTo(actor.pos, FVECTOR2(mPos.x, mPos.y), -5.0f);
		break;
	}
	if (!captureKey(kShift) & (defaultPlayer.stamina < 100)) defaultPlayer.stamina += 0.25f;
	if (captureKey(kR) & (defaultPlayer.selectedWpn.magCurrent < defaultPlayer.selectedWpn.magSize))
	{
		defaultPlayer.selectedWpn.magCurrent = defaultPlayer.selectedWpn.magSize;
		// Сделать изъятие патронов из инвентаря
	}
	if (captureKey(VK_LBUTTON) & (defaultPlayer.selectedWpn.magCurrent > 0))
	{
		// Сделать учёт скорострельности
		defaultPlayer.selectedWpn.magCurrent--;
	}
}

void Test::render() {
	clscr(black);
	if (!map && frameCounter == 1) {
		createBmp(L"resources/img/maps/LevelMap_Test.png", map);
		loadSkins();
		createWeapons();
		createActor(actor);
	}
	else if (frameCounter < 1) {
		text(L"-> Загрузка уровня: Компиляция текстур окружения...\n", FVECTOR2(15.0f, 15.0f), orangeRed, fixedsys);
		frameCounter++;
		return;
	}
	if (frameCounter == 1) fast_picture(map, anchor, 255.0f, 2.0f);
	drawHuman(actor, 90 + vecAngle(actor.pos, FVECTOR2(mPos.x, mPos.y)));
	lag.setTimeTo();
	showHud(defaultPlayer);
	drawLagometer(captureKey(kL), enLagometer, enLagometerPressed);
	drawCursor();
}


void Test::unload() {

}