#pragma once
#ifndef OSTR_TYPE
#define OSTR_TYPE std::wstring
#define OSTR_RETURN return sout.str();
#define OSTR_CHTYPE wchar_t
#endif
#include "CubeCoords.h"


namespace Model
{
	using HexCoords::CubeDirections;
	enum front { vulnerable = -2, unprotected, normal, reinforced, strong }; 
	static std::wistream& operator>>(std::wistream& sin, front & cd)
	{
		int i;
		sin >> i;
		i = (i <= strong && i >= vulnerable) ? i : normal;
		cd = (front)i;
		return sin;
	}
	typedef std::pair<CubeDirections, front> FrontOnDirection;
	class frontmaps
	{
	private:
		front currentState[6];
		front defaultState[6];
		CubeDirections Head;
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