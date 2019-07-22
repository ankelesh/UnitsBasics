#pragma once
#ifndef OSTR_TYPE
#define OSTR_TYPE std::wstring
#define OSTR_RETURN return sout.str();
#define OSTR_CHTYPE wchar_t
#endif
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>


namespace Model
{
	enum armtypes { infantry, rocket, HE, AP, energy, special };
	class DynamicDefence
	{
	private:
		int amount;
		int max;
		double effective_absorb;
		bool expendable;
	public:
		DynamicDefence();
		DynamicDefence(int, int, double, bool);
		DynamicDefence(int, double = 0.5, bool = true);
		void refill();
		int absorb(int);
		bool active();
		int& refAmount() { return amount; };
		int& refMax() { return max; };
		double& refEA() { return effective_absorb; };
		bool& refExp() { return expendable; };
		OSTR_TYPE serialize() const;
		void deserialize(OSTR_TYPE&);
		void deserialize(std::wistream&);
	};
	class Fortifications
	{
	private:
		int amount;
		int max;
		int growth;
		int one_time_damage;
		int min;
		double growth_coef;
		double growth_multiplier;
		int first_up;
		int second_up;
	public:
		Fortifications();
		Fortifications(int, int, int, int, int, double, double, int, int);
		int drop();
		int& refAmount() { return amount; };
		int& refMax() { return max; }
		int& refGrowth(){ return growth; }
		int& refOneTD(){ return one_time_damage; }
		int& refMin() {return min;	}
		int& refFirstUp() {	return first_up;}
		int& refSecondUp() {return second_up;	}
		int grow();
		int getHit(int);
		OSTR_TYPE serialize() const;
		void deserialize(OSTR_TYPE&);
		void deserialize(std::wistream&);		
	};
	class Defences
	{
	private:
		int resistances[6];
		int armours[6];
		DynamicDefence dynamics;
		Fortifications fortifications;
	public:
		Defences(); 
		Defences(const std::vector<int>&,const std::vector<int>&,const DynamicDefence &,const Fortifications &);
		Defences(std::vector<std::pair<armtypes, int>>&,
			std::vector<std::pair<armtypes, int> >&, DynamicDefence&, Fortifications &);
		void setResistanses(std::vector<int> &);
		void setArmours(std::vector<int> &);
		void setResistance(armtypes, int);
		void setArmour(armtypes, int);
		int getArmour(armtypes);
		int getResistanse(armtypes);
		DynamicDefence& getDynamic();
		Fortifications& getFortifications();
		OSTR_TYPE serialize() const;
		void deserialize(OSTR_TYPE&);
		void deserialize(std::wistream&);
		OSTR_TYPE about();
	};

}