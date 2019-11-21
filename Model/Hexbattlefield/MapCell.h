#pragma once
#include "Model/CubeCoords.h"
#include "Model/Terrains/terrain.h"
#include "Model/Units/abs_unit.h"
#include <bitset>



/*
		В этом файле содержатся классы клеток, из которых впоследствии будет состоять карта.
	Их задача - хранить данные и предоставлять простые методы для манипуляций с ними

*/
using namespace HexCoords;
namespace Model {
	class HexViewMapCell;	// предефайн 
	class HexMapCell
		// Этот класс представляет собой собрание всех нужных для одной клетки элементов. 
		// впоследствии нужно будет вынести их в ->impl для ускорения копирования
	{
	protected:
		terrain Terrain; // местность
		UnitPointer UnitHere;	// указатель на юнита. может быть пустым
		bool unitInside;		// флаг наличия юнита. 
		std::bitset<16> player_visibility;	// видимость игрокам данной клетки

	public:
		HexMapCell(); // конструкторы
		HexMapCell(terrain t, UnitPointer u);
		HexMapCell(terrain t);
		const UnitPointer getUnit(); // возвращает поинтер на юнита
		bool isUnitInside();		// возвращает наличие юнита. Может быть замещено UnitHere == nullptr;
		const terrain& getTerrain();	// возвращает ссылку на местность
		HexViewMapCell makeView(int forPlayer);	// создаёт и возвращает вид данной клетки для одного из игроков. 
		UnitPointer removeUnit();	// move'ает юнита из клетки, возвращая поинтер на него
		bool killUnitInside();		// убивает юнита внутри, после чего если не было копий его поинтера - память очищается.
	};
	class HexViewMapCell
		// Этот класс содержит исключительно вид клетки
	{
	protected:
		terrain_view_charset terrview;	// вид местности
		unit_map_view_charset unitview;	// вид юнита
		bool isVisible;					// флаг видимости юнита
	public:
		HexViewMapCell(terrain_view_charset t = terrain_view_charset(), unit_map_view_charset u = unit_map_view_charset(), bool isV = false);
		terrain_view_charset& getTerrainView();	// геттеры
		unit_map_view_charset& getUnitView();
	
	};
	static bool operator<(const HexMapCell& f, const HexMapCell& s) {
		return &f < &s;
	}
	static bool operator<(const HexViewMapCell& f, const HexViewMapCell& s) {
		return &f < &s;
	}
}