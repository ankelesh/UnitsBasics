#pragma once
#include "abs_damage.h"

/*
	Этот файл содержит классы противопехотного вооружения. Основные его свойства:
	оно практически никогда не шумит, в основном учитывает только Infantry-броню.


*/
namespace Model {
	using std::unique_ptr;

	struct standard_infantry_weapon_charset
	{
		// Базовый набор характеристик для большинства противопехотного оружия
		OSTR_TYPE name = L"None";	//Название
		int amount = 10;			// Количество урона которое будет изменяться модификаторами
		bool bulky = false;			// Неповоротливое оружие не стреляет в ответ
		int penetration = 2;		// Бронебойность
		int min_range = 1;			// дистанция досягаемости. 1 - соседние клетки
		int max_range = 1;
		int initiative = 1;			//инициатива
	};
	typedef	unique_ptr<standard_infantry_weapon_charset> inf_weapon_base;	//	поинтер на пехотное оружие
	//Функции сериализации
	OSTR_TYPE static serialize_charset(const standard_infantry_weapon_charset& chs, int clid)
	{
		
		std::wostringstream sout;
		sout << clid << chs.name << '\n' << ' ' << chs.amount << ' ' << chs.bulky << ' ' <<
			chs.penetration << ' ' << chs.min_range << ' ' << chs.max_range << ' ' << chs.initiative << ' ';
		OSTR_RETURN
	}
	standard_infantry_weapon_charset static deserialize_charset(OSTR_TYPE& str)
	{
		std::wistringstream sin(str);
		standard_infantry_weapon_charset chs;
		std::getline(sin, chs.name);
		sin >> chs.amount >> chs.bulky >> chs.penetration >> chs.min_range >> chs.max_range >> chs.initiative;
		return chs;
	}
	standard_infantry_weapon_charset static deserialize_charset(std::wistream& sin)
	{
		standard_infantry_weapon_charset chs;
		std::getline(sin, chs.name);
		sin >> chs.amount >> chs.bulky >> chs.penetration >> chs.min_range >> chs.max_range >> chs.initiative;
		return chs;
	}




	class InfantryWeapons : public abs_damage
	{
		/*
				Базовый класс всего противопехотного. Поддерживает все интерфейсы abs_damage. 
				Простое вооружение подчиняющееся стандартным методам вычисления урона можно и нужно
				создавать передавая набор характеристик вместо создания подкласса. Создавайте подкласс только
				в случае если у оружия есть особые свойства для которого не хватает стандартных методов или
				характеристик.
		*/
	protected:
		inf_weapon_base basic;		// impl - указатель. См. шаблон проектирования impl\pimpl
		
		virtual OSTR_TYPE serialization() override;
		virtual void deserialization(OSTR_TYPE&) override;
		virtual void deserialization(std::wistream&) override;
		virtual int calcnoise() override;	//Возвращает количество шума данного типа урона. 
		//Можно рассчитывать его на основе урона или ещё чего-либо
		virtual bool canRetaliate() override;	//Возвращает возможность контратаки
		virtual int calcdmg(const int, const bool)override;	//возвращает amount. 
		virtual bool canReach(const int) override;	//проверяет достигаемость 
		virtual bool canPenetrate(Defences&) override;	//проверяет бронебойность
		virtual double calcResist(Defences&) override;	//возвращает процент поглощённого урона
		virtual int calcDP(Defences&) override;	//возвращает число поглощённых единиц урона
		virtual int damageFortifications(Defences&, const bool) override; 
		virtual abs_damage* fabricate()override;
		virtual int calcIni(const bool) override;
	public:
		const int class_id = 1;
		InfantryWeapons();
		InfantryWeapons(const inf_weapon_base&);
		InfantryWeapons(const standard_infantry_weapon_charset&);
	};


	class SniperWeapons : public InfantryWeapons
	{
		/*
			Снайперское оружие всегда наносит дополнительный урон если сила отряда не меньше границы
		*/
	protected:
		int sniperDamage;	//	количество нанесённого урона
		int minimumForce;	//	минимальная сила отряда которая сможет дать снайперам нанести урон
		abs_damage* fabricate() override;

		virtual OSTR_TYPE serialization() override;
		virtual void deserialization(OSTR_TYPE&) override;
		virtual void deserialization(std::wistream&) override;
		virtual int calcdmg(const int, const bool)override;	//возвращает amount. 
	public:
		const int class_id = 2;	
		SniperWeapons();
		SniperWeapons(const standard_infantry_weapon_charset&, int sD, int mF);
		SniperWeapons(const inf_weapon_base& b, int sD, int mF) :
			InfantryWeapons(b), sniperDamage(sD), minimumForce(mF) {};
	};

	class SubmachWeapons : public InfantryWeapons
	{
		/*
			Штурмовые ПП стреляют раньше если отряд атакует
		*/
	protected:
		virtual int calcIni(const bool) override;
		abs_damage* fabricate() override;
		virtual OSTR_TYPE serialization() override;
		virtual void deserialization(OSTR_TYPE&) override;
		virtual void deserialization(std::wistream&) override;
	public:
		const int class_id = 3;
		SubmachWeapons() : InfantryWeapons() {};
		SubmachWeapons(const standard_infantry_weapon_charset& b) : InfantryWeapons(b) {};
		SubmachWeapons(const inf_weapon_base& b) : InfantryWeapons(b) {};
	};
	class LightMachWeapons : public InfantryWeapons
	{
		/*
			Для эффективного использования лёгких пулемётов не нужно иметь полный отряд
		*/
	protected:
		int minimumForce;	// граница после которой урон начнёт падать
		virtual int calcdmg(const int, const bool) override;
		abs_damage* fabricate() override;
		virtual OSTR_TYPE serialization() override;
		virtual void deserialization(OSTR_TYPE&) override;
		virtual void deserialization(std::wistream&) override;
	public:
		const int class_id = 4;
		LightMachWeapons() : InfantryWeapons(), minimumForce(0) {};
		LightMachWeapons(const standard_infantry_weapon_charset& b, int mF) : InfantryWeapons(b), minimumForce(mF) {};
		LightMachWeapons(const inf_weapon_base& b, int mF) : InfantryWeapons(b), minimumForce(mF) {};
	};
	class HeavyMachWeapons : public InfantryWeapons
	{
		/*
			Тяжёлые станковые пулемёты обладают низкой инициативой при атаке и очень высокой - при обороне 
		*/
	protected:
		virtual int calcIni(const bool) override;
		abs_damage* fabricate() override;
	public:
		const int class_id = 5;
		HeavyMachWeapons() : InfantryWeapons() {};
		HeavyMachWeapons(const standard_infantry_weapon_charset& b) : InfantryWeapons(b) {};
		HeavyMachWeapons(const inf_weapon_base& b) : InfantryWeapons(b) {};
	};
	class RockInfantryWeapons : public InfantryWeapons
	{
		/*
			Это базовый класс вооружения Рокской империи. В силу особенностей роков их оружие гораздо 
			более бронебойное, но менее инициативное и очень шумное.
			Данный класс выбирает из значений противопехотной и противобронебойной защит минимальное.		
		*/
	protected:
		virtual int calcnoise() override;	
		virtual double calcResist(Defences&) override;	
		virtual abs_damage* fabricate()override;
		virtual bool canPenetrate(Defences&) override;
	public:
		const int class_id = 6;
		RockInfantryWeapons() : InfantryWeapons() {};
		RockInfantryWeapons(const standard_infantry_weapon_charset& b) : InfantryWeapons(b) {};
		RockInfantryWeapons(const inf_weapon_base& b) : InfantryWeapons(b) {};
	};
	class RockSniperWeapons : public SniperWeapons
	{
		/*
			Копия снайперского оружия, но с рокскими особенностями
		*/
	protected:
		virtual int calcnoise() override;
		virtual double calcResist(Defences&);
		virtual abs_damage* fabricate()override;
		virtual bool canPenetrate(Defences&) override;
	public:
		const int class_id = 7;
		RockSniperWeapons() : SniperWeapons() {};
		RockSniperWeapons(const standard_infantry_weapon_charset& b, int sd, int mf) : SniperWeapons(b, sd, mf) {};
		RockSniperWeapons(const inf_weapon_base& b, int sd, int mf) : SniperWeapons(b, sd, mf) {};
	};
	class RockHeavyMachWeapons : public RockInfantryWeapons
	{
	protected:
		
	};
}