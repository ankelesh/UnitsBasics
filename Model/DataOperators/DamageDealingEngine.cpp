#include "DamageDealingEngine.h"
#include <queue>
#include "debugtrace.h"

namespace Model
{
	// ��� ������ ��������� ������������ ��� ������ � �������� ����� �� �������� ������� ��������� �������� ������
	struct DamageInqueuingValue
	{
		int initiative;	// ����� ����������
		bool ofAttacker;	// ����� ����� ����������
		damagePointer weapon;// ������� �� ������
	};
	bool operator<(const DamageInqueuingValue& f, const DamageInqueuingValue& s)
		// ���������� 
	{
		if (f.initiative == s.initiative)
		{
			if (f.ofAttacker == s.ofAttacker)
				return f.initiative < s.initiative;
			if (f.ofAttacker)
				// ������������ ���������� - ��� ���������� ���������� �� ����� ������. ��� ����� �������� ��������� ���������
				// "���������" �������, ������� �� ������� �� ������� �����������, �.�. �8 < d9 && a8 > d8 
				return f.initiative + 1 < s.initiative;
			return f.initiative < s.initiative + 1;
		}
		return f.initiative < s.initiative;
	}
	// ����������� ������� �������
	typedef std::priority_queue<DamageInqueuingValue, std::vector<DamageInqueuingValue> > InitiativeScale;

	std::wstring show(DamageDealingStatistics* dataToShow)
		// �����-����� ����������
	{
		std::wostringstream sout;
		sout <<  L"Battle statistics:\n" << dataToShow->damageDealt.first << L" damage dealt by attacker\n from them "
			<< dataToShow->damageBlocked.second << L" damage was blocked\n"
			<< dataToShow->damageDealt.second << L" damage dealt by defender\nFrom them "
			<< dataToShow->damageBlocked.first << L" damage was blocked\n" <<  ((dataToShow->wasRetaliation) ? L"defender retaliated" : L"defender did not retaliated");
		sout << L"\nViews snapshot: " << dataToShow->unitViews.first.force << L"is force of attacker and " << dataToShow->unitViews.second.force
			<< L" is force of defender. ";
		return sout.str();
	}

	Model::DamageDealingEngine::DamageDealingEngine()
		: statistics(), hasStatistics(false)
	{
	}
	bool DamageDealingEngine::processDamageBetween(UnitPointer attacker, UnitPointer defender, int range, HexCoords::CubeDirections attackDirection, HexMapCell* mapCell)
	// �������� ����� - ���������� ������� �����
	{
		clear(); // ������� ����������
#ifdef DEBUG
		detrace_METHCALL("processDamageBetween: " << attacker->getName() << " vs " << defender->getName());
#endif
		hasStatistics = true;
		statistics = new DamageDealingStatistics;
		std::vector<damagePointer>& weaponsOfAttacker = attacker->getWeaponsList(); // ������������� ������ ��� ���������� ��������
		std::vector<damagePointer>& weaponsOfDefender = defender->getWeaponsList(); 
#ifdef DEBUG
		detrace_METHDATAS("pdb", "len of arsenals: ", << weaponsOfAttacker.size() << " | " << weaponsOfDefender.size());
#endif
		frontmaps& attackerFrontmap = attacker->getFrontmap();
		frontmaps& defenderFrontmap = defender->getFrontmap();
		InitiativeScale initiativeScale;	// ������ ����� ������� ���� � ��������� �
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
		// ������ ��������� ���������� ��� ���������� ��������
		DamageInqueuingValue currentWeapon;
		unsigned int damageDealt;
		UnitPointer target;
		UnitPointer dealer;
		// ������� ���������� ����� � �������������. ��� - ������ ������������ ����������
		attacker->getFrontmap().turn(attackDirection);
		// ������������ ����������� �������� ����������� �����. ������ ��������, ��������� ���� ����������� ���������� ����� ��������� ���� ����. 
		// ��� ������� � ���, ��� ����������� ����� �������������� ������������ ������ ����������, � ����������� ���������� ����� - ������������
		// ��������������
		CubeDirections counterAttackDirection = reverseDirection(attackDirection);
#ifdef DEBUG
		detrace_METHEXPL("cAD: " << directionToString(reverseDirection(attackDirection)));
		detrace_METHEXPL("now frontmap:\n" << defenderFrontmap.show());
#endif
		while (!initiativeScale.empty())
			// ���� � ������� ��� ���� ��������
		{
			currentWeapon = initiativeScale.top();	// �������� �������
			initiativeScale.pop();
#ifdef DEBUG
			detrace_CYCLEEXPL("initiativeScale processing. Now value: " << currentWeapon.weapon->serialize());
#endif
			if (currentWeapon.weapon->inRange(range))
				// ���� ������� ������ ����� ������� �� ����� ����������. ��� ����� - ��������� ������ ����� ����� ������ ���������� ���������
			{
				// ������������ �� ��������� ���������� ��������� �� ������
				dealer = (currentWeapon.ofAttacker) ? attacker : defender;
				target = (currentWeapon.ofAttacker) ? defender : attacker;
				// ������������ ���� ��������� � ������ ������ ��������������
				damageDealt = currentWeapon.weapon->countDamage(target->getDefences(), dealer->myForce(), currentWeapon.ofAttacker);
#ifdef DEBUG
				detrace_METHEXPL("damage dealt altered: now " << damageDealt);
#endif
				// ����� ������� ��������� ���� ���� ��� �����������. ������ ��������, ����������� �������������.
				damageDealt = target->getFrontmap().alterDamageFromDirection(
					(currentWeapon.ofAttacker) ? counterAttackDirection : attackDirection, damageDealt
				);
#ifdef DEBUG
				detrace_METHEXPL("damage corrected by frontmap: " << damageDealt);
#endif
				// ���������� � ���������� ������
				if (currentWeapon.ofAttacker)
				{
					statistics->damageDealt.first += damageDealt; 
					statistics->damageBlocked.second += (currentWeapon.weapon->getNominalDamage() >= damageDealt) ? (currentWeapon.weapon->getNominalDamage() - damageDealt) : currentWeapon.weapon->getNominalDamage();
				}
				else
				{
					statistics->damageDealt.second += damageDealt;
					statistics->damageBlocked.first += (currentWeapon.weapon->getNominalDamage() > damageDealt) ? (currentWeapon.weapon->getNominalDamage() - damageDealt) : currentWeapon.weapon->getNominalDamage();
					statistics->wasRetaliation = true;
				}
				if (target->applyDamageToThis(damageDealt))
				// ������� ����. true - ���� �����. ����� ����� ��� �� ����� ������ ������ ������������ - ���� �������� ������� � ������� ����������
				// ���������� ���� ����
				{
#ifdef DEBUG
					detrace_METHEXPL("target died after applying " << damageDealt);
#endif
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
#ifdef DEBUG
					detrace_METHEXPL("statistics snapshot: " << show(statistics));
#endif
					return true;
				}
			}
		}
		//������ ����� ���� ������ ��� ����������
		statistics->unitViews = ViewPair(attacker->getViewOfThis(), defender->getViewOfThis());
#ifdef DEBUG
		detrace_METHEXPL("statistics snapshot: " << show(statistics));
#endif
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


	DamageDealingStatistics::DamageDealingStatistics()
		: unitDied(false), targetDied(false), unitViews(), damageDealt(), damageBlocked(), wasRetaliation(false)
	{
	}

}