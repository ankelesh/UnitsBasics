#pragma once
#include "Model/CubeCoords.h"
#include "Model/Terrains/terrain.h"
#include "Model/Units/abs_unit.h"
#include <bitset>
using namespace HexCoords;
namespace Model {
	class HexViewMapCell;
	class HexMapCell
	{
	protected:
		terrain Terrain;
		UnitPointer UnitHere;
		bool unitInside;
		std::bitset<16> player_visibility;

	public:
		HexMapCell();
		HexMapCell(terrain t, UnitPointer u);
		HexMapCell(terrain t);
		const UnitPointer getUnit();
		bool isUnitInside();
		const terrain& getTerrain();
		HexViewMapCell makeView(int forPlayer);
		UnitPointer removeUnit();
		bool killUnitInside();
	};
	class HexViewMapCell
	{
	protected:
		terrain_view_charset terrview;
		unit_map_view_charset unitview;
		bool isVisible;
	public:
		HexViewMapCell(terrain_view_charset t = terrain_view_charset(), unit_map_view_charset u = unit_map_view_charset(), bool isV = false);
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