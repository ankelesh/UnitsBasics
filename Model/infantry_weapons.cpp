#include "infantry_weapons.h"

OSTR_TYPE Model::EGA_SAR_2::serialization()
{
	return serialize(c);
}

void Model::EGA_SAR_2::deserialization(OSTR_TYPE& str)
{
	c = deserialize(str);
}

void Model::EGA_SAR_2::deserialization(std::wistream& stream)
{
	c = deserialize(stream);
}

int Model::EGA_SAR_2::calcDP(Defences& def)
{
	return 0;
}
int Model::EGA_SAR_2::calcnoise()
{
	return 0;
}
bool Model::EGA_SAR_2::canRetaliate()
{
	return c.bulky;
}
int Model::EGA_SAR_2::calcdmg(const int force)
{
	return (int) ((double)c.amount * ((double)force / 10));
}
bool Model::EGA_SAR_2::canReach(const int rng)
{
	return c.max_range >= rng;
}
bool Model::EGA_SAR_2::canPenetrate(Defences& def)
{
	return (c.penetration > def.getArmour(infantry)- 2);
}

double Model::EGA_SAR_2::calcResist(Defences& def)
{
	double percent = ((double)def.getResistanse(infantry)) / 100 + (
		(def.getArmour(infantry) > c.penetration) ? 0.1 : 0);
	if (percent > 1) percent = 1;
	return percent;
}

Model::abs_damage* Model::EGA_SAR_2::fabricate()
{
	return new EGA_SAR_2;
}

Model::EGA_SAR_2::EGA_SAR_2()
	:c(standard_infantry_weapon_charset({L"SAR-2", 20, false, 5, 0, 1}))
{
}

Model::EGA_SAR_2::EGA_SAR_2(const standard_infantry_weapon_charset& chs)
{
	c = chs;
}
