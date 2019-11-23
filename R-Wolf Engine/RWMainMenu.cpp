#include "RWGraphics.h"
#include "RWController.h"
#include "RWMainMenu.h"
#include <iostream>
#include <string>
#include <fstream>

#define _RW_USE_GLOBAL_DEFINITIONS
#define _RW_USE_PHYSICS_CONSTANTS
#include "RWGlobalDefinitions.h"
#include "RWPhysicsConstants.h"

void RWMainMenu::Load() {
	tex = new RWSpriteSystem(L"texture.png", core);
	picture = new RWSpriteSystem(L"test.png", core, 128, 128);
	mapsize = XMFLOAT2(4096, 1024);
	encounter = 0;
	action = 0;
	objectpos = XMFLOAT2(800, 100);
	startpos = XMFLOAT2(0, 0);
	dtime = 0;
	ttime = 0;
	buildname = L"Maverick [Pre-Alpha edition]";
	buildnum = L"0034";
	con_in = '\0';
	speed = 4;
	//sound->initialize();
}

void RWMainMenu::Update(double timeTotal, double timeDelta) {
	dtime = timeDelta;
	ttime = timeTotal;
	if (objectpos.x <= 15) {
		startpos.x += speed;
		objectpos.x = 15 - speed/2;
	}
	else if (objectpos.x >= SW - 143) {
		startpos.x -= speed;
		objectpos.x = SW - 143 - speed/2;
	}
	if (objectpos.y <= 15) {
		startpos.y += speed;
		objectpos.y = 15 - speed/2;
	}
	else if (objectpos.y >= SH - 143) {
		startpos.y -= speed;
		objectpos.y = SH - 143 - speed/2;
	}
	if (GetAsyncKeyState(VK_UP)) {
		objectpos.y -= speed;
		anim = 2;
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		objectpos.y += speed;
		anim = 2;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		objectpos.x -= speed;
		anim = 2;
	}

	else if (GetAsyncKeyState(VK_RIGHT)) {
		objectpos.x += speed;
		anim = 2;
	}

	if (GetAsyncKeyState(VK_LBUTTON)) {
		anim = 1;
		action = 1;
	}
	encounter++;
}

void RWMainMenu::Render() {
	core->d_info("begin");
	core->clscr(XMFLOAT3(15, 15, 15));
	//core->rw_iface("open");
	tex->Texturize(startpos, mapsize, 25);
	picture->Draw(anim, objectpos, 255);
	if (action == 1) {
		item object = { L"rect", 1000, 1000 };
		core->obj(object, XMFLOAT2(objectpos.x + 144, objectpos.y + 48), XMFLOAT2(1000, 2000), ENTITY_BULLET);
		action = 1;
	}
		// Исполнение кода уровня
	ScriptInit("MainMenuScript.xml");
	//core->rw_iface("close");
	core->d_hware(ttime, buildname, buildnum);
	core->d_menu();
	core->d_info("end");
}

void RWMainMenu::Unload() {
	delete picture;
	delete tex;
}