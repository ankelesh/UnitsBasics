#include "MapCell.h"



namespace Model {
	Model::HexMapCell::HexMapCell()
		: Terrain(), UnitHere()
	{
	}

	Model::HexMapCell::HexMapCell(terrain t, UnitPointer u)
		: Terrain(t), UnitHere(UnitPointer(u->clone()))
	{
	}

	const UnitPointer Model::HexMapCell::getUnit()
	{
		return UnitHere;
	}

	const terrain& Model::HexMapCell::getTerrain()
	{
		return Terrain;
	}
	HexViewMapCell::HexViewMapCell(terrain_view_charset t, unit_map_view_charset u)
		: terrview(t), unitview(u)
	{

	}
	terrain_view_charset& HexViewMapCell::getTerrainView()
	{
		return terrview;
	}
	unit_map_view_charset& HexViewMapCell::getUnitView()
	{
		return unitview;
	}
}