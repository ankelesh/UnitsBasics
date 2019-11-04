#pragma once
#include <vector>
#include "Model/Defences.h"
#include "Model/abs_damage.h"
#include "Model/frontmaps.h"
#include <memory>
#include <iostream>


/*
	���� ����� ������������ ����� ������ ��� ���� ������. ��� �������� �������������� - 
	���� ������, ������, ���������� � ����� �������.


*/

namespace Model {
	class abs_unit
	{
	protected:
		unsigned int force;
		std::vector<damagePointer> weapons;
		Defences defences;
		frontmaps frontmap;
		std::wstring name;

		virtual void takeDamage(unsigned int dmg) = 0;
		void 
	public:
		abs_unit();
		abs_unit(std::wstring nm, unsigned int force, std::vector<damagePointer> w, Defences d, frontmaps f);
		abs_unit(std::wistream& wsin);


	};
}