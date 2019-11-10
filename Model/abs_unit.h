#pragma once
#include <vector>
#include "Model/Defences.h"
#include "Model/abs_damage.h"
#include "Model/frontmaps.h"
#include "Model/unit_view_charset.h"
#include <memory>
#include <iostream>


/*
	Ётот класс представл€ет собой основу дл€ всех юнитов. ≈го основные характеристики - 
	сила отр€да, защита, вооружение и карта фронтов. ѕока что тут только основные обозначени€.


*/

namespace Model {
	class abs_unit;
	typedef std::shared_ptr<abs_unit> UnitPointer;
	class abs_unit
	{
	public:
		enum UnitTypes { None, Infantry };

	protected:
		unsigned int force;
		std::vector<damagePointer> weapons;
		Defences defences;
		frontmaps frontmap;
		std::wstring name;

		virtual bool takeDamage(unsigned int dmg) = 0;
		virtual int collectDamage(UnitPointer uptr, bool isAttacking) = 0;
		virtual const unit_map_view_charset& mapView() =0;
	public:
		abs_unit();
		abs_unit(std::wstring nm, unsigned int force, std::vector<damagePointer> w, Defences d, frontmaps f);
		abs_unit(std::wistream& wsin);
		bool applyDamageToThis(unsigned int dmg);
		int countPureDamageTo(UnitPointer uptr, bool isAttacking);
		const unit_map_view_charset & getViewOfThis();

	};
}