#include "infantry_weapons.h"

namespace Model {

	// сериализация-десериализация
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

	// оверрайды
	int  InfantryWeapons::calcDP(Defences& def)
		// на стрелковое оружие не работает динамическая защита
	{
		return 0;
	}
	int  InfantryWeapons::calcnoise()
		// всё стандартное пехотное оружие производит слишком мало шума чтобы отразиться на карте
	{
		return 0;
	}
	bool  InfantryWeapons::canRetaliate()
		// возвращает то, можно ли этим оружием контратаковать
	{
		return basic->bulky;
	}
	int  InfantryWeapons::calcdmg(const int force, const bool isAttacking)
		// возвращает урон по стандартной формуле. Для большинства стандартного оружия не важно, атака или оборона
	{
		return (int)( //округление
			(double)basic->amount    // базовое количество
			* ((double)force / 10)   // сила отряда 
			);
	}
	bool  InfantryWeapons::canReach(const int rng)
		// стандартная формула досягаемости
	{
		return basic->max_range >= rng;
	}
	bool  InfantryWeapons::canPenetrate(Defences& def)
		// смотрит на то чтобы противопехотная броня могла быть пробита. Порог бронебойности - 1, т.е. оружие может пробить 1 мм сверх бронебойности со штрафом урона
	{
		return (basic->penetration >= def.getArmour(infantry)-1);
	}

	double  InfantryWeapons::calcResist(Defences& def)
		// рассчитывает резист, учитывая порог бронебойности
	{
		double percent = ((double)def.getResistanse(infantry)) / 100   // берётся дефолтное сопротивление
			+ ((def.getArmour(infantry) > basic->penetration) ? 0.1 : 0  // если порог не пройден - то урон всё ещё наносится, но с большим штрафом - 0.1 урона пройдёт
				);
		if (percent > 1) percent = 1;			// процент не может быть более 1
		return percent;
	}
	int  InfantryWeapons::damageFortifications(Defences& def, const bool isAttacking)
		// стрелковое оружие не повреждает укрепления
	{
		return 0;
	}
	abs_damage* InfantryWeapons::fabricate()
		// создаёт копию данного оружия
	{
		return new InfantryWeapons(basic);
	}

	int InfantryWeapons::calcIni( const bool isAttacking)
		// на инициативу обычного стрелкового оружия не влияет то, атакует ли отряд
	{
		return basic->initiative;
	}
	// конструкторы
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
	//-----------------------------------------------------------------------------------------------------
	abs_damage* SniperWeapons::fabricate()
		// копирует снайперское оружие
	{
		return new SniperWeapons(*basic, sniperDamage, minimumForce);
	}
	// сериализация - десериализация
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
	//подсчёт урона. Если соблюдены условия минимальной силы - урон увеличивается на 1 
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
	//-----------------------------------------------------------------------------------------------
	int SubmachWeapons::calcIni(const bool isAttacking)
	// ПП в атаке имеют бонус инициативы
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
	//--------------------------------------------------------------------------------------------------
	int LightMachWeapons::calcdmg(const int force, const bool isAttacking)
		// Ручные пулемёты всегда наносят полный урон если соблюдается условие минимальной силы
	{
		if (force >= minimumForce)
			return basic->amount;
		return (int)((double)basic->amount * ((double)force / 10));	// иначе урон режется как и всегда
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
	//----------------------------------------------------------------------------------------------------
	int HeavyMachWeapons::calcIni(const bool isAttacking)
		// тяжёлые пулемёты имеют очень большой бонус инициативы в обороне
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
	//-----------------------------------------------------------------------------------------------------
	int RockInfantryWeapons::calcnoise()
		// рокское оружие шумит
	{
		return 1;
	}
	double RockInfantryWeapons::calcResist(Defences& def)
		// Рокское оружие выбирает из AP и пехотной брони минимальную. Это важно в случае лёгкой техники бронированной против пуль
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
		// Схема рассчёта пробивания стандартна, но выбирает из АР и пехотной брони минимальную
	{
		return (def.getArmour(infantry) > def.getArmour(AP) )?
			(def.getArmour(AP) -1 > basic->penetration) : 
			(def.getArmour(infantry) - 1 > basic->penetration);
	}
	//------------------------------------------------------------------------------------------------------------
	int RockSniperWeapons::calcnoise()
	{
		return 1;
	}

	double RockSniperWeapons::calcResist(Defences& def)
		// сопротивление считается как обычно, за исключением выбора
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