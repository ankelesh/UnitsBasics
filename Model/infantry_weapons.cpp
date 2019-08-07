#include "infantry_weapons.h"

namespace Model {


	OSTR_TYPE  InfantryWeapons::serialization()
	{
		return serialize_charset(*basic, class_id);
	}

	void  InfantryWeapons::deserialization(OSTR_TYPE& str)
	{
		*basic = deserialize_charset(str);
	}


	void  InfantryWeapons::deserialization(std::wistream& stream)
	{
		*basic = deserialize_charset(stream);
	}

	int  InfantryWeapons::calcDP(Defences& def)
	{
		return 0;
	}
	int  InfantryWeapons::calcnoise()
	{
		return 0;
	}
	bool  InfantryWeapons::canRetaliate()
	{
		return basic->bulky;
	}
	int  InfantryWeapons::calcdmg(const int force, const bool isAttacking)
	{
		return (int)((double)basic->amount * ((double)force / 10));
	}
	bool  InfantryWeapons::canReach(const int rng)
	{
		return basic->max_range >= rng;
	}
	bool  InfantryWeapons::canPenetrate(Defences& def)
	{
		return (basic->penetration >= def.getArmour(infantry)-1);
	}

	double  InfantryWeapons::calcResist(Defences& def)
	{
		double percent = ((double)def.getResistanse(infantry)) / 100 + (
			(def.getArmour(infantry) > basic->penetration) ? 0.1 : 0);
		if (percent > 1) percent = 1;
		return percent;
	}
	int  InfantryWeapons::damageFortifications(Defences& def, const bool isAttacking)
	{
		return 0;
	}
	abs_damage* InfantryWeapons::fabricate()
	{
		return new InfantryWeapons(basic);
	}

	int InfantryWeapons::calcIni( const bool isAttacking)
	{
		return basic->initiative;
	}

	InfantryWeapons::InfantryWeapons()
		: basic(inf_weapon_base(new standard_infantry_weapon_charset))
	{
	}

	InfantryWeapons::InfantryWeapons(const inf_weapon_base& b)
		: basic(inf_weapon_base(new standard_infantry_weapon_charset(*b)))
	{

	}

	InfantryWeapons::InfantryWeapons(const standard_infantry_weapon_charset& b)
		: basic(inf_weapon_base(new standard_infantry_weapon_charset(b)))
	{
	}

	abs_damage* SniperWeapons::fabricate()
	{
		return new SniperWeapons(*basic, sniperDamage, minimumForce);
	}

	OSTR_TYPE SniperWeapons::serialization()
	{
		std::wostringstream wsout(InfantryWeapons::serialization());
		wsout << sniperDamage << ' ' << minimumForce << ' ';
		return wsout.str();
	}

	void SniperWeapons::deserialization(OSTR_TYPE& str)
	{
		std::wistringstream wsin(str);
		InfantryWeapons::deserialization(wsin);
		wsin >> sniperDamage >> minimumForce;
	}

	void SniperWeapons::deserialization(std::wistream& stream)
	{
		InfantryWeapons::deserialization(stream);
		stream >> sniperDamage >> minimumForce;
	}

	int SniperWeapons::calcdmg(const int force, const bool isAttacking)
	{
		if (minimumForce >= force)
		{
			return InfantryWeapons::calcdmg(force, isAttacking) + 1;
		}
		return InfantryWeapons::calcdmg(force, isAttacking);
	}

	SniperWeapons::SniperWeapons()
		:InfantryWeapons(), sniperDamage(0), minimumForce(0)
	{

	}

	SniperWeapons::SniperWeapons(const standard_infantry_weapon_charset& chs, int sD, int mF)
		: InfantryWeapons(chs), sniperDamage(sD), minimumForce(mF)
	{}
	int SubmachWeapons::calcIni(const bool isAttacking)
	{
		if (isAttacking)
			return basic->initiative + 2;
		return basic->initiative;
	}
	abs_damage* SubmachWeapons::fabricate()
	{
		return new SubmachWeapons(basic);
	}
	OSTR_TYPE SubmachWeapons::serialization()
	{
		return InfantryWeapons::serialization();
	}
	void SubmachWeapons::deserialization(OSTR_TYPE& str)
	{
		InfantryWeapons::deserialization(str);
	}
	void SubmachWeapons::deserialization(std::wistream& stream)
	{
		InfantryWeapons::deserialization(stream);
	}
	int LightMachWeapons::calcdmg(const int force, const bool isAttacking)
	{
		if (force >= minimumForce)
			return basic->amount;
		return (int)((double)basic->amount * ((double)force / 10));
	}
	abs_damage* LightMachWeapons::fabricate()
	{
		return new LightMachWeapons(basic, minimumForce);
	}
	OSTR_TYPE LightMachWeapons::serialization()
	{
		std::wostringstream wsout(InfantryWeapons::serialization());
		wsout << minimumForce << ' ';
		return wsout.str();
	}
	void LightMachWeapons::deserialization(OSTR_TYPE& str)
	{
		std::wistringstream wsin(str);
		InfantryWeapons::deserialization(wsin);
		wsin >> minimumForce;
	}
	void LightMachWeapons::deserialization(std::wistream& stream)
	{
		InfantryWeapons::deserialization(stream);
		stream >> minimumForce;
	}
	int HeavyMachWeapons::calcIni(const bool isAttacking)
	{
		if (isAttacking) {
			return basic->initiative;
		}
		else {
			return basic->initiative + 7;
		}
	}
	abs_damage* HeavyMachWeapons::fabricate()
	{
		return new HeavyMachWeapons(*basic);
	}
	int RockInfantryWeapons::calcnoise()
	{
		return 1;
	}
	double RockInfantryWeapons::calcResist(Defences& def)
	{
		double Resist = (def.getResistanse(infantry) > def.getResistanse(AP)) ?
			def.getResistanse(AP) : def.getResistanse(infantry);
		int Armour = (def.getArmour(infantry) > def.getArmour(AP)) ?
			def.getArmour(AP) : def.getArmour(infantry);
		double percent = (Resist) / 100 + (
			(Armour > basic->penetration) ? 0.1 : 0);
		if (percent > 1) percent = 1;
		return percent;
	}
	abs_damage* RockInfantryWeapons::fabricate()
	{
		return new RockInfantryWeapons(*basic);
	}
	bool RockInfantryWeapons::canPenetrate(Defences& def)
	{
		return (def.getArmour(infantry) > def.getArmour(AP) )?
			(def.getArmour(AP) -1 > basic->penetration) : 
			(def.getArmour(infantry) - 1 > basic->penetration);
	}
	
	int RockSniperWeapons::calcnoise()
	{
		return 1;
	}

	double RockSniperWeapons::calcResist(Defences& def)
	{
		double Resist = (def.getResistanse(infantry) > def.getResistanse(AP)) ?
			def.getResistanse(AP) : def.getResistanse(infantry);
		int Armour = (def.getArmour(infantry) > def.getArmour(AP)) ?
			def.getArmour(AP) : def.getArmour(infantry);
		double percent = (Resist) / 100 + (
			(Armour > basic->penetration) ? 0.1 : 0);
		if (percent > 1) percent = 1;
		return percent;
	}

	abs_damage* RockSniperWeapons::fabricate()
	{
		return new RockSniperWeapons(*basic, sniperDamage, minimumForce);
	}

	bool RockSniperWeapons::canPenetrate(Defences& def)
	{
		return (def.getArmour(infantry) > def.getArmour(AP)) ?
			(def.getArmour(AP) - 1 > basic->penetration) :
			(def.getArmour(infantry) - 1 > basic->penetration);
	}

}