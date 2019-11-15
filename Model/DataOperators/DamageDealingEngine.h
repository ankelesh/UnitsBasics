#pragma once
#include "Model/Units/abs_unit.h"
#include <utility>
#include <memory>
#include "Model/Hexbattlefield/MapCell.h"
namespace Model
{
	typedef std::pair<unit_map_view_charset, unit_map_view_charset> ViewPair;
	struct DamageDealingStatistics
	{
		bool unitDied;
		bool targetDied;
		ViewPair unitViews;
		std::pair<unsigned int, unsigned int> damageDealt;
		std::pair<unsigned int, unsigned int> damageBlocked;
		bool wasRetaliation;
		DamageDealingStatistics();
	};
	typedef std::unique_ptr<DamageDealingStatistics> statisticsPtr;
	class DamageDealingEngine
	{
	private:
		DamageDealingStatistics* statistics;
		bool hasStatistics;
	public:
		DamageDealingEngine();
		bool processDamageBetween(UnitPointer attacker, UnitPointer defender, int range, HexCoords::CubeDirections attackDirection, HexMapCell* mapCell);
		void clear();
		bool isUnitDeathProcessingRequired();
		DamageDealingEngine& operator=(DamageDealingEngine& sdde);
		DamageDealingEngine& operator=(DamageDealingEngine&& mdde) noexcept;
		DamageDealingStatistics getStatistics();
		statisticsPtr takeStatistics();
		~DamageDealingEngine();
	};
}