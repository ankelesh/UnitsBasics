#pragma once
#include "infantry_weapons.h"

/*
		Ётот файл содержит заранее объ€вленные константы дл€ инициализации будущих объектов
		использу€ неспециализированные классы InfantryWeapons и его потомков чтобы избежать 
		дубли классов оружи€, отличающихс€ лишь базовыми характеристиками, а не механикой
*/


namespace Model
{
	//------------EGA----------------------------------------------------------------------------------
	const damagePointer EGA_SAR_2(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"SAR-2", 20, false, 2, 0, 1, 8 })
		));
	const damagePointer EGA_SSR_2(
		new SniperWeapons(
			standard_infantry_weapon_charset({ L"SSR-2", 4, false, 2, 0, 1, 9 }), 1, 4
		));
	const damagePointer EGA_SASMG_3(
		new SubmachWeapons(
			standard_infantry_weapon_charset({ L"SASMG-3", 6, false, 2, 0, 1, 2 })
		));
	const damagePointer EGA_SLMG_1(
		new LightMachWeapons(
			standard_infantry_weapon_charset({ L"SLMG-1", 6, false, 2, 0, 1, 5 }), 4
		));
	const damagePointer EGA_SHMG_1(
		new HeavyMachWeapons(
			standard_infantry_weapon_charset({ L"SHMG-1", 6, false, 10, 0, 1, 6 })
		));
	//------------PI------------------------------------------------------------------------------------
	const damagePointer PI_AW_8(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"AW-8", 16, false, 3, 0, 1, 9 })
		));
	const damagePointer PI_EP_4(
		new SniperWeapons(
			standard_infantry_weapon_charset({ L"EP-4", 3, false, 2, 0, 1, 10 }), 1, 2
		));
	const damagePointer PI_BI_6(
		new LightMachWeapons(
			standard_infantry_weapon_charset({ L"BI-6", 5, false, 2, 0, 1, 4 }), 5
		));
	//------------RE------------------------------------------------------------------------------------
	const damagePointer RE_SMG_14mm(
		new SubmachWeapons(
			standard_infantry_weapon_charset({ L"SMG 14mm", 7, false, 10, 0, 1, 1 })
		));
	const damagePointer RE_SMG_16mm(
		new SubmachWeapons(
			standard_infantry_weapon_charset({ L"SMG 16mm", 8, false, 15, 0, 1, 1 })
		));
	const damagePointer RE_SLR_25mm(
		new RockInfantryWeapons(
			standard_infantry_weapon_charset({ L"SLR 25mm", 20, false, 40, 0, 1, 7 })
		));
	const damagePointer RE_AR_20mm(
		new RockInfantryWeapons(
			standard_infantry_weapon_charset({ L"AR 20mm", 20, false, 30, 0, 1, 8 })
		));
	const damagePointer RE_SR_35mm(
		new RockSniperWeapons(
			standard_infantry_weapon_charset({ L"SR 35mm", 5, false, 60, 0, 1, 9 })
			, 1, 4));
	//------------CR-----------------------------------------------------------------------------------
	const damagePointer CR_MU_SB(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"Musket", 12, false, 1, 0, 1, 3 })
		));
	const damagePointer CR_MU_TH(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"Musket", 14, false, 1, 0, 1, 4 })
		));
	const damagePointer CR_NP(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"Pistol", 8, false, 1, 0, 1, 1 })
		));




}