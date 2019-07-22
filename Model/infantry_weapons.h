#pragma once
#include "abs_damage.h"

namespace Model {
	struct standard_infantry_weapon_charset
	{
		OSTR_TYPE name;
		int amount;
		bool bulky;
		int penetration;
		int min_range;
		int max_range;
	};
	OSTR_TYPE static serialize(const standard_infantry_weapon_charset& chs)
	{
		std::wostringstream sout;
		sout << chs.name << '\n' << ' ' << chs.amount << ' ' << chs.bulky << ' ' <<
			chs.penetration << ' ' << chs.min_range << ' ' << chs.max_range << ' ';
		OSTR_RETURN
	}
	standard_infantry_weapon_charset static deserialize(OSTR_TYPE& str)
	{
		std::wistringstream sin(str);
		standard_infantry_weapon_charset chs;
		std::getline(sin, chs.name);
		sin >> chs.amount >> chs.bulky >> chs.penetration >> chs.min_range >> chs.max_range;
		return chs;
	}
	standard_infantry_weapon_charset static deserialize(std::wistream& sin)
	{
		standard_infantry_weapon_charset chs;
		std::getline(sin, chs.name);
		sin >> chs.amount >> chs.bulky >> chs.penetration >> chs.min_range >> chs.max_range;
		return chs;
	}

	class EGA_SAR_2 : public abs_damage
	{
	private:
		standard_infantry_weapon_charset c;
	protected:
		OSTR_TYPE serialization();
		void deserialization(OSTR_TYPE&);
		void deserialization(std::wistream&);
		int calcnoise();	//���������� ���������� ���� ������� ���� �����. 
		//����� ������������ ��� �� ������ ����� ��� ��� ����-����
		bool canRetaliate();	//���������� ����������� ����������
		int calcdmg(const int);	//���������� amount. 
		bool canReach(const int);	//��������� ������������� 
		bool canPenetrate(Defences&);	//��������� �������������
		double calcResist(Defences&);	//���������� ������� ������������ �����
		int calcDP(Defences&);	//���������� ����� ����������� ������ �����
		abs_damage* fabricate();
	public:
		EGA_SAR_2();
		EGA_SAR_2(const standard_infantry_weapon_charset&);
	};
}