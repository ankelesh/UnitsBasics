#pragma once
#include "abs_unit.h"



namespace Model {
	class InfantryUnit : public abs_unit
	{
	protected:
		virtual bool takeDamage(unsigned int dmg) override =0;
		virtual int collectDamage(UnitPointer uptr, bool isAttacking) override =0;
		virtual const unit_map_view_charset mapView() const override =0;
	public:
		InfantryUnit(unsigned int f, std::vector<damagePointer>& w, Defences& def, frontmaps& fm, std::
			wstring nm);



		// Inherited via abs_unit
		virtual abs_unit* fabricate() override;

		virtual bool checkRange(int rng) override;

	};

	class EGA_infantry : public InfantryUnit
	{
	public:
		EGA_infantry(unsigned int f, std::vector<damagePointer>& w, Defences& def, frontmaps& fm, std::
			wstring nm);
		virtual bool takeDamage(unsigned int dmg) override;
		virtual int collectDamage(UnitPointer uptr, bool isAttacking) override;
		virtual const unit_map_view_charset mapView() const override;

		// Inherited via InfantryUnit
		virtual abs_unit* fabricate() override;
	};
}
