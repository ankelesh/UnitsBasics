#pragma once
#include "abs_damage.h"

namespace Model {
	using std::unique_ptr;

	struct standard_infantry_weapon_charset
	{
		// Ѕазовый набор характеристик дл€ большинства противопехотного
		OSTR_TYPE name = L"None";
		int amount = 10;
		bool bulky = false;
		int penetration = 2;
		int min_range = 1;
		int max_range = 1;
		int initiative = 1;
	};
	typedef	unique_ptr<standard_infantry_weapon_charset> inf_weapon_base;
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
				Ѕазовый класс всего противопехотного. ѕоддерживает все интерфейсы abs_damage. 
				ѕростое вооружение подчин€ющеес€ стандартным методам вычислени€ урона можно и нужно
				создавать передава€ набор характеристик вместо создани€ подкласса. —оздавайте подкласс только
				в случае если у оружи€ есть особые свойства дл€ которого не хватает стандартных методов или
				характеристик.
		*/
	protected:
		inf_weapon_base basic;
		
		virtual OSTR_TYPE serialization() override;
		virtual void deserialization(OSTR_TYPE&) override;
		virtual void deserialization(std::wistream&) override;
		virtual int calcnoise() override;	//¬озвращает количество шума данного типа урона. 
		//ћожно рассчитывать его на основе урона или ещЄ чего-либо
		virtual bool canRetaliate() override;	//¬озвращает возможность контратаки
		virtual int calcdmg(const int, const bool)override;	//возвращает amount. 
		virtual bool canReach(const int) override;	//провер€ет достигаемость 
		virtual bool canPenetrate(Defences&) override;	//провер€ет бронебойность
		virtual double calcResist(Defences&) override;	//возвращает процент поглощЄнного урона
		virtual int calcDP(Defences&) override;	//возвращает число поглощЄнных единиц урона
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
	protected:
		int sniperDamage;
		int minimumForce;
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
	protected:
		int minimumForce;
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