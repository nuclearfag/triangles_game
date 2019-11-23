#pragma once
#include <memory>
#include "RWGraphics.h"
#include "RWVectorMath.h"

using namespace RWGraphics;
using namespace RWGraphics::Sprite;
using namespace RWMath;

// Базовая объектная модель игры
namespace Marchenwelt_BOM {
	// Скины предметов:
	enum skinsID {
		loner_novice_body = 1,
		loner_novice_head,
		loner_novice_arm,
		loner_novice_shoulder,
		loner_novice_leg,

		loner_novice_icon_1 = 512,

		weapon_ak74 = 1024,

		ammo_545x39 = 2048
	};
	// Хранилища объектов:
	vector<LPCWSTR> skins(65535);
	method addSkin(vector<LPCWSTR> &skins, LPCWSTR path, int skinID = -2) {
		if (!(skinID + 2)) {
			skins.push_back(path);
		}
		else {
			skins[skinID] = path;
		}
	}
	method loadSkins() {
		skins[loner_novice_body] = L"resources/img/textures/characters/lone_novice_body.png";
		skins[loner_novice_head] = L"resources/img/textures/characters/lone_novice_head.png";
		skins[loner_novice_arm] = L"resources/img/textures/characters/lone_novice_arm.png";
		skins[loner_novice_shoulder] = L"resources/img/textures/characters/lone_novice_shoulder.png";
	}
		
	// Локации:
	enum Locations {
		loc_armybase = 0
	};
	// Характеристика предмета
	struct ItemStats {
		ItemStats() {
			memset(this, 0, sizeof(*this));
		}
		~ItemStats() {
			memset(this, 0, sizeof(*this));
		}
		ItemStats(
			float istQuality,
			float istCharge,
			float istStrength,
			float istArmor,
			float istHealing,
			float istVivacity,
			float istElectricalProtection,
			float istChemicalProtection,
			float istGravityProtection,
			float istFireProtection,
			float istExplosionProtection,
			float istBulletProtection
		) {
			this->isUsing = true;
			this->quality = istQuality;
			this->charge = istCharge;
			this->strength = istStrength;
			this->armor = istArmor;
			this->healing = istHealing;
			this->vivacity = istVivacity;
			this->electrical_protection = istElectricalProtection;
			this->chemical_protection = istChemicalProtection;
			this->gravity_protection = istGravityProtection;
			this->fire_protection = istFireProtection;
			this->exlplosion_protection = istExplosionProtection;
			this->bullet_protection = istBulletProtection;
		}
		method use() {
			this->isUsing = true;
		}
		method nouse() {
			this->isUsing = false;
		}
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
	// Типы предметов
	enum ItemTypes {
		HELMET = 0,
		HELMET_ADDON_NIGHTVISION,
		HELMET_ADDON_ARMOR,
		HELMET_ADDON_FLASHLIGHT,
		HELMET_ADDON_CAMO,
		VEST,
		VEST_ADDON_ARMOR,
		VEST_ADDON_FRACTION,
		VEST_ADDON_CAMO,
		PANTS,
		PANTS_ADDON_CAMO,
		HANDS,
		HANDS_ADDON_CAMO,
		FOOTS,
		FOOTS_ADDON_CAMO,
		WEAPON_PISTOL,
		WEAPON_PISTOL_ADDON_SILENCER_9X18,
		WEAPON_PISTOL_ADDON_SILENCER_9X19,
		WEAPON_PISTOL_ADDON_SILENCER_9X39,
		WEAPON_PISTOL_ADDON_SILENCER_45ACP,
		WEAPON_PISTOL_ADDON_SILENCER_50AE,
		WEAPON_PISTOL_ADDON_SILENCER_TT,
		WEAPON_PISTOL_ADDON_SIGHT_PIC_HOLO,
		WEAPON_PISTOL_ADDON_SIGHT_PIC_COL,
		WEAPON_PISTOL_ADDON_SIGHT_WEA_HOLO,
		WEAPON_PISTOL_ADDON_SIGHT_WEA_COL,
		WEAPON_PISTOL_ADDON_MAG_GLOCK_15,
		WEAPON_PISTOL_ADDON_MAG_GLOCK_30,
		WEAPON_PISTOL_ADDON_MAG_GLOCK_45,
		WEAPON_PISTOL_ADDON_MAG_GLOCK_90,
		WEAPON_RIFLE,
		WEAPON_RIFLE_ADDON_SILENCER_5X45,
		WEAPON_RIFLE_ADDON_SILENCER_5X56,
		WEAPON_RIFLE_ADDON_SILENCER_335L,
		WEAPON_RIFLE_ADDON_SILENCER_762X39,
		WEAPON_RIFLE_ADDON_SILENCER_762X54,
		WEAPON_RIFLE_ADDON_SILENCER_50BMG,
		WEAPON_RIFLE_ADDON_SILENCER_127G,
		WEAPON_RIFLE_ADDON_SCOPE_PIC_HOLO,
		WEAPON_RIFLE_ADDON_SCOPE_PIC_COL,
		WEAPON_RIFLE_ADDON_SCOPE_PIC_ACOG,
		WEAPON_RIFLE_ADDON_SCOPE_PIC_2X,
		WEAPON_RIFLE_ADDON_SCOPE_PIC_4X,
		WEAPON_RIFLE_ADDON_SCOPE_PIC_8X,
		WEAPON_RIFLE_ADDON_SCOPE_PIC_16X,
		WEAPON_RIFLE_ADDON_SCOPE_WEA_HOLO,
		WEAPON_RIFLE_ADDON_SCOPE_WEA_COL,
		WEAPON_RIFLE_ADDON_SCOPE_WEA_ACOG,
		WEAPON_RIFLE_ADDON_SCOPE_WEA_2X,
		WEAPON_RIFLE_ADDON_SCOPE_WEA_4X,
		WEAPON_RIFLE_ADDON_SCOPE_WEA_8X,
		WEAPON_RIFLE_ADDON_SCOPE_WEA_16X,
		WEAPON_RIFLE_ADDON_SCOPE_RUS_PSO,
		WEAPON_RIFLE_ADDON_SCOPE_RUS_EKP,
		WEAPON_RIFLE_ADDON_SCOPE_RUS_TO_PIC,
		WEAPON_RIFLE_ADDON_MAG_5X45_30,
		WEAPON_RIFLE_ADDON_MAG_5X45_45,
		WEAPON_RIFLE_ADDON_MAG_5X45_60,
		WEAPON_RIFLE_ADDON_MAG_5X45_2X30,
		WEAPON_RIFLE_ADDON_MAG_5X56_30,
		WEAPON_RIFLE_ADDON_MAG_5X56_45,
		WEAPON_RIFLE_ADDON_MAG_5X56_60,
		WEAPON_RIFLE_ADDON_MAG_5X56_90,
		WEAPON_RIFLE_ADDON_MAG_5X56_100,
		WEAPON_RIFLE_ADDON_MAG_5X56_200,
		WEAPON_RIFLE_ADDON_MAG_762X39_30,
		WEAPON_RIFLE_ADDON_MAG_762X39_45,
		WEAPON_RIFLE_ADDON_MAG_762X39_60,
		WEAPON_RIFLE_ADDON_MAG_762X39_2X30,
		WEAPON_RIFLE_ADDON_MAG_762X54_10,
		WEAPON_RIFLE_ADDON_MAG_762X54_15,
		WEAPON_RIFLE_ADDON_MAG_762X54_20,
		WEAPON_RIFLE_ADDON_MAG_127_10,
		WEAPON_RIFLE_ADDON_MAG_127_15,
		WEAPON_RIFLE_ADDON_MAG_127_20,
		WEAPON_RIFLE_ADDON_MAG_127_30,
		WEAPON_RIFLE_ADDON_MAG_127_45,
		WEAPON_RIFLE_ADDON_MAG_127_60,
		WEAPON_RIFLE_ADDON_MAG_50BMG_10,
		WEAPON_RIFLE_ADDON_MAG_335L_10,
		WEAPON_RIFLE_ADDON_MAG_335L_30,
		WEAPON_RIFLE_ADDON_BARREL_BIPOD,
		WEAPON_RIFLE_ADDON_BARREL_GRIP_HOR,
		WEAPON_RIFLE_ADDON_BARREL_GRIP_VER,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_NODEMP_NATO,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_NODEMP_RUS,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_NODEMP_M4,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_NODEMP_ACR,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_DEMP_NATO,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_DEMP_RUS,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_DEMP_M4,
		WEAPON_RIFLE_ADDON_BUTTSTOCK_DEMP_ACR,
		RUCKSACK_CAPAC1,
		RUCKSACK_CAPAC2,
		RUCKSACK_CAPAC5,
		RUCKSACK_CAPAC10,
		RUCKSACK_CAPAC15,
		RUCKSACK_CAPAC30,
		RUCKSACK_CAPAC50,
		RUCKSACK_CAPAC60,
		RUCKSACK_CAPAC80,
		RUCKSACK_CAPAC100,
		RUCKSACK_CAPAC120,
		ARTEFACT,
		AMMO_9X18,
		AMMO_9X19,
		AMMO_9X39,
		AMMO_45ACP,
		AMMO_50AE,
		AMMO_545X39,
		AMMO_556X45,
		AMMO_335L,
		AMMO_50BMG,
		AMMO_127,
		MEDKIT,
		QUEST,
		COMMON
	};
	enum ItemRarity {
		RARITY_COMMON = 0,
		RARITY_UNCOMMON,
		RARITY_RARE,
		RARITY_LEGENDARY,
		RARITY_EPIC
	};
	// Предмет
	struct Item {
		Item() {
			memset(this, 0, sizeof(*this));
		}
		~Item() {
			memset(this, 0, sizeof(*this));
		}
		ItemStats stat;
		int id;
		int type;
		int rarity;
		int cost;
		int count;
		LPCWSTR invIcon;
		LPCWSTR worldIcon;
		std::wstring name;
		std::wstring descr;
		Item(
			ItemStats iStats,
			int iID,
			int iType,
			int iRarity,
			int iCost,
			int iCount,
			LPCWSTR iInvIcon,
			LPCWSTR iWorldIcon,
			std::wstring iName,
			std::wstring iDescr
		) {
			this->stat = iStats;
			this->id = iID;
			this->type = iType;
			this->rarity = iRarity;
			this->cost = iCost;
			this->count = iCount;
			this->invIcon = iInvIcon;
			this->worldIcon = iWorldIcon;
			this->name = iName;
			this->descr = iDescr;
		}
		method createItem(
			ItemStats iStats,
			int iID,
			int iType,
			int iRarity,
			int iCost,
			int iCount,
			LPCWSTR iInvIcon,
			LPCWSTR iWorldIcon,
			std::wstring iName,
			std::wstring iDescr
		) {
			return Item(iStats, iID, iType, iRarity, iCost, iCount, iInvIcon, iWorldIcon, iName, iDescr);
		}
	};
	vector<Item> items(65535);
	method getItemByID(vector<Item> _Items, int itemId) {
		for (int iter = 0; iter < _Items.size(); iter++) {
			if (_Items[iter].id = itemId) {
				return _Items[iter];
			}
		}
	}
	method setItemByID(vector<Item> &items, Item item, int itemID = -2) {
		if (!(itemID + 2)) items.push_back(item);
		else items[itemID] = item;
	}
	method loadItems() {
		
	}
	// Дочерние структуры предметов
	struct Weapon : Item {
		float damage;
		float liveDistance;
		int ammoId;
		int magSize;
		int magElapsed;
		Weapon() {
			memset(this, 0, sizeof(*this));
		}
		~Weapon() {
			memset(this, 0, sizeof(*this));
		}
		Weapon(
			ItemStats iStats,
			int iID,
			int iType,
			int iRarity,
			int iCost,
			int iCount,
			LPCWSTR iInvIcon,
			LPCWSTR iWorldIcon,
			std::wstring iName,
			std::wstring iDescr,
			float wDamage,
			float wLiveDistance,
			int wMagSize,
			int wMagElapsed
		) {
			this->stat = iStats;
			this->id = iID;
			this->type = iType;
			this->rarity = iRarity;
			this->cost = iCost;
			this->count = iCount;
			this->invIcon = iInvIcon;
			this->worldIcon = iWorldIcon;
			this->name = iName;
			this->descr = iDescr;
			this->damage = wDamage;
			this->liveDistance = wLiveDistance;
			this->magSize = wMagSize;
			this->magElapsed = wMagElapsed;
		}
		method createWeapon(
			Weapon &wObject,
			ItemStats iStats,
			int iID,
			int iType,
			int iRarity,
			int iCost,
			int iCount,
			LPCWSTR iInvIcon,
			LPCWSTR iWorldIcon,
			std::wstring iName,
			std::wstring iDescr,
			float wDamage,
			float wLiveDistance,
			int wMagSize,
			int wMagElapsed
		) {
			wObject = Weapon(iStats, iID, iType, iRarity, iCost, iCount, iInvIcon, iWorldIcon, iName, iDescr, wDamage, wLiveDistance, wMagSize, wMagElapsed);
			return Item(iStats, iID, iType, iRarity, iCost, iCount, iInvIcon, iWorldIcon, iName, iDescr);
		}
	};
	Weapon ak74(ItemStats(), weapon_ak74, WEAPON_RIFLE, COMMON, 30000, 1,
		L"resources/img/icons/ak-74_inv.png",
		L"resources/img/textures/weapons/ak-74_world.png",
		L"AK-74 обр. 1974 года\n",
		L"Более совершенный по сравнению с АКМ автомат. Есть почти у каждого сталкера\n",
		39.45f, 500.0f, 30, 30);
	method createWeapons() {
		// Используй ItemStats() для объектов без специальных свойств
		// Все строки должны заканчиваться на \n, иначе будет тупить strheight!
		Item ammo545x39(ItemStats(), ammo_545x39, AMMO_545X39, COMMON, 1500, 30,
			L"resources/img/icons/545x39_fmj_inv.png",
			L"resources/img/icons/545x39_fmj_inv.png",
			L"Патроны 5.45х39 мм ЦМО\n",
			L"Боеприпасы для большинства видов оружия в Зоне\n");
		ak74.ammoId = ammo_545x39;
		setItemByID(items, ammo545x39, ammo_545x39);
		setItemByID(items, ak74, weapon_ak74);
	}
	struct Equipment : Item {
		Equipment() {
			memset(this, 0, sizeof(*this));
		}
		~Equipment() {
			memset(this, 0, sizeof(*this));
		}
	};
	struct Artefact : Item {

	};
	struct Rucksack : Equipment {

	};
	// НПС
	enum Groups {
		group_loner = 0
	};
	struct HumanTexture {
		LPCWSTR iconPath;
		LPCWSTR bodySkin;
		LPCWSTR headSkin;
		LPCWSTR armSkin;
		LPCWSTR shoulderSkin;
		LPCWSTR legSkin;
		HumanTexture() {
			memset(this, 0, sizeof(*this));
		}
		HumanTexture(LPCWSTR icon, LPCWSTR body, LPCWSTR head, LPCWSTR arm, LPCWSTR shoulder, LPCWSTR leg) {
			this->iconPath = icon;
			this->bodySkin = body;
			this->headSkin = head;
			this->armSkin = arm;
			this->shoulderSkin = shoulder;
			this->legSkin = leg;
		}
	};
	struct NPC {
		int loc;
		int rep;
		int group;
		FVECTOR2 pos;
		wstring name;
		HumanTexture pics;
		Weapon currentWpn;
		vector<Item> inventory;
		NPC() {
			memset(this, 0, sizeof(*this));
		}
		NPC(int loc, int rep, int group, FVECTOR2 pos,
			wstring name, HumanTexture pics, Weapon currentWpn,
			vector<Item> inventory) {
			this->loc = loc;
			this->rep = rep;
			this->group = group;
			this->pos = pos;
			this->name = name;
			this->pics = pics;
			this->currentWpn = currentWpn;
			this->inventory = inventory;
		}
	};
	HumanTexture stdLoner = {
		L"",
		skins[loner_novice_body],
		skins[loner_novice_head],
		skins[loner_novice_arm],
		skins[loner_novice_shoulder],
		skins[loner_novice_leg]
	};
	NPC actor;
	vector<Item> actorInventory(128);
	method createActor(NPC &actor) {
		actor = NPC(
			loc_armybase,
			0,
			group_loner,
			FVECTOR2(getConfigString("X") / 2, getConfigString("Y") / 2),
			L"Имя\n",
			HumanTexture(
				L"",
				skins[loner_novice_body],
				skins[loner_novice_head],
				skins[loner_novice_arm],
				skins[loner_novice_shoulder],
				skins[loner_novice_leg]
			),
			ak74,
			actorInventory
		);
	}

	// Секция методов
	method drawHuman(NPC &obj, float angle) {
		//matrixRotation(FVECTOR2(obj.pos.x, obj.pos.y), 85 + angle);
		picture(obj.currentWpn.worldIcon, FVECTOR2(obj.pos.x + 22.0f, obj.pos.y - 72.0f), 255.0f);
		//matrixRestore();
		matrixRotation(FVECTOR2(obj.pos.x, obj.pos.y), angle);
		picture(obj.pics.bodySkin, FVECTOR2(obj.pos.x - 32.0f, obj.pos.y - 16.0f), 255.0f);
		picture(obj.pics.shoulderSkin, FVECTOR2(obj.pos.x - 32.0f, obj.pos.y - 48.0f), 255.0f);
		picture(obj.pics.shoulderSkin, FVECTOR2(obj.pos.x + 22.0f, obj.pos.y - 48.0f), 255.0f);
		picture(obj.pics.armSkin, FVECTOR2(obj.pos.x - 32.0f, obj.pos.y - 72.0f), 255.0f);
		picture(obj.pics.armSkin, FVECTOR2(obj.pos.x + 22.0f, obj.pos.y - 72.0f), 255.0f);
		picture(obj.pics.headSkin, FVECTOR2(obj.pos.x - 16.0f, obj.pos.y - 32.0f), 255.0f);
		matrixRestore();
	}
}