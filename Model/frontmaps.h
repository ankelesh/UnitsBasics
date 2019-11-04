#pragma once
#ifndef OSTR_TYPE
#define OSTR_TYPE std::wstring
#define OSTR_RETURN return sout.str();
#define OSTR_CHTYPE wchar_t
#endif
#include "CubeCoords.h"

/*
Этот файл содержит карту соприкосновений для определённого юнита. Каждый юнит имеет 6 точек соприкосновения с другими
шестиугольниками на карте и у каждой такой точки есть свой уровень защищённости. Соответственно противник нападающий с 
той стороны получит штраф\бонус к урону соответствующий уровню этой точки.


*/

namespace Model
{
	using HexCoords::CubeDirections;
	enum front { vulnerable = -2, unprotected, normal, reinforced, strong};	// Уровни  
	static std::wistream& operator>>(std::wistream& sin, front & cd)	// оператор сериализации, получает значение фронта из потока
	{
		int i;
		sin >> i;
		i = (i <= strong && i >= vulnerable) ? i : normal;
		cd = (front)i;
		return sin;
	}
	typedef std::pair<CubeDirections, front> FrontOnDirection;	// уровень защищённости на определённом направлении
	class frontmaps
	{
	private:
		front currentState[6];		// Текущее состояние. По идее некоторые обстоятельства будут менять состояние фронт-карты, но я не уверен
		front defaultState[6];		// Дефолт. К нему сбрасывается состояние
		CubeDirections Head;		// точка на которую юнит "смотрит". Именно этой точкой он будет повёрнут к врагу
		CubeDirections defaultHead;	
	public:
		frontmaps();
		frontmaps(const CubeDirections, std::vector<front> &);
		frontmaps(const CubeDirections, front*);
		frontmaps(const CubeDirections, std::vector<FrontOnDirection>&);
		void operator<<(const int);
		void operator>>(const int);
		front & operator[](const CubeDirections);
		void operator++();
		void drop();
		void turn(const CubeDirections);
		OSTR_TYPE serialize();
		void deserialize(OSTR_TYPE &);
		void deserialize(std::wistream&);
		OSTR_TYPE show();
	};

}