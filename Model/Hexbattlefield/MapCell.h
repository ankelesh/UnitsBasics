#pragma once
#include "Model/CubeCoords.h"
#include "Model/Terrains/terrain.h"
#include "Model/Units/abs_unit.h"

using namespace HexCoords;
namespace Model {
	class HexMapCell
	{
	protected:
		terrain Terrain;
		UnitPointer UnitHere;

	public:
		HexMapCell();
		HexMapCell(terrain t, UnitPointer u);
		const UnitPointer getUnit();
		const terrain& getTerrain();



	};
	class HexViewMapCell
	{
	protected:
		terrain_view_charset terrview;
		unit_map_view_charset unitview;
	public:
		HexViewMapCell(terrain_view_charset t = terrain_view_charset(), unit_map_view_charset u = unit_map_view_charset());
		terrain_view_charset& getTerrainView();
		unit_map_view_charset& getUnitView();
	
	};
	static bool operator<(const HexMapCell& f, const HexMapCell& s) {
		return &f < &s;
	}
	static bool operator<(const HexViewMapCell& f, const HexViewMapCell& s) {
		return &f < &s;
	}
}