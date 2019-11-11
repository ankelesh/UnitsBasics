#include "abs_damage.h"



namespace Model
{
	/*
		“ут собраны не-виртуальные методы интерфейса, которые инкапсулируют вызовы виртуальных.
		“ак же тут выставл€етс€ общее поведение навроде формулы урона
	*/
	int Model::abs_damage::makeNoise()
		// ¬озвращает уровень произведЄнного шума
	{
		return calcnoise();
	}

	int Model::abs_damage::countDamage(Defences& def, const int force, const bool isAttacking)
		// ’ранит в себе формулу нанесени€ урона в наибольшей абстракции
	{
		if (canPenetrate(def))			// ≈сли можем пробить броню:
			return (int)(			// округление вниз. ¬озможно лучше делать это матфункцией std::floor
				calcdmg(force, isAttacking)		// урон рассчитывает то, как он нанесЄтс€ в идеальных услови€х
				* (1 -							// потом домножаетс€ на обратное значение сопротивлени€
					calcResist(def))
				- calcDP(def));					// потом вычитаетс€ динамическа€ защита
		return 0;						// иначе экономим врем€
	}

	int Model::abs_damage::inRange(const int rng)
		// возвращаем возможность нанесени€ урона на рассто€нии rng
	{
		return canReach(rng);
	}

	int Model::abs_damage::getInitiative(const bool isAttacking)
		// возвращаем инициативу оружи€
	{
		return calcIni(isAttacking);
	}

	int Model::abs_damage::countDamageToForts(Defences& def, const bool isAttacking)
		// считаем сколько урона будет нанесено укреплени€м. Ќекоторое оружие повреждает их даже в обороне.
	{
		return damageFortifications(def, isAttacking);
	}

	OSTR_TYPE abs_damage::serialize()
		// превращает любой тип урона в строку пригодную дл€ записи в файл
	{
		return serialization();
	}
	abs_damage* Model::abs_damage::clone()
		// создаЄт точную копию любого типа урона возвраща€ его как абстраетный поинтер
	{
		return fabricate();
	}
}