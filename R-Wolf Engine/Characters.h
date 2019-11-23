#pragma once
#include "RWGraphics.h"
#include "RWVectorMath.h"

using namespace RWGraphics;
using namespace RWGraphics::Sprite;
using namespace RWMath;

namespace Marchenwelt_Characters {
	// Skins:
	constauto LONE_NOVICE_BODY = L"resources/img/textures/characters/lone_novice_body.png";
	constauto LONE_NOVICE_HEAD = L"resources/img/textures/characters/lone_novice_head.png";
	struct NPC {
		int location;
		FVECTOR2 position;
		//Item* inventory;
		//Weapon current;
		LPCWSTR name;
		LPCWSTR icon;
		LPCWSTR bodyTex;
		LPCWSTR headTex;
		LPCWSTR armTex;
		LPCWSTR legTex;
		NPC() {
			this->location = 0;
			this->position = FVECTOR2(0.0f, 0.0f);
			//this->inventory = new Item[1];
			this->name = L"Default NPC";
			this->icon = L"";
			LPCWSTR bodyTex = LONE_NOVICE_BODY;
			LPCWSTR headTex = LONE_NOVICE_HEAD;
		}
		NPC(int loc, FVECTOR2 pos) {
			this->location = loc;
			this->position = FVECTOR2(pos);
			//this->inventory = new Item[1];
			this->name = L"Default NPC";
			this->icon = L"";
			LPCWSTR bodyTex = LONE_NOVICE_BODY;
			LPCWSTR headTex = LONE_NOVICE_HEAD;
		}
		~NPC() {
			//delete[] this->inventory;
		}
	};
	struct Actor : NPC{
		int invSpace;
		float health;
	};
	method setActor(Actor _char) {
		return 0;
	}
	method drawCharacter(NPC obj, float angle) {
		matrixRotation(FVECTOR2(obj.position.x, obj.position.y), 85 + angle);
		picture(L"resources/img/textures/weapons/defaultWeapon.png", FVECTOR2(obj.position.x - 65.0f, obj.position.y - 25.0f), 255.0f);
		matrixRestore();
		matrixRotation(FVECTOR2(obj.position.x, obj.position.y), angle);
		picture(obj.bodyTex, FVECTOR2(obj.position.x - 29.0f, obj.position.y - 10.0f), 255.0f);
		picture(obj.headTex, FVECTOR2(obj.position.x - 10.0f, obj.position.y - 20.0f), 255.0f);
		matrixRestore();
	}
	method inventory() {

	}
}