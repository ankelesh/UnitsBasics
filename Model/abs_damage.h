#pragma once
#ifndef OSTR_TYPE
#define OSTR_TYPE std::wstring
#define OSTR_RETURN return sout.str();
#define OSTR_CHTYPE wchar_t
#endif
#include <iostream>
#include "Defences.h"

namespace Model
{
	class abs_damage
	{
	protected:

		virtual OSTR_TYPE serialization() = 0;
		virtual void deserialization(OSTR_TYPE&) = 0;
		virtual void deserialization(std::wistream&) = 0;
		virtual int calcnoise() = 0;	//���������� ���������� ���� ������� ���� �����. 
		//����� ������������ ��� �� ������ ����� ��� ��� ����-����
		virtual bool canRetaliate() = 0;	//���������� ����������� ����������
		virtual int calcdmg(const int) = 0;	//���������� amount. 
		virtual bool canReach(const int) = 0;	//��������� ������������� 
		virtual bool canPenetrate(Defences&) = 0;	//��������� �������������
		virtual double calcResist(Defences&) = 0;	//���������� ������� ������������ �����
		virtual int calcDP(Defences&) = 0;	//���������� ����� ����������� ������ �����
		virtual abs_damage* fabricate() = 0;
	public:
		int makeNoise();
		int countDamage(Defences&, const int );
		int inRange(const int);
	};
}
