//	Базовый заголовочный файл с краткими названиями функций и т.д.
//	Версия 1.2
//	Последнее обновление: 16.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once

//	Графические инструменты


//	RW1.2 : Расширить и реализовать, либо упразднить
enum Actions {
	RW_SYSTEM_CONSOLE = 0,
	RW_ACTION_SHOOT = 1,
	RW_ACTION_SWITCH_WEAPONS = 2,
	RW_ACTION_JUMP = 3,
	RW_ACTION_EQUIP_SHIELD = 4,
	RW_ACTION_DEQUIP_SHIELD = 5,
	RW_ACTION_THROW_GRENADE = 6,
	RW_ACTION_TAKE_ITEM = 7,
	RW_ACTION_DROP_ITEM = 8,
	RW_ACTION_INVENTORY = 9,
	RW_ACTION_USE = 10,
	RW_ACTION_RUN = 11,
	RW_ACTION_STEALTH = 12
};