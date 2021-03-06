#pragma once
#ifndef OSTR_TYPE
#define OSTR_TYPE std::wstring
#define OSTR_RETURN return sout.str();
#define OSTR_CHTYPE wchar_t
#endif
#include "../../CubeCoords.h"
#include <qevent.h>

/*
���� ���� �������� ����� ��������������� ��� ������������ �����. ������ ���� ����� 6 ����� ��������������� � �������
���������������� �� ����� � � ������ ����� ����� ���� ���� ������� ������������. �������������� ��������� ���������� � 
��� ������� ������� �����\����� � ����� ��������������� ������ ���� �����.


*/

namespace Model
{
	using HexCoords::CubeDirections;
	enum front { vulnerable = -2, unprotected, normal, reinforced, strong};	// ������  
	static std::wistream& operator>>(std::wistream& sin, front & cd)	// �������� ������������, �������� �������� ������ �� ������
	{
		int i;
		sin >> i;
		i = (i <= strong && i >= vulnerable) ? i : normal;
		cd = (front)i;
		return sin;
	}
	typedef std::pair<CubeDirections, front> FrontOnDirection;	// ������� ������������ �� ����������� �����������
	class frontmaps
	{
	private:
		front currentState[6];		// ������� ���������. �� ���� ��������� �������������� ����� ������ ��������� �����-�����, �� � �� ������
		front defaultState[6];		// ������. � ���� ������������ ���������
		CubeDirections Head;		// ����� �� ������� ���� "�������". ������ ���� ������ �� ����� ������� � �����
		CubeDirections defaultHead;	
	public:
		frontmaps();
		frontmaps(const CubeDirections, std::vector<front> &);
		frontmaps(const CubeDirections, front*);
		frontmaps(const CubeDirections, std::vector<FrontOnDirection>&);
		frontmaps(const CubeDirections head, const front rF, const front rbf, const front lbf, const front lF, const front ltf, const front rtf);
		void operator<<(const int);
		void operator>>(const int);
		front & operator[](const CubeDirections);
		front& operator[](const int);
		const front* begin() const;
		const front* end() const;
		void operator++();
		void drop();
		void turn(const CubeDirections);
		unsigned int alterDamageFromDirection(const CubeDirections dir,const unsigned int damage);
		OSTR_TYPE serialize();
		void deserialize(OSTR_TYPE &);
		void deserialize(std::wistream&);
		OSTR_TYPE show();
	};

}