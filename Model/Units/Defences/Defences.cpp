#include "Defences.h"

namespace Model {
	// конструкторы динамической защиты
	DynamicDefence::DynamicDefence() : amount(10), max(10), effective_absorb(0.5), expendable(true)
	{
	}
	DynamicDefence::DynamicDefence(int am, int ma, double ea, bool ex) 
		: amount(am), max(ma), effective_absorb(ea), expendable(ex)
	{
	}
	DynamicDefence::DynamicDefence(int ma, double ea, bool ex):
		amount(ma), max(ma), effective_absorb(ea), expendable(ex)
	{
	}
	void DynamicDefence::refill()
		// перезаряжает динамическую защиту
	{
		if (expendable)
			amount = max;
	}
	int DynamicDefence::absorb(int incoming)
		// поглощает входящий урон
	{
		if (amount == 0)	// нечего поглощать - экономим время
			return 0;
		int reduced_damage = (int)(incoming * effective_absorb);	// высчитывается максимальное значение снижения с округлением вниз
		reduced_damage = (reduced_damage > amount) ? amount : reduced_damage;	// Если количества не хватает на такое снижение - то уменьшаем снижение до нужного уровня
		if (expendable)			// если можно потратить - тратим
		{
			amount -= reduced_damage;
		}
		return reduced_damage;
	}
	bool DynamicDefence::active()
	{
		return amount == 0;		// проверка на возможность поглощения, экономит время
	}
	OSTR_TYPE DynamicDefence::serialize() const
	{
		std::wostringstream sout;
		sout << amount << ' ' << max << ' ' << effective_absorb << ' ' << expendable << " ";
		OSTR_RETURN
	}
	void DynamicDefence::deserialize(OSTR_TYPE& str)
	{
		std::wistringstream sin(str);
		sin >> amount >> max >> effective_absorb >> expendable;
	}
	void DynamicDefence::deserialize(std::wistream& stream)
	{
		stream >> amount >> max >> effective_absorb >> expendable;
	}
	// конструкторы укреплений
	Fortifications::Fortifications()
		: amount(0), max(25), growth(4), one_time_damage(1000), min(0), growth_coef(3),
		growth_multiplier(4), first_up(10), second_up(20)
	{
	}

	Fortifications::Fortifications(int Amount, int Max, int Growth, int OneTimeDmg, 
		int Min, double growthCoef, double growthMult, int fiUp, int seUp)
		: amount(Amount), max(Max), growth(Growth), one_time_damage(OneTimeDmg),
		min(Min), growth_coef(growthCoef), growth_multiplier(growthMult), 
		first_up(fiUp), second_up(seUp)
	{
	}

	int Fortifications::drop()
		// возвращает количество уровней для понижения карты фронтов и устанавливает количество в минимум
	{
		int levels_to_decrease = (amount >= second_up) ? 2 : (amount >= first_up) ? 1 : 0;
		amount = min;
		return levels_to_decrease;
	}

	int Fortifications::grow()
		// рост фортификаций, возвращает количество уровней для повышения карты фронтов С НУЛЯ
	{
		// формула вычисления будущего роста - округление к верху основанное на степенях дробей. В целом чем выше накопленное количество - тем меньше оно будет возрастать
		int growed = (int)std::ceil(growth * std::pow(((100 - amount * growth_multiplier)/100),growth_coef));
		int new_amount = ((amount + growed) > max) ? max : amount + growed;		// смотрим чтобы не вылететь за максимум
		if (new_amount > amount)												// если реального прироста не было - экономим время
		{	
			amount = new_amount;
			return (amount >= second_up) ? 2 : (amount >= first_up) ? 1 : 0;	// важно! рост карты фронтов возвращается принимая за точку отправления нулевую карту фронтов
		}
		return 0;
	}

	int Fortifications::getHit(int hit)
		// получает урон по накопленному количеству и возвращает количество оставшихся уровней начиная С НУЛЯ
	{
		int new_amount = amount - hit;
		if (new_amount < min) new_amount = min;		// смотрим чтобы не вылететь за минимум
		
		if (amount > second_up && new_amount < second_up)
		{
			amount = new_amount;
			if (new_amount < first_up)
				return 2;
			else 
				return 1;
		}
		else if (amount > first_up && new_amount < first_up)
		{
			amount = new_amount;
			return 1;
		}
		amount = new_amount;
		return 0;
		
	}

	OSTR_TYPE Fortifications::serialize() const
	{
		std::wostringstream sout;
		sout << amount << ' ' << max << ' ' << growth << ' ' << one_time_damage << ' '
			<< min << ' ' << growth_coef << ' ' << growth_multiplier << ' ' << first_up <<
			' ' << second_up << ' ';
		OSTR_RETURN
	}

	void Fortifications::deserialize(OSTR_TYPE& str)
	{
		std::wistringstream sin(str);
		sin >> amount >> max >> growth >> one_time_damage >> min >> growth_coef >> growth_multiplier
			>> first_up >> second_up;
	}

	void Fortifications::deserialize(std::wistream& sin)
	{
		sin >> amount >> max >> growth >> one_time_damage >> min >> growth_coef >> growth_multiplier
			>> first_up >> second_up;
	}
	// конструкторы защит
	Defences::Defences() : resistances(), armours(), dynamics()
	{
	}

	Defences::Defences(const std::vector<int>& res,const std::vector<int>& arm,const DynamicDefence & dyn
	, const Fortifications & fort)
		: resistances(), armours(), dynamics(dyn),fortifications(fort)
	{
		if (res.size() < 7)
		{
			for (unsigned int i = 0; i < res.size(); ++i)
			{
				resistances[i] = res.at(i);
			}
		}
		if (arm.size() < 7)
		{
			for (unsigned int i = 0; i < arm.size(); ++i)
			{
				armours[i] = arm.at(i);
			}
		}
	}

	Defences::Defences(std::vector<std::pair<armtypes, int>> & res, 
		std::vector<std::pair<armtypes, int> >& arm, DynamicDefence & dyn, 
		Fortifications & fort) 
		: resistances(), armours(), dynamics(dyn), fortifications(fort)
	{
		auto start = res.begin();
		while (start != res.end())
		{
			resistances[start->first] = start->second;
			++start;
		}
		start = arm.begin();
		while (start != arm.end())
		{
			armours[start->first] = start->second;
			++start;
		}
	}

	void Defences::setResistanses(std::vector<int>& res)
		// выставляет резисты
	{
		if (res.size() < 7)
		{
			for (int i = 0; i < 6; ++i)
			{
				resistances[i] = res.at(i);
			}
		}
	}

	void Defences::setArmours(std::vector<int>& arm)
		// выставляет броню
	{
		if (arm.size() < 7)
		{
			for (int i = 0; i < 6; ++i)
			{
				armours[i] = arm.at(i);
			}
		}
	}
	//геттеры - сеттеры
	void Defences::setResistance(armtypes at, int value)
		// выставляет конкретный резист
	{
		resistances[at] = value;
	}

	void Defences::setArmour(armtypes at, int value)
		// выставляет конкретный параметр брони
	{
		armours[at] = value;
	}
	
	int Defences::getArmour(armtypes at)
	{
		return armours[at];
	}

	int Defences::getResistanse(armtypes at)
	{
		return resistances[at];
	}

	DynamicDefence& Defences::getDynamic()
	{
		return dynamics;
	}

	Fortifications& Defences::getFortifications()
	{
		return fortifications;
	}

	OSTR_TYPE Defences::serialize() const
	{
		std::wostringstream sout;
		for (int i = 0; i < 6; ++i)
		{
			sout << resistances[i] << ' ' << armours[i] << ' ';
		}
		sout << dynamics.serialize() << ' ' << fortifications.serialize();
		sout << " | ";
		OSTR_RETURN
	}

	void Defences::deserialize(OSTR_TYPE& str)
	{
		std::wistringstream sin(str);
		for (int i = 0; i < 6; ++i)
		{
			if (sin.eof()) break;
			sin >> resistances[i] >> armours[i];
		}
		dynamics.deserialize(sin);
		fortifications.deserialize(sin);
	}

	void Defences::deserialize(std::wistream& stream)
	{
		OSTR_TYPE temp;
		std::getline(stream, temp, (OSTR_CHTYPE)'|');
		deserialize(temp);
	}

	OSTR_TYPE Defences::about()
		// дебаг - описание содержимого класса
	{
		std::wostringstream sout;
		sout << "This is defence object:\nResistances: ";
		for (int i = 0; i < 6; ++i)
		{
			sout << resistances[i] << ' ';
		}
		sout << "\nArmours: ";
		for (int i = 0; i < 6; ++i)
		{
			sout << armours[i] << ' ';
		}
		sout << "\nDynamic defences: " << dynamics.refAmount() << " amount, "
			<< dynamics.refEA() << " effective absorb, " << dynamics.refMax() <<
			" max, " << dynamics.refExp() << " expendability\n";
		sout << "Fortifications: " << fortifications.refAmount() << " amount, " <<
			fortifications.refMin() << " min, " << fortifications.refMax() << " max, "
			<< fortifications.refGrowth() << " growth, " << fortifications.refOneTD() <<
			" damage restrictions, " << fortifications.refFirstUp() << " for first upgrade "
			<< fortifications.refSecondUp() << " for second upgrade.\n";
		OSTR_RETURN
	}
	
	

}