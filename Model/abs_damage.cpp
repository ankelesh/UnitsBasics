#include "abs_damage.h"



namespace Model
{
	int Model::abs_damage::makeNoise()
	{
		return calcnoise();
	}

	int Model::abs_damage::countDamage(Defences& def, const int force, const bool isAttacking)
	{
		if (canPenetrate(def))
			return (int)(calcdmg(force, isAttacking) * (1 - calcResist(def)) - calcDP(def));
		return 0;
	}

	int Model::abs_damage::inRange(const int rng)
	{
		return canReach(rng);
	}

	int Model::abs_damage::getInitiative(const bool isAttacking)
	{
		return calcIni(isAttacking);
	}

	int Model::abs_damage::countDamageToForts(Defences& def, const bool isAttacking)
	{
		return damageFortifications(def, isAttacking);
	}

	OSTR_TYPE abs_damage::serialize()
	{
		return serialization();
	}
	abs_damage* Model::abs_damage::clone()
	{
		return fabricate();
	}
}