#include "frontmaps.h"
#include <iomanip>
#include "debugtrace.h"
namespace Model {


	CubeDirections shift(CubeDirections value, int steps)
		// Сдвигает направление вправо
	{
		if (steps >= CubeDirections::None || steps <= (-CubeDirections::None))
			return value;
		int v = (int)value + steps;
		v = ((v) >= CubeDirections::None) ? (v - 6) : v;
		v = ((v) < 0) ? (v + 6) : v;
		return (CubeDirections)v;
	}
	OSTR_TYPE frontAsString(front fr, OSTR_CHTYPE sym)
		// отладочная конверсия для псевдографики
	{
		OSTR_TYPE temp;
		for (int i = -3; i < fr; ++i)
		{
				temp += sym;
		}
			return temp;
	}
	OSTR_CHTYPE dirAsChar(CubeDirections dir)
		// отладочная конверсия для псевдографики
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
		// дефолтный конструктор
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
	// конструктор из вектора
	{
		size_t vectsize = fondvector.size();
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
	// конструктор из массива
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
	// Конструктор из фронтов-на-направлении
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

	frontmaps::frontmaps(const CubeDirections head, const front rF, const front rbf, const front lbf, const front lF, const front ltf, const front rtf)
	{
		defaultState[0] = rF;
		defaultState[1] = rbf;
		defaultState[2] = lbf;
		defaultState[3] = lF;
		defaultState[4] = ltf;
		defaultState[5] = rtf;
		std::copy(defaultState, defaultState + 6, currentState);
		Head = head;
		defaultHead = head;
	}

	void frontmaps::operator<<(const int steps)
		// операторы сдвига вправо\влево
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
		// оператор доступа в стиле массивов
	{
		if (dir == CubeDirections::None)
		{
			return currentState[0];
		}
		int steps = Head + dir;
		detrace_METHEXPL("NEED to make " << steps << " steps");
		steps = (steps > 5) ? steps - 6 : steps;
		steps = (steps < 0) ? steps + 6 : steps;
		return currentState[steps];
	}
	front& frontmaps::operator[](const int ind)
	{
		if (ind < 0 || ind > 5)
		{
			return currentState[0];
		}
		return currentState[ind];
	}
	const front * frontmaps::begin() const
	{
		return currentState;
	}
	const front* frontmaps::end() const
	{
		return currentState + 6;
	}
	void frontmaps::operator++()
		// переводит карту фронтов на следущий уровень
	{
		for (int i = 0; i < 6; ++i)
		{
			currentState[i] = (currentState[i] < front::strong) ?
				(front)(currentState[i] + 1) : currentState[i];
		}
	}
	void frontmaps::drop()
		// сбрасывает карту фронтов
	{
		std::copy(defaultState, defaultState + 6, currentState);
		Head = defaultHead;
	}
	void frontmaps::turn(const CubeDirections dir)
		// поворачивает карту в нужном направлении
	{
		detrace_METHEXPL("turning to" << HexCoords::directionToString(dir));
		if (Head == dir) return;
		int steps = (Head > dir) ? Head - dir : Head + dir;
		detrace_METHEXPL("NEED to make " << steps << " steps");
		steps = (steps > 5) ? steps - 6 : steps;
		steps = (steps < 0) ? steps + 6 : steps;
		steps = (Head > dir) ? -steps : steps;
		detrace_METHEXPL("after adjusting left: " << steps << " steps");
		operator>>(steps);
	}
	unsigned int frontmaps::alterDamageFromDirection(const CubeDirections dir, const unsigned int damage)
	{
		detrace_METHEXPL("cutting damage from " << dir << " equal to " << damage);
		detrace_METHEXPL(serialize());
		detrace_METHEXPL("front on that dir: " << frontAsString(currentState[dir], L'|'));
		switch (operator[](dir))
		{
		case strong:
			return damage / 2;
		case reinforced:
			return damage + damage / 4;
		case unprotected:
			return damage * 1.5;
		case vulnerable:
			return damage * 2;
		case normal:
		default:
			return damage;
		}
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
		// отладочный метод показывающий карту
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