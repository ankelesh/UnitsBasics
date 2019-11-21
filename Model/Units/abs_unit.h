#pragma once
#include <vector>
#include "Defences/Defences.h"
#include "Damage/abs_damage.h"
#include "Defences/frontmaps.h"
#include "Charsets/unit_view_charset.h"
#include <memory>


/*
	���� ����� ������������ ����� ������ ��� ���� ������. ��� �������� �������������� - 
	���� ������, ������, ���������� � ����� �������. ���� ��� ��� ������ �������� �����������.
	����:
		��������� � impl ��� ������ ��� ��������� �����������

*/

namespace Model {
	class abs_unit;
	typedef std::shared_ptr<abs_unit> UnitPointer;
	class abs_unit
	{
	public:
		enum UnitTypes { None, Infantry };
	protected:
		unsigned int force;				// ����. ����������� �������� - 100
		std::vector<damagePointer> weapons;	// ������ � �������
		Defences defences;					// ������
		frontmaps frontmap;					// ����������
		std::wstring name;					// ���
		int belongsToPlayer;				// ��������������

		virtual bool takeDamage(unsigned int dmg) = 0;		// ���������������� ����� ��������� �����, ���������� true ���� ���� ����� �� ��������
		virtual int collectDamage(UnitPointer uptr, bool isAttacking) = 0;	//	��� �������� ����� �������� �����
		virtual const unit_map_view_charset mapView() const  =0;			// ���������� ��� �����
		virtual abs_unit* fabricate() = 0;									// �����������
		virtual bool checkRange(int rng) = 0;								// ���������, ����� �� ������� ���� ����. ��� ����������� ����������� ����� ����� 
																			// ����� ���������� ��� �������� ��� ������ ������
	public:
		abs_unit();
		abs_unit(std::wstring nm, unsigned int force, std::vector<damagePointer> w, Defences d, frontmaps f);
		abs_unit(std::wistream& wsin);
		bool applyDamageToThis(unsigned int dmg);		// �������� takeDamage
		int countPureDamageTo(UnitPointer uptr, bool isAttacking);
		std::vector<damagePointer>& getWeaponsList();	// �������
		Defences& getDefences();
		int belongsTo();
		unsigned int myForce();
		frontmaps& getFrontmap();
		const unit_map_view_charset getViewOfThis() const;
		const std::wstring& getName();
		bool canAttackOnRange(int range);				// �������� checkRange
		abs_unit* clone(int team = -1);
	};
	class generateName
		// ������� ��������� ������������� �������� �����
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