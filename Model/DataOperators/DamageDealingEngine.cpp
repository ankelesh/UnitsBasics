#include "DamageDealingEngine.h"
#include <queue>
#include "debugtrace.h"

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

	std::wstring show(DamageDealingStatistics* dataToShow)
	{
		std::wostringstream sout;
		sout <<  L"Battle statistics:\n" << dataToShow->damageDealt.first << L" damage dealt by attacker\n from them "
			<< dataToShow->damageBlocked.second << L" damage was blocked\n"
			<< dataToShow->damageDealt.second << L" damage dealt by defender\nFrom them "
			<< dataToShow->damageBlocked.first << L" damage was blocked\n" <<  ((dataToShow->wasRetaliation) ? L"defender retaliated" : L"defender did not retaliated");
		return sout.str();
	}

	Model::DamageDealingEngine::DamageDealingEngine()
		: statistics(), hasStatistics(false)
	{
	}
	bool DamageDealingEngine::processDamageBetween(UnitPointer attacker, UnitPointer defender, int range, HexCoords::CubeDirections attackDirection, HexMapCell* mapCell)
	{
		clear();
		detrace_METHCALL("processDamageBetween: " << attacker->getName() << " vs " << defender->getName());
		hasStatistics = true;
		statistics = new DamageDealingStatistics;
		std::vector<damagePointer>& weaponsOfAttacker = attacker->getWeaponsList();
		std::vector<damagePointer>& weaponsOfDefender = defender->getWeaponsList();
		detrace_METHDATAS("pdb", "len of arsenals: ", << weaponsOfAttacker.size() << " | " << weaponsOfDefender.size());
		frontmaps& attackerFrontmap = attacker->getFrontmap();
		frontmaps& defenderFrontmap = defender->getFrontmap();
		InitiativeScale initiativeScale;
		auto begin = weaponsOfAttacker.begin();
		while (begin != weaponsOfAttacker.end())
		{
			initiativeScale.push(DamageInqueuingValue{ (*begin)->getInitiative(true), true, *begin });
			detrace_CYCLEEXPL("pushing " << (*begin)->serialize());
			++begin;
		}
		begin = weaponsOfDefender.begin();
		while (begin != weaponsOfDefender.end())
		{
			initiativeScale.push(DamageInqueuingValue{ (*begin)->getInitiative(false), false, *begin });
			detrace_CYCLEEXPL("pushing " << (*begin)->serialize());
			++begin;
		}
		DamageInqueuingValue currentWeapon;
		unsigned int damageDealt;
		UnitPointer target;
		UnitPointer dealer;
		attacker->getFrontmap().turn(attackDirection);
		CubeDirections counterAttackDirection = reverseDirection(attackDirection);
		detrace_METHEXPL("cAD: " << directionToString(reverseDirection(attackDirection)));
		detrace_METHEXPL("now frontmap:\n" << defenderFrontmap.show());
		while (!initiativeScale.empty())
		{
			currentWeapon = initiativeScale.top();
			initiativeScale.pop();

			detrace_CYCLEEXPL("initiativeScale processing. Now value: " << currentWeapon.weapon->serialize());
			if (currentWeapon.weapon->inRange(range))
			{
				dealer = (currentWeapon.ofAttacker) ? attacker : defender;
				target = (currentWeapon.ofAttacker) ? defender : attacker;
				detrace_METHDATAS("iscS", "attacker, defender, dealer, target", << (int)&*attacker << (int)&*defender << (int)&*dealer << (int)&*target);
				damageDealt = currentWeapon.weapon->countDamage(target->getDefences(), dealer->myForce(), currentWeapon.ofAttacker);
				detrace_METHEXPL("damage dealt altered: now " << damageDealt);
				damageDealt = target->getFrontmap().alterDamageFromDirection(
					(currentWeapon.ofAttacker) ? attackDirection : counterAttackDirection, damageDealt
				);
				detrace_METHEXPL("damage corrected by frontmap: " << damageDealt);
				if (currentWeapon.ofAttacker)
				{
					statistics->damageDealt.first += damageDealt; 
					statistics->damageBlocked.first += (currentWeapon.weapon->getNominalDamage() > damageDealt) ? (currentWeapon.weapon->getNominalDamage() - damageDealt) : 0;
					detrace_METHEXPL("altering statistics: damage dealt of " << " attacker " << statistics->damageDealt.first
						<< " and damage blocked by " << " defender" << statistics->damageBlocked.second);
					detrace_METHDATAS("iscS", "nominal damage ", << currentWeapon.weapon->getNominalDamage());
				}
				else
				{
					statistics->damageDealt.second += damageDealt;
					statistics->damageBlocked.first += (currentWeapon.weapon->getNominalDamage() > damageDealt) ? (currentWeapon.weapon->getNominalDamage() - damageDealt) : 0;
					statistics->wasRetaliation = true;
					detrace_METHEXPL("altering statistics: damage dealt of " << " defender"<< statistics->damageDealt.second
						<< " and damage blocked by " << " attacker " << statistics->damageBlocked.first);
					detrace_METHDATAS("iscS", "nominal damage ", << currentWeapon.weapon->getNominalDamage());
				}
				if (target->applyDamageToThis(damageDealt))
				{
					detrace_METHEXPL("target died after applying " << damageDealt);
					statistics->unitDied = true;
					statistics->targetDied = (currentWeapon.ofAttacker) ? true : false;
					if (statistics->targetDied)
					{
						statistics->unitViews = ViewPair(attacker->getViewOfThis(), unit_map_view_charset());
					}
					else
					{
						statistics->unitViews = ViewPair(unit_map_view_charset(), defender->getViewOfThis());
					}
					detrace_METHEXPL("statistics snapshot: " << show(statistics));
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
	}

	bool DamageDealingEngine::isUnitDeathProcessingRequired()
	{
		if (hasStatistics)
			return statistics->unitDied;
		return false;
	}

	DamageDealingEngine& DamageDealingEngine::operator=(DamageDealingEngine& sdde)
	{
		clear();
		if (sdde.hasStatistics)
		{
			hasStatistics = true;
			statistics = new DamageDealingStatistics(sdde.getStatistics());
		}
		return *this;
	}
	
	DamageDealingEngine& DamageDealingEngine::operator=(DamageDealingEngine&& mdde) noexcept
	{
		clear();
		hasStatistics = mdde.hasStatistics;
		statistics = mdde.statistics;
		mdde.statistics = nullptr;
		mdde.hasStatistics = false;
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