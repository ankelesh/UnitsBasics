#include "infantry_units.h"

namespace Model {
	Model::InfantryUnit::InfantryUnit(unsigned int f, std::vector<damagePointer>& w, Defences& def, frontmaps& fm, std::wstring nm)
		: abs_unit(nm, f, w, def, fm)
	{
	}

	Model::EGA_infantry::EGA_infantry(unsigned int f, std::vector<damagePointer>& w, Defences& def, frontmaps& fm, std::wstring nm)
		: InfantryUnit(f, w, def, fm, nm)
	{
	}

	bool Model::EGA_infantry::takeDamage(unsigned int dmg)
	{
		return false;
	}

	int Model::EGA_infantry::collectDamage(UnitPointer uptr, bool isAttacking)
	{
		return 0;
	}

	const unit_map_view_charset Model::EGA_infantry::mapView() const
	{
		std::wstring t;
		return unit_map_view_charset(frontmap,t, 1, 1, false,  force, 2);
	}
	abs_unit* EGA_infantry::fabricate()
	{
		return new EGA_infantry(force, weapons, defences, frontmap, name);
	}
}