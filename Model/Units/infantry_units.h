#pragma once
#include "abs_unit.h"


/*
	Этот класс создан для юнита пехоты

*/
namespace Model {
	class InfantryUnit : public abs_unit
	{
	protected:
		virtual bool takeDamage(unsigned int dmg) override =0;
		virtual int collectDamage(UnitPointer uptr, bool isAttacking) override =0;
		virtual const unit_map_view_charset mapView() const override =0;
		virtual abs_unit* fabricate() override;
		virtual bool checkRange(int rng) override;

	public:
		InfantryUnit(unsigned int f, std::vector<damagePointer>& w, Defences& def, frontmaps& fm, std::
			wstring nm);
	};

	class EGA_infantry : public InfantryUnit
		// Юнит пехоты EGА
	{
	protected:
		virtual bool takeDamage(unsigned int dmg) override;
		virtual int collectDamage(UnitPointer uptr, bool isAttacking) override;
		virtual const unit_map_view_charset mapView() const override;
		virtual abs_unit* fabricate() override;
	public:
		EGA_infantry(unsigned int f, std::vector<damagePointer>& w, Defences& def, frontmaps& fm, std::
			wstring nm);
	};
}
