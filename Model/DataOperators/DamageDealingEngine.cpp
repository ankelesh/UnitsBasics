#include "DamageDealingEngine.h"
#include <queue>
#include "debugtrace.h"

namespace Model
{
	// эта мелкая структура используется для работы с очередью чтобы не затенять никакие операторы обычного оружия
	struct DamageInqueuingValue
	{
		int initiative;	// копия инициативы
		bool ofAttacker;	// копия флага атакующего
		damagePointer weapon;// поинтер на оружие
	};
	bool operator<(const DamageInqueuingValue& f, const DamageInqueuingValue& s)
		// компаратор 
	{
		if (f.initiative == s.initiative)
		{
			if (f.ofAttacker == s.ofAttacker)
				return f.initiative < s.initiative;
			if (f.ofAttacker)
				// Преимущество атакующего - при одинаковой инициативе он ходит первым. Для этого оператор сравнения добавляет
				// "фантомную" единицу, которая не выходит за пределы компаратора, т.е. а8 < d9 && a8 > d8 
				return f.initiative + 1 < s.initiative;
			return f.initiative < s.initiative + 1;
		}
		return f.initiative < s.initiative;
	}
	// сокращающий тайпдеф очереди
	typedef std::priority_queue<DamageInqueuingValue, std::vector<DamageInqueuingValue> > InitiativeScale;

	std::wstring show(DamageDealingStatistics* dataToShow)
		// дебаг-вывод статистики
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
	// основной метод - производит подсчёт урона
	{
		clear(); // очищаем статистику
#ifdef DEBUG
		detrace_METHCALL("processDamageBetween: " << attacker->getName() << " vs " << defender->getName());
#endif
		hasStatistics = true;
		statistics = new DamageDealingStatistics;
		std::vector<damagePointer>& weaponsOfAttacker = attacker->getWeaponsList(); // промежуточные ссылки для сокращения запросов
		std::vector<damagePointer>& weaponsOfDefender = defender->getWeaponsList(); 
#ifdef DEBUG
		detrace_METHDATAS("pdb", "len of arsenals: ", << weaponsOfAttacker.size() << " | " << weaponsOfDefender.size());
#endif
		frontmaps& attackerFrontmap = attacker->getFrontmap();
		frontmaps& defenderFrontmap = defender->getFrontmap();
		InitiativeScale initiativeScale;	// создаём новую очередь хода и заполняем её
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
		// создаём временные переменные для облегчения запросов
		DamageInqueuingValue currentWeapon;
		unsigned int damageDealt;
		UnitPointer target;
		UnitPointer dealer;
		// поворот атакующего лицом к защищающемуся. Это - второе преимущество атакующего
		attacker->getFrontmap().turn(attackDirection);
		// рассчитываем направление обратное направлению атаки. Обрати внимание, атакующий берёт НАПРАВЛЕНИЕ КОНТРАТАКИ чтобы сократить свой урон. 
		// это связано с тем, что направление атаки рассчитывается относительно самого атакующего, а направление сокращения урона - относительно
		// обороняющегося
		CubeDirections counterAttackDirection = reverseDirection(attackDirection);
#ifdef DEBUG
		detrace_METHEXPL("cAD: " << directionToString(reverseDirection(attackDirection)));
		detrace_METHEXPL("now frontmap:\n" << defenderFrontmap.show());
#endif
		while (!initiativeScale.empty())
			// пока в очереди ещё есть элементы
		{
			currentWeapon = initiativeScale.top();	// забираем верхний
			initiativeScale.pop();
#ifdef DEBUG
			detrace_CYCLEEXPL("initiativeScale processing. Now value: " << currentWeapon.weapon->serialize());
#endif
			if (currentWeapon.weapon->inRange(range))
				// если текущее оружие может достать на таком расстоянии. Это важно - некоторые отряды будут иметь сильно разнящуюся дальность
			{
				// распределяем по временным переменным указатели на юнитов
				dealer = (currentWeapon.ofAttacker) ? attacker : defender;
				target = (currentWeapon.ofAttacker) ? defender : attacker;
				// подсчитываем урон нанесённый с учётом защиты обороняющегося
				damageDealt = currentWeapon.weapon->countDamage(target->getDefences(), dealer->myForce(), currentWeapon.ofAttacker);
#ifdef DEBUG
				detrace_METHEXPL("damage dealt altered: now " << damageDealt);
#endif
				// карта фронтов уменьшает этот урон или увеличивает. Обрати внимание, направления инвертированы.
				damageDealt = target->getFrontmap().alterDamageFromDirection(
					(currentWeapon.ofAttacker) ? counterAttackDirection : attackDirection, damageDealt
				);
#ifdef DEBUG
				detrace_METHEXPL("damage corrected by frontmap: " << damageDealt);
#endif
				// накидываем в статистику данные
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
				// наносим урон. true - юнит помер. После этого уже не нужно ничего дальше рассчитывать - надо прервать рассчёт и вернуть статистику
				// отражающую этот факт
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
		//создаём новые виды юнитов для статистики
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