#include "abs_damage.h"




int Model::abs_damage::makeNoise()
{
	return calcnoise();
}

int Model::abs_damage::countDamage(Defences& def, const int force)
{

	return (int)(calcdmg(force)* (1 - calcResist(def)) - calcDP(def));
}

int Model::abs_damage::inRange(const int rng)
{
	return canReach(rng);
}
