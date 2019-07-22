#include "Defences.h"

namespace Model {
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
	{
		if (expendable)
			amount = max;
	}
	int DynamicDefence::absorb(int incoming)
	{
		if (amount == 0)
			return 0;
		int reduced_damage = (int)(incoming * effective_absorb);
		reduced_damage = (reduced_damage > amount) ? amount : reduced_damage;
		if (expendable)
		{
			amount -= reduced_damage;
		}
		return reduced_damage;
	}
	bool DynamicDefence::active()
	{
		return amount == 0;
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

	Fortifications::Fortifications()
		: amount(0), max(20), growth(4), one_time_damage(1000), min(0), growth_coef(3),
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
	{
		int levels_to_decrease = (amount >= second_up) ? 2 : (amount >= first_up) ? 1 : 0;
		amount = min;
		return levels_to_decrease;
	}

	int Fortifications::grow()
	{
		int growed = (int)std::ceil(growth * std::pow(((100 - amount * growth_multiplier)/100),growth_coef));
		int new_amount = ((amount + growed) > max) ? max : amount + growed;
		if (new_amount > amount)
		{
			amount = new_amount;
			return (amount >= second_up) ? 2 : (amount >= first_up) ? 1 : 0;
		}
		return 0;
	}

	int Fortifications::getHit(int hit)
	{
		int new_amount = amount - hit;
		int levels_to_decrease = 0;
		if (new_amount < min) new_amount = min;
		
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
	{
		if (arm.size() < 7)
		{
			for (int i = 0; i < 6; ++i)
			{
				armours[i] = arm.at(i);
			}
		}
	}

	void Defences::setResistance(armtypes at, int value)
	{
		resistances[at] = value;
	}

	void Defences::setArmour(armtypes at, int value)
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