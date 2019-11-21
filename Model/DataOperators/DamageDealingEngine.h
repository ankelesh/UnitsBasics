#pragma once
#include "Model/Units/abs_unit.h"
#include <utility>
#include <memory>
#include "Model/Hexbattlefield/MapCell.h"

/*
		Этот файл содержит механизм нанесения урона. Помимо собственно корректной обработки он так же собирает статистику в отдельный
	объект статистики, который несёт в себе помимо всего прочего - информацию о изменениях вида юнитов. Объект статистики позже
	передаётся целиком наружу, либо просто удаляется перед новой обработкой.
*/



namespace Model
{
	// облегчение читабельности
	typedef std::pair<unit_map_view_charset, unit_map_view_charset> ViewPair;
	struct DamageDealingStatistics
		// Эта структура хранит в себе всю информацию, которая может быть нужна контроллеру
	{
		bool unitDied;	// умер ли юнит в бою
		bool targetDied;	// умерла ли цель
		ViewPair unitViews;	// изменившиеся view юнитов
		std::pair<unsigned int, unsigned int> damageDealt; // статистика по урону
		std::pair<unsigned int, unsigned int> damageBlocked;	// статистика по заблокированному урону
		bool wasRetaliation;	// статистика по наличию контратаки
		DamageDealingStatistics();	// дефолтный конструктор заполняет всё нулями
	};
	typedef std::unique_ptr<DamageDealingStatistics> statisticsPtr;	// уникальный поинтер на статистику. Для передачи его обязательно move'ать
	class DamageDealingEngine
		// Класс механизма нанесения урона. По сути это функтор, а не полноценный класс, поэтому есть вероятность что нужно будет удалить его потом
	{
	private:
		DamageDealingStatistics* statistics;	// поинтер на актуальную статистику. Её можно peek'нуть.
		bool hasStatistics;						// флаг чтобы не ловить нуллпоинтеры
	public:
		DamageDealingEngine();					
		// основная функция. Её задача - посчитать урон и отдать статистику. ВНИМАНИЕ - УРОН СРАЗУ НАНОСИТСЯ. БЭКАПА НЕ ПРОИСХОДИТ
		bool processDamageBetween(UnitPointer attacker, UnitPointer defender, int range, HexCoords::CubeDirections attackDirection, HexMapCell* mapCell);
		// стереть статистику
		void clear();
		// заглядывает в статистику возвращая unitDied
		bool isUnitDeathProcessingRequired();
		// конструкторы копирования
		DamageDealingEngine& operator=(DamageDealingEngine& sdde);
		DamageDealingEngine& operator=(DamageDealingEngine&& mdde) noexcept;
		// возвращает копию статистики
		DamageDealingStatistics getStatistics();
		// забирает статистику, оставляя поинтер пустым
		statisticsPtr takeStatistics();
		~DamageDealingEngine();
	};
}