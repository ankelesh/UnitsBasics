#pragma once
#ifndef OSTR_TYPE
#define OSTR_TYPE std::wstring
#define OSTR_RETURN return sout.str();
#define OSTR_CHTYPE wchar_t
#endif
#include <iostream>
#include <memory>
#include "Defences.h"

namespace Model
{
	const int MODEL_ABSTRACT_CLASS_ID = 1;	// �� ����� ����� ��� ��������� ���
	class abs_damage
	{
		/*
			������� ����� ��� ���� ����� �����. ��������� ������� : ��� ������� ������� �����������
			�� ������ ����� inRange(), ����� ������������ countDamage() � makeNoise(). 
			getInitiative() -> ��� ������������� PriorityQueue;
			
			� ������� ����� ������ ���� ��������� ����� ����������� ����� ������� ����������� �� �������,
			������������� �� ��������� ����������. ������ ��� ����� ��� ������ ��� ������������
			�������� ���������.

			������ ��� ����� ����� ��������� ���������� ���, ������� ���������� �� ����� ������������� 
			��������. 

			������ ��� ����� ����� �������� ���� �� �������������, ��� ���� �� �� ����� ��������� ����������� 
			����� ����������.

			��. ������ - ������ ��� ����� ����� �������� �������������, ������� ������ �� ��, ����� �� 
			���� ���� ������. �� ��� ������������ ������ - ������ ��� ��� �����. � ������� ������� ��������
			������ ��������� ��� ���������������, ��� � HE-�����.

			��� �� �����, ���� ���������� ������ ������� � ��������� ���������� ��������� �������
		*/
	protected:
		const int class_id = 0; // ������������� ������ ����� ��� ���� ����� ��� �������������� ������ ����� ��� ����� �������� � ������

		virtual OSTR_TYPE serialization() = 0;
		virtual void deserialization(OSTR_TYPE&) = 0;
		virtual void deserialization(std::wistream&) = 0;
		virtual int calcnoise() = 0;	//���������� ���������� ���� ������� ���� �����. 
		//����� ������������ ��� �� ������ ����� ��� ��� ����-����
		virtual bool canRetaliate() = 0;	//���������� ����������� ����������
		virtual int calcdmg(const int, const bool) = 0;	//���������� amount. 
		virtual bool canReach(const int) = 0;	//��������� ������������� 
		virtual bool canPenetrate(Defences&) = 0;	//��������� �������������
		virtual double calcResist(Defences&) = 0;	//���������� ������� ������������ �����
		virtual int calcDP(Defences&) = 0;	//���������� ����� ����������� ������ �����
		virtual int damageFortifications(Defences&, const bool) = 0; // ���������� ���� �� �������������
		virtual int calcIni(const bool) = 0;		//	���������� ����������
		virtual abs_damage* fabricate() = 0;
	public:
		// ���������� ���������� ������������� ����
		int makeNoise();
		// ���������� ���������� ���������� �����
		int countDamage(Defences&, const int, const bool );		//  ��������� �������, ��� ������� �������������� ������ ����� ����
		// ���������� true ���� ���� ����� ���� ������ �� ����� ����������
		int inRange(const int);
		// ���������� ���������� ����� ���� �����
		int getInitiative(const bool);
		// ���������� �������� ���������� ����� �� �����������
		int countDamageToForts(Defences&, const bool);
		//����������� ������ ��� ����������� �� ��� ������
		OSTR_TYPE serialize();
		// ��������� ������
		abs_damage* clone();
	};


	typedef std::shared_ptr<abs_damage> damagePointer;	// ���� ������� ������������ ��� ������������ �������� �����
}
