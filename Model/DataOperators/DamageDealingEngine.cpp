#include "DamageDealingEngine.h"
#include <queue>


namespace Model
{
	struct DamageInqueuingValue
	{
		int initiative;
		bool ofAttacker;
		damagePointer weapon;
	};
	bool operator<(const DamageInqueuingValue& f, const DamageInqueuingValue& s)
	{
		if (f.initiative == s.initiative)
		{
			if (f.ofAttacker && s.ofAttacker)
				return f.initiative < s.initiative;
			if (!f.ofAttacker && !s.ofAttacker)
				return f.initiative < s.initiative;
			if (f.ofAttacker)
				return f.initiative + 1 < s.initiative;
			return f.initiative < s.initiative + 1;
		}
		return f.initiative < s.initiative;
	}
	typedef std::priority_queue<DamageInqueuingValue, std::vector<DamageInqueuingValue> > InitiativeScale;

	Model::DamageDealingEngine::DamageDealingEngine()
		: unitDied(false), statistics(), hasStatistics(false)
	{
	}
	bool DamageDealingEngine::processDamageBetween(UnitPointer attacker, UnitPointer defender, int range, HexCoords::CubeDirections attackDirection, HexMapCell* mapCell)
	{
		clear();
		hasStatistics = true;
		statistics = new DamageDealingStatistics;
		std::vector<damagePointer>& weaponsOfAttacker = attacker->getWeaponsList();
		std::vector<damagePointer>& weaponsOfDefender = defender->getWeaponsList();
		frontmaps& attackerFrontmap = attacker->getFrontmap();
		frontmaps& defenderFrontmap = defender->getFrontmap();
		InitiativeScale initiativeScale;
		auto begin = weaponsOfAttacker.begin();
		while (begin != weaponsOfAttacker.end())
		{
			initiativeScale.push(DamageInqueuingValue{ (*begin)->getInitiative(true), true, *begin });
			++begin;
		}
		begin = weaponsOfDefender.begin();
		while (begin != weaponsOfDefender.end())
		{
			initiativeScale.push(DamageInqueuingValue{ (*begin)->getInitiative(false), false, *begin });
			++begin;
		}
		DamageInqueuingValue currentWeapon;
		unsigned int damageDealt;
		UnitPointer target;
		UnitPointer dealer;
		CubeDirections counterAttackDirection = reverseDirection(attackDirection);
		while (!initiativeScale.empty())
		{
			currentWeapon = initiativeScale.top();
			initiativeScale.pop();
			if (currentWeapon.weapon->inRange(range))
			{
				dealer = (currentWeapon.ofAttacker) ? attacker : defender;
				target = (currentWeapon.ofAttacker) ? defender : attacker;
				damageDealt = currentWeapon.weapon->countDamage(target->getDefences(), dealer->myForce(), currentWeapon.ofAttacker);
				damageDealt = target->getFrontmap().alterDamageFromDirection(
					(currentWeapon.ofAttacker) ? attackDirection : counterAttackDirection, damageDealt
				);
				if (currentWeapon.ofAttacker)
				{
					statistics->damageDealt.first += damageDealt;
					statistics->damageBlocked.second += currentWeapon.weapon->getNominalDamage() - damageDealt;
				}
				else
				{
					statistics->damageDealt.second += damageDealt;
					statistics->damageBlocked.first += currentWeapon.weapon->getNominalDamage() - damageDealt;
					statistics->wasRetaliation = true;
				}
				if (target->applyDamageToThis(damageDealt))
				{
					unitDied = true;
					statistics->targetDied = (currentWeapon.ofAttacker) ? true : false;
					if (statistics->targetDied)
					{
						statistics->unitViews = ViewPair(attacker->getViewOfThis(), unit_map_view_charset());
					}
					else
					{
						statistics->unitViews = ViewPair(unit_map_view_charset(), defender->getViewOfThis());
					}
					return true;
				}
			}
		}
		statistics->unitViews = ViewPair(attacker->getViewOfThis(), defender->getViewOfThis());
		return false;
	}

	void DamageDealingEngine::clear()
	{
		if (hasStatistics)
		{
			if (statistics != nullptr)
			{
				delete statistics;
			}
		}
		hasStatistics = false;
		unitDied = false;
	}

	bool DamageDealingEngine::isUnitDeathProcessingRequired()
	{
		return unitDied;
	}

	DamageDealingEngine& DamageDealingEngine::operator=(DamageDealingEngine& sdde)
	{
		clear();
		if (sdde.hasStatistics)
		{
			hasStatistics = true;
			statistics = new DamageDealingStatistics(sdde.getStatistics());
		}
		unitDied = sdde.unitDied;
		return *this;
	}
	
	DamageDealingEngine& DamageDealingEngine::operator=(DamageDealingEngine&& mdde) noexcept
	{
		clear();
		hasStatistics = mdde.hasStatistics;
		statistics = mdde.statistics;
		mdde.statistics = nullptr;
		mdde.hasStatistics = false;
		unitDied = mdde.unitDied;
		return *this;
	}

	DamageDealingStatistics DamageDealingEngine::getStatistics()
	{
		return *statistics;
	}

	statisticsPtr DamageDealingEngine::takeStatistics()
	{
		statisticsPtr toreturn(statistics);
		statistics = nullptr;
		hasStatistics = false;
		unitDied = false;
		return std::move(toreturn);
	}

	DamageDealingEngine::~DamageDealingEngine()
	{
		if (hasStatistics)
		{
			if (statistics != nullptr)
				delete statistics;
		}
	}


}