#pragma once
#include <vector>
#include "Defences/Defences.h"
#include "Damage/abs_damage.h"
#include "Defences/frontmaps.h"
#include "Charsets/unit_view_charset.h"
#include <memory>


/*
	Этот класс представляет собой основу для всех юнитов. Его основные характеристики - 
	сила отряда, защита, вооружение и карта фронтов. Пока что тут только основные обозначения.
	Надо:
		перенести в impl все данные для ускорения копирования

*/

namespace Model {
	class abs_unit;
	typedef std::shared_ptr<abs_unit> UnitPointer;
	class abs_unit
	{
	public:
		enum UnitTypes { None, Infantry };
	protected:
		unsigned int force;				// Сила. Стандартное значение - 100
		std::vector<damagePointer> weapons;	// оружие в векторе
		Defences defences;					// защита
		frontmaps frontmap;					// фронткарта
		std::wstring name;					// имя
		int belongsToPlayer;				// принадлежность

		virtual bool takeDamage(unsigned int dmg) = 0;		// абстрагированный метод получения урона, возврашает true если юнит урона не выдержал
		virtual int collectDamage(UnitPointer uptr, bool isAttacking) = 0;	//	уже ненужный метод подсчёта урона
		virtual const unit_map_view_charset mapView() const  =0;			// возвращает вид юнита
		virtual abs_unit* fabricate() = 0;									// копирование
		virtual bool checkRange(int rng) = 0;								// проверяет, может ли достать этот юнит. Для оптимизации определённые юниты могут 
																			// сразу возвращать это значение без обхода оружия
	public:
		abs_unit();
		abs_unit(std::wstring nm, unsigned int force, std::vector<damagePointer> w, Defences d, frontmaps f);
		abs_unit(std::wistream& wsin);
		bool applyDamageToThis(unsigned int dmg);		// вызывает takeDamage
		int countPureDamageTo(UnitPointer uptr, bool isAttacking);
		std::vector<damagePointer>& getWeaponsList();	// геттеры
		Defences& getDefences();
		int belongsTo();
		unsigned int myForce();
		frontmaps& getFrontmap();
		const unit_map_view_charset getViewOfThis() const;
		const std::wstring& getName();
		bool canAttackOnRange(int range);				// вызывает checkRange
		abs_unit* clone(int team = -1);
	};
	class generateName
		// функтор создающий упорядоченное название юнита
	{
	public:
		static int counter;

		generateName() {};
		std::wstring operator()()
		{
			++counter;
			return std::wstring(L"squad ") + std::to_wstring(counter);
		}

	};
}