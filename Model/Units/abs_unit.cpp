#include "abs_unit.h"

namespace Model {

	Model::abs_unit::abs_unit()
		: force(0), weapons(), defences(), frontmap(), name(L"None")
	{
	}

	Model::abs_unit::abs_unit(std::wstring nm, unsigned int Force, std::vector<damagePointer> w, Defences d, frontmaps f)
		: force(Force), weapons(), defences(d), frontmap(f), name(nm)
	{
		weapons.reserve(w.size());
		std::vector<damagePointer>::iterator start = w.begin();
		while (start != w.end())
		{
			weapons.push_back(damagePointer((*start++)->clone()));
		}
	}

	Model::abs_unit::abs_unit(std::wistream& wsin)
	{
		//TODO
	}

	bool Model::abs_unit::applyDamageToThis(unsigned int dmg)
	{
		return takeDamage(dmg);
	}

	int Model::abs_unit::countPureDamageTo(UnitPointer uptr, bool isAttacking)
	{
		return collectDamage(uptr, isAttacking);
	}

	const unit_map_view_charset& Model::abs_unit::getViewOfThis() const
	{
		return mapView();
	}
	abs_unit* abs_unit::clone()
	{
		return fabricate();
	}
}