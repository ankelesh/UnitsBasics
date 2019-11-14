#pragma once
#include "infantry_weapons.h"

/*
		Этот файл содержит заранее объявленные константы для инициализации будущих объектов
		используя неспециализированные классы InfantryWeapons и его потомков чтобы избежать 
		дублирования классов оружия, отличающихся лишь базовыми характеристиками, а не механикой
*/


namespace Model
{
	//------------EGA----------------------------------------------------------------------------------
	const damagePointer EGA_SAR_2(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"SAR-2", 10, false, 2, 0, 1, 8 })
		));
		// АКД-2 Автомат Космический Десантный - основное оружие всей пехоты Альянса Земных Государств.
	const damagePointer EGA_SSR_2(
		new SniperWeapons(
			standard_infantry_weapon_charset({ L"SSR-2", 4, false, 2, 0, 1, 9 }), 1, 4
		));
		// СВК-2 Снайперская Винтовка Космическая - снайперское оружие Альянса
	const damagePointer EGA_SASMG_3(
		new SubmachWeapons(
			standard_infantry_weapon_charset({ L"SASMG-3", 6, false, 2, 0, 1, 2 })
		));
		// ППК-3 Пистолет-Пулемёт Космический - основные ПП Альянса
	const damagePointer EGA_SLMG_1(
		new LightMachWeapons(
			standard_infantry_weapon_charset({ L"SLMG-1", 6, false, 2, 0, 1, 5 }), 4
		));
		// ПК Пулемёт Космический - основные ручные пулемёты Альянса
	const damagePointer EGA_SHMG_1(
		new HeavyMachWeapons(
			standard_infantry_weapon_charset({ L"SHMG-1", 6, false, 10, 0, 1, 6 })
		));
		// СПК Станковый Пулемёт Космический - основной тяжёлый пулемёт Альянса
	//------------PI------------------------------------------------------------------------------------
	const damagePointer PI_AW_8(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"AW-8", 16, false, 3, 0, 1, 9 })
		));
		// ГА-8, основное оружие пехоты пиратов - отличается меньшей убойной силой, но большей инициативой
	const damagePointer PI_EP_4(
		new SniperWeapons(
			standard_infantry_weapon_charset({ L"EP-4", 3, false, 2, 0, 1, 10 }), 1, 2
		));
		// ГП-4, снайперская винтовка пиратов приспособлена для партизанских боёв и отличается меньшими
		// требованиями к количеству прикрывающих войск
	const damagePointer PI_BI_6(
		new LightMachWeapons(
			standard_infantry_weapon_charset({ L"BI-6", 5, false, 2, 0, 1, 4 }), 5
		));
		// БН-4, лёгкий пулемёт пиратов отличается меньшим уроном и меньшими требованиями к силе отряда
	//------------RE------------------------------------------------------------------------------------
	const damagePointer RE_SMG_14mm(
		new SubmachWeapons(
			standard_infantry_weapon_charset({ L"SMG 14mm", 7, false, 10, 0, 1, 1 })
		));
		// Рокский 14мм ПП предназначен исключительно для снаряжения тыловых подразделений и отрядов террора 
	const damagePointer RE_SMG_16mm(
		new SubmachWeapons(
			standard_infantry_weapon_charset({ L"SMG 16mm", 8, false, 15, 0, 1, 1 })
		));
		// Рокский 16мм ПП используется для штурмов
	const damagePointer RE_SLR_25mm(
		new RockInfantryWeapons(
			standard_infantry_weapon_charset({ L"SLR 25mm", 20, false, 40, 0, 1, 7 })
		));
		// Рокская 25мм затворная винтовка используется как основное вооружение рокской пехоты.
		// Её бронебойность достаточна для охоты на лёгкую технику, но инициатива ниже аналогов
	const damagePointer RE_AR_20mm(
		new RockInfantryWeapons(
			standard_infantry_weapon_charset({ L"AR 20mm", 20, false, 30, 0, 1, 8 })
		));
		// Рокская 20мм штурмовая винтовка предназначена для особых частей и равна в остальном
		// винтовкам альянса
	const damagePointer RE_SR_35mm(
		new RockSniperWeapons(
			standard_infantry_weapon_charset({ L"SR 35mm", 5, false, 60, 0, 1, 9 })
			, 1, 4));
		// Рокская 35мм снайперская винтовка является скорее противотанковым нежели противопехотным
		// оружием
	//------------CR-----------------------------------------------------------------------------------
	const damagePointer CR_MU_SB(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"Musket", 12, false, 1, 0, 1, 3 })
		));
		// Гладкоствольные ружья являются основой вооружения пехоты криогеников и их характеристики
	// крайне недостаточны для равного противостояния с ксено-пехотой
	const damagePointer CR_MU_TH(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"Musket", 14, false, 1, 0, 1, 5 })
		));
		// Нарезные винтовки состоят на вооружении ветеранской пехоты криогеников.
	const damagePointer CR_NP(
		new InfantryWeapons(
			standard_infantry_weapon_charset({ L"Pistol", 8, false, 1, 0, 1, 1 })
		));
		// Кремниевые пистолеты являются оружием последнего шанса у военной аристократии криогеников




}