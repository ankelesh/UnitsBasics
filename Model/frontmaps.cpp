#include "frontmaps.h"
#include <iomanip>

namespace Model {
	CubeDirections shift(CubeDirections value, int steps)
	{
		if (steps >= CubeDirections::None || steps <= (-CubeDirections::None))
			return value;
		int v = (int)value + steps;
		v = ((v) >= CubeDirections::None) ? (v - 6) : v;
		v = ((v) < 0) ? (v + 6) : v;
		return (CubeDirections)v;
	}
	OSTR_TYPE frontAsString(front fr, OSTR_CHTYPE sym)
	{
		OSTR_TYPE temp;
		for (int i = -3; i < fr; ++i)
		{
				temp += sym;
		}
			return temp;
	}
	OSTR_CHTYPE dirAsChar(CubeDirections dir)
	{
		switch (dir)
		{
		case (CubeDirections::Right):
		case (CubeDirections::Left):
			return (OSTR_CHTYPE)'|';
		case (CubeDirections::Lefttop):
		case(CubeDirections::Rigthlow):
			return (OSTR_CHTYPE)'/';
		case(CubeDirections::Leftlow):
		case(CubeDirections::Righttop):
			return (OSTR_CHTYPE)'\\';
		default:
			return (OSTR_CHTYPE)'\0';
		}

	}
	frontmaps::frontmaps()
	{
		front* start = defaultState;
		front* end = defaultState + CubeDirections::None;
		while (start != end)
		{
			*start = front::normal;
			++start;
		}
		Head = CubeDirections::Right;
		defaultHead = Head;
		drop();
	}
	frontmaps::frontmaps(const CubeDirections dir, std::vector<front> & fondvector)
	{
		int vectsize = fondvector.size();
		if (vectsize < 6)
		{
			for (int i = 0; i < vectsize; ++i)
			{
				defaultState[i] = fondvector.at(i);
				currentState[i] = fondvector.at(i);
			}
		}
		else
		{
			for (int i = 0; i < 6; ++i)
			{
				defaultState[i] = fondvector.at(i);
				currentState[i] = fondvector.at(i);
			}
		}
		Head = dir;
		defaultHead = Head;
	}

	frontmaps::frontmaps(const CubeDirections dir, front * fondarray)
	{
		for (int i = 0; i < 6; ++i)
		{
			defaultState[i] = fondarray[i];
			currentState[i] = fondarray[i];
		}
		Head = dir;
		defaultHead = Head;
	}

	frontmaps::frontmaps(const CubeDirections dir,std::vector<FrontOnDirection>& fondvector)
	{
		auto start = fondvector.begin();
		while (start != fondvector.end())
		{
			if (start->first != CubeDirections::None) {
				defaultState[start->first] = start->second;
				currentState[start->first] = start->second;
			}
			++start;
		}
		Head = dir;
		defaultHead = Head;
	}

	void frontmaps::operator<<(const int steps)
	{
		operator>>(-steps);
	}
	void frontmaps::operator>>(const int steps)
	{
		front temp[6];
		for (int i = 0; i < 6; ++i)
		{
			//std::cout <<"sh: -f "<<i <<" -a "<< shift((CubeDirections)i, steps)<<'\n';
			temp[shift((CubeDirections)i, steps)] = currentState[i];
		}
		std::copy(temp, temp + 6, currentState);
		Head = shift(Head, steps);
	}
	front & frontmaps::operator[](const CubeDirections dir)
	{
		if (dir == CubeDirections::None)
		{
			return currentState[0];
		}
		return currentState[dir];
	}
	void frontmaps::operator++()
	{
		for (int i = 0; i < 6; ++i)
		{
			currentState[i] = (currentState[i] < front::strong) ?
				(front)(currentState[i] + 1) : currentState[i];
		}
	}
	void frontmaps::drop()
	{
		std::copy(defaultState, defaultState + 6, currentState);
		Head = defaultHead;
	}
	void frontmaps::turn(const CubeDirections dir)
	{
		if (Head == dir) return;
		int steps = (Head > dir) ? Head - dir : Head + dir;
		steps = (steps > 5) ? steps - 6 : steps;
		steps = (steps < 0) ? steps + 6 : steps;
		operator>>(steps);
	}
	OSTR_TYPE frontmaps::serialize()
	{
		std::wostringstream sout;
		sout << Head << ' ' << defaultHead << ' ';
		for (int i = 0; i < 6; ++i)
		{
			sout << currentState[i] << ' ' << defaultState[i]<< ' ';
		}
		sout << " | ";
		OSTR_RETURN
	}
	void frontmaps::deserialize(OSTR_TYPE & str)
	{
		std::wistringstream sin(str);
		sin >> Head >> defaultHead;
		for (int i = 0; i < 6; ++i)
		{
			sin >> currentState[i] >> defaultState[i];
		}
	}
	void frontmaps::deserialize(std::wistream& stream)
	{
		std::wstring temp;
		std::getline(stream, temp, (OSTR_CHTYPE)'|');
		deserialize(temp);
	}
	OSTR_TYPE frontmaps::show()
	{
		std::wostringstream sout;
		sout.setf(std::ios::right, std::ios::adjustfield);
		sout << std::setw(3) << frontAsString(currentState[CubeDirections::Lefttop], (OSTR_CHTYPE)'/');
		sout.setf(std::ios::left, std::ios::adjustfield);
		sout << std::setw(3) << frontAsString(currentState[CubeDirections::Righttop], (OSTR_CHTYPE)'\\');
		sout <<(OSTR_CHTYPE)'\n';
		sout.setf(std::ios::left, std::ios::adjustfield);
		sout << std::setw(3) << frontAsString(currentState[CubeDirections::Left], (OSTR_CHTYPE)'|');
		sout.setf(std::ios::right, std::ios::adjustfield);
		sout << std::setw(3) << frontAsString(currentState[CubeDirections::Right], (OSTR_CHTYPE)'|');
		sout << (OSTR_CHTYPE)'\n';
		sout.setf(std::ios::right, std::ios::adjustfield);
		sout << std::setw(3) << frontAsString(currentState[CubeDirections::Leftlow], (OSTR_CHTYPE)'\\');
		sout.setf(std::ios::left, std::ios::adjustfield);
		sout << std::setw(3) << frontAsString(currentState[CubeDirections::Rigthlow], (OSTR_CHTYPE)'/');
		sout << (OSTR_CHTYPE)'\n';
		OSTR_RETURN
	}
}