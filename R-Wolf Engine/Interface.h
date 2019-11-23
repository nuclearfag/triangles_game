#pragma once
#include "RWGraphics.h"

using namespace RWGraphics;
using namespace RWGraphics::Colors;

namespace Marchenwelt_Interface {
	FONTPROPERTY header{
		L"Fixedsys Excelsior 3.01",
		24.0f,
		DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL
	};
	FONTPROPERTY content{
		L"Fixedsys Excelsior 3.01",
		18.0f,
		DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL
	};
	enum ItemTypes {
		COMMON = 0,
		UNCOMMON,
		RARE,
		LEGENDARY,
		EPIC
	};
	struct ItemStats {
		bool isUsing;
		float quality;
		float charge;
		float strength;
		float armor;
		float healing;
		float vivacity;
		float electrical_protection;
		float chemical_protection;
		float gravity_protection;
		float fire_protection;
		float exlplosion_protection;
		float bullet_protection;

	};
	struct Item {
		int id;
		ItemStats stat;
		LPCWSTR icon;
		wstring name;
		wstring descr;
		wstring typestr;
		int type;
		int count;
		Item() {
			memset(this, 0, sizeof(*this));
		}
		Item(int _id, LPCWSTR _ico, wstring _name, wstring _descr, int _type, int _count) {
			this->id = _id;
			this->icon = _ico;
			this->name = _name;
			this->descr = _descr;
			this->type = _type;
			this->count = _count;
			switch (this->type) {
			case COMMON:
				this->typestr = L"Обычный\n";
				break;
			case UNCOMMON:
				this->typestr = L"Необычный\n";
				break;
			case RARE:
				this->typestr = L"Редкий\n";
				break;
			case LEGENDARY:
				this->typestr = L"Легендарный\n";
				break;
			case EPIC:
				this->typestr = L"Эпический\n";
				break;
			}
		}
	};
	struct Weapon : Item {
		LPCWSTR actionIco;
		float dmg;
		float liveDistance;
		Item ammo;
		int magSize;
		int magCurrent;
		Weapon() {
			memset(this, 0, sizeof(*this));
		}
		Weapon(LPCWSTR aico, int id, LPCWSTR ico, wstring nam, wstring desc, int typ, int coun,
			float dm, float livedist, Item amm, int msize, int mcurr) {
			this->actionIco = aico;
			this->id = id;
			this->icon = ico;
			this->name = nam;
			this->descr = desc;
			this->type = typ;
			this->count = coun;
			this->dmg = dm;
			this->liveDistance = livedist;
			this->ammo = amm;
			this->magSize = msize;
			this->magCurrent = mcurr;
			switch (this->type) {
			case COMMON:
				this->typestr = L"Обычный\n";
				break;
			case UNCOMMON:
				this->typestr = L"Необычный\n";
				break;
			case RARE:
				this->typestr = L"Редкий\n";
				break;
			case LEGENDARY:
				this->typestr = L"Легендарный\n";
				break;
			case EPIC:
				this->typestr = L"Эпический\n";
				break;
			}
		}
	};
	struct Equipment : Item {
		int availSize;
		Item* items;
		Equipment() {
			memset(this, 0, sizeof(*this));
		}
		Equipment(int size) {
			this->availSize = size;
			this->items = new Item[this->availSize];
		}
	};
	struct Artefact : Item {

	};
	struct Rucksack : Equipment {
		int size;
		Rucksack() {
			memset(this, 0, sizeof(*this));
		}
		Rucksack(int size) {
			this->size = size;
			this->items = new Item[this->size];
		}
		~Rucksack() {
			delete[] this->items;
		}
	};
	struct Task {
		wstring head;
		wstring descr;
		bool isCompleted;
		int experience;
		Task() {
			memset(this, 0, sizeof(*this));
		}
		Task(wstring head, wstring descr, bool isc, int exp) {
			this->head = head;
			this->descr = descr;
			this->isCompleted = isc;
			this->experience = exp;
		}
	};
	struct PlayerStats {
		float hp;
		float stamina;
		Weapon selectedWpn;
		Task selectedTask;
		PlayerStats() {
			memset(this, 0, sizeof(*this));
		}
		PlayerStats(int hp, int stamina, Weapon selectedWp, Task selectedTask) {
			this->hp = hp;
			this->stamina = stamina;
			this->selectedWpn = selectedWp;
			this->selectedTask = selectedTask;
		}
	};
	struct PlayerInventory {
		Equipment head, body, legs, foots, arms;
		Weapon left, right;
		Artefact first, second, third;
		Rucksack storage;
	};
	Item defaultItem{
		-65535,
		L"resources/img/icons/defaultItem.png",
		L"Стандартный предмет\n",
		L"Обычный предмет, каких много.\nНичего из себя не представляет.",
		COMMON,
		10
	};
	Weapon defaultWeapon{
		L"resources/img/textures/weapons/defaultWeapon.png",
		-65534,
		L"resources/img/icons/defaultWeapon.png",
		L"Винтовка - это праздник\n",
		L"Оружие неизвестного происхождения\n",
		COMMON,
		1,
		35.25f,
		180.05f,
		defaultItem,
		30,
		0
	};
	Task defaultTask{
		L"Тестовая строка\n",
		L"1111111111\n",
		false,
		1000
	};
	PlayerStats defaultPlayer{
		100,
		80,
		defaultWeapon,
		defaultTask
	};
	method showHud(PlayerStats stat) {
		// Задание
		text(stat.selectedTask.head, FVECTOR2(15.0f, 15.0f), orangeRed, header);
		text(stat.selectedTask.descr, FVECTOR2(15.0f, 40.0f), white, content);
		// ХП и стамина
		picture(L"resources/img/icons/health.png", FVECTOR2(getConfigString("X") - 347.0f,
			getConfigString("Y") - 96.0f), 255.0f);
		picture(L"resources/img/icons/stamina.png", FVECTOR2(getConfigString("X") - 347.0f,
			getConfigString("Y") - 56.0f), 255.0f);
		rect(
			TWDRECT(
				FVECTOR2(getConfigString("X") - 315.0f,
					getConfigString("Y") - 96.0f),
				FVECTOR2(getConfigString("X") - 315.0f + 3.0f * stat.hp,
					getConfigString("Y") - 72.0f)),
			red,
			1.0f);
		rect(
			TWDRECT(
				FVECTOR2(getConfigString("X") - 315.0f,
					getConfigString("Y") - 56.0f),
				FVECTOR2(getConfigString("X") - 315.0f + 3.0f * stat.stamina,
					getConfigString("Y") - 32.0f)),
			blue,
			1.0f);
		rect(
			TWDRECT(
				FVECTOR2(getConfigString("X") - 315.0f,
					getConfigString("Y") - 96.0f),
				FVECTOR2(getConfigString("X") - 15.0f,
					getConfigString("Y") - 72.0f)),
			darkGray,
			2.0f,
			false);
		rect(
			TWDRECT(
				FVECTOR2(getConfigString("X") - 315.0f,
					getConfigString("Y") - 56.0f),
				FVECTOR2(getConfigString("X") - 15.0f,
					getConfigString("Y") - 32.0f)),
			darkGray,
			2.0f,
			false);
		// Оружие
		line(TWDLINE(FVECTOR2(
				getConfigString("X") - 295.0f,
				getConfigString("Y") - 196.0f
			), 
			FVECTOR2(
				getConfigString("X") - 15.0f,
				getConfigString("Y") - 196.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 15.0f,
			getConfigString("Y") - 196.0f
			),
			FVECTOR2(
				getConfigString("X") - 15.0f,
				getConfigString("Y") - 120.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 15.0f,
			getConfigString("Y") - 120.0f
			),
			FVECTOR2(
				getConfigString("X") - 31.0f,
				getConfigString("Y") - 104.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 31.0f,
			getConfigString("Y") - 104.0f
		),
			FVECTOR2(
				getConfigString("X") - 303.0f,
				getConfigString("Y") - 104.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 303.0f,
			getConfigString("Y") - 104.0f
		),
			FVECTOR2(
				getConfigString("X") - 303.0f,
				getConfigString("Y") - 188.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 303.0f,
			getConfigString("Y") - 188.0f
		),
			FVECTOR2(
				getConfigString("X") - 295.0f,
				getConfigString("Y") - 196.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 367.0f,
			getConfigString("Y") - 156.0f
		),
			FVECTOR2(
				getConfigString("X") - 303.0f,
				getConfigString("Y") - 156.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 367.0f,
			getConfigString("Y") - 116.0f
		),
			FVECTOR2(
				getConfigString("X") - 367.0f,
				getConfigString("Y") - 180.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 367.0f,
			getConfigString("Y") - 140.0f
		),
			FVECTOR2(
				getConfigString("X") - 303.0f,
				getConfigString("Y") - 140.0f
			)),
			2.0f,
			darkBlue);
		text(to_wstring(stat.selectedWpn.magCurrent), FVECTOR2(getConfigString("X") - 364.0f,
			getConfigString("Y") - 178.0f), white, content);
		text(to_wstring(stat.selectedWpn.ammo.count), FVECTOR2(getConfigString("X") - 364.0f,
			getConfigString("Y") - 137.0f), white, content);
		picture(stat.selectedWpn.icon, FVECTOR2(getConfigString("X") - 287.0f,
			getConfigString("Y") - 180.0f), 255.0f);
		// Патроны
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 475.0f,
			getConfigString("Y") - 196.0f
		),
			FVECTOR2(
				getConfigString("X") - 399.0f,
				getConfigString("Y") - 196.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 399.0f,
			getConfigString("Y") - 196.0f
		),
			FVECTOR2(
				getConfigString("X") - 383.0f,
				getConfigString("Y") - 180.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 383.0f,
			getConfigString("Y") - 180.0f
		),
			FVECTOR2(
				getConfigString("X") - 383.0f,
				getConfigString("Y") - 104.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 383.0f,
			getConfigString("Y") - 104.0f
		),
			FVECTOR2(
				getConfigString("X") - 467.0f,
				getConfigString("Y") - 104.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 475.0f,
			getConfigString("Y") - 112.0f
		),
			FVECTOR2(
				getConfigString("X") - 475.0f,
				getConfigString("Y") - 196.0f
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			getConfigString("X") - 475.0f,
			getConfigString("Y") - 112.0f
		),
			FVECTOR2(
				getConfigString("X") - 467.0f,
				getConfigString("Y") - 104.0f
			)),
			2.0f,
			darkBlue);
		picture(stat.selectedWpn.ammo.icon, FVECTOR2(getConfigString("X") - 459.0f,
			getConfigString("Y") - 180.0f), 255.0f);
	}
	method showInfo(FVECTOR2 mousePos, Item item) {
		RGBA clr;
		switch (item.type) {
		case COMMON:
			clr = white;
			break;
		case UNCOMMON:
			clr = lightGreen;
			break;
		case RARE:
			clr = aliceBlue;
			break;
		case LEGENDARY:
			clr = yellow;
			break;
		case EPIC:
			clr = orangeRed;
			break;
		}
		line(TWDLINE(mousePos, FVECTOR2(
			mousePos.x + 16.0f * strwidth(item.name),
			mousePos.y
		)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			mousePos.x,
			mousePos.y + 30
		),
			FVECTOR2(
				mousePos.x + 16.0f * strwidth(item.name) - 40,
				mousePos.y + 30
			)),
			2.0f,
			yellow);
		line(TWDLINE(FVECTOR2(
			mousePos.x + 16.0f * strwidth(item.name),
			mousePos.y
		),
			FVECTOR2(
				mousePos.x + 16.0f * strwidth(item.name) + 25,
				mousePos.y + 25
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			mousePos.x + 16.0f * strwidth(item.name) + 25,
			mousePos.y + 25
		),
			FVECTOR2(
				mousePos.x + 16.0f * strwidth(item.name) + 25,
				mousePos.y + 30 + 18.0f * strheight(item.descr) + 25
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			mousePos.x,
			mousePos.y
		),
			FVECTOR2(
				mousePos.x,
				mousePos.y + 50 + 18.0f * strheight(item.descr)
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			mousePos.x,
			mousePos.y + 50 + 18.0f * strheight(item.descr)
		),
			FVECTOR2(
				mousePos.x + 5,
				mousePos.y + 30 + 18.0f * strheight(item.descr) + 25
			)),
			2.0f,
			darkBlue);
		line(TWDLINE(FVECTOR2(
			mousePos.x + 5,
			mousePos.y + 30 + 18.0f * strheight(item.descr) + 25
		),
			FVECTOR2(
				mousePos.x + 16.0f * strwidth(item.name) + 25,
				mousePos.y + 30 + 18.0f * strheight(item.descr) + 25
			)),
			2.0f,
			darkBlue);
		text(item.name, FVECTOR2(mousePos.x + 4.0f, mousePos.y + 4.0f), white, header);
		text(item.descr, FVECTOR2(mousePos.x + 4.0f, mousePos.y + 30.0f), white, content);
		text(item.typestr, FVECTOR2(mousePos.x + 16.0f * strwidth(item.name) - 80, mousePos.y + 35 + 18.0f * strheight(item.descr)), clr, content);
	}
	method showInventory(PlayerInventory &inv) {
		rect(TWDRECT(
			FVECTOR2(64.0f, 64.0f),
			FVECTOR2(192.0f, 192.0f)
		),
			darkBlue,
			2.0f,
			false);
		picture(inv.head.icon, FVECTOR2(64.0f, 64.0f), 255.0f);
	}
}