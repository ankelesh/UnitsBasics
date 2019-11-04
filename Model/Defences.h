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
/*
Этот файл содержит защитные свойства юнита. они не абстрактные, т.к. одинаковы для всех. 
Да и не представляю я как сделать оба класса в паре "урон - защита" абстрактными



*/

namespace Model
{
	enum armtypes { infantry, rocket, HE, AP, energy, special };
	/*
		Типы брони. 
		infantry: этот тип брони должен защищать от противопехотного вооружения, в том числе от газов, радиации и прочей не вредящей бронетехнике фигни
		rocket: этот тип брони защищает от кумулятивных и прочих ракетных боеприпасов. Чаще всего не очень отличается от общего уровня бронирования - но важен для 
			классификации урона
		HE:   фугасный урон. Именно его наносит взрывчатка и артиллерийские заряды. К нему нормальную броню могут иметь только укрепления, и то ограниченно
				, ну и пехота его гораздо меньше ощущает.
		AP:	бронебойный урон - наносится неуправляемыми зарядами. Обычно пехота к нему почти иммунна.
		energy: этот урон наносится в основном криогениками и ядерными взрывами. Броня к нему редко встречается
		special: сюда вносится урон который не подходит под классификации
	
	*/
	class DynamicDefence
	{
		/*
			Динамическая защита - это прослойка брони, которая конечна. В основном блокирует только ракетный урон. 
			Таким образом танки с большим запасом динамической защиты будут дольше жить против ракетного урона, но только
			пока защита не исчерпана.
		*/
	private:
		int amount;	// Количество оставшейся динамической защиты
		int max;	// максимальный запас, считается в количестве урона который он способен заблокировать
		double effective_absorb;	//	Эффективное поглощение, которое означает максимальный процент поглощённого урона за одно применение
		bool expendable;			//	некоторые юниты могут иметь неограниченную динамическую защиту
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
		/*
			Укрепления по идее должны увеличивать степень защиты по незащищённым направлениям. Т.Е. окопавшаяся пехота не будет иметь настолько уязвимого
			тыла. Укрепления разрушаются не всем уроном, и прирастают каждый ход сидения на одном месте. Рост укреплений нелинеен - наиболее эффективны первые
			несколько ходов окапывания. когда укрепления проходят определённый порог - карта фронтов пересчитывается, увеличивая все значения на 1.
		*/
	private:
		int amount; // текущее к-во укреплений
		int max;	//	максимальное к-во. Обычно 20
		int growth;	//	прирост за ход по умолчанию. Режется multiplier'ом
		int one_time_damage;	//	насколько разрушаются за одну атаку тем оружием, что повреждает укрепление
		int min;			//	минимум фортификаций
		double growth_coef;	//	применяется в рассчётах прироста
		double growth_multiplier;
		int first_up;	//	уровень фортификаций для первого пересчёта карты фронтов
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
		/*
			Это комплексный класс который отвечает за все параметры кроме карты фронтов. 
		*/
	private:
		int resistances[6];	// сопротивление режет урон %-но
		int armours[6];		// броня даёт полный иммунитет к урону если её уровень на 10 превышает бронебойность оружия. Разница между 0 и 10 режет урон процентно
							// если бронебойности хватает на 100%-ное пробитие, броня не делает ничего
		DynamicDefence dynamics;	// см описания классов
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