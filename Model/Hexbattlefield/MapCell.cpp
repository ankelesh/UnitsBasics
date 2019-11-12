#include "MapCell.h"
#include <exception>


class ViewCreationException : public std::exception
{
	const char msg[68] { "Error creating view! Check visibility parameters or unit existance." };
	const char* what() { return msg; };
};


namespace Model {
	Model::HexMapCell::HexMapCell()
		: Terrain(), UnitHere(), unitInside(false), player_visibility()
	{
	}

	Model::HexMapCell::HexMapCell(terrain t, UnitPointer u)
		: Terrain(t), UnitHere(UnitPointer(u->clone())), unitInside(true), player_visibility()
	{
		if (u->belongsTo() >= 0 && u->belongsTo() < player_visibility.size())
		{
			player_visibility[u->belongsTo()] = true;
		}
	}


	HexMapCell::HexMapCell(terrain t)
		: Terrain(t), UnitHere(nullptr), unitInside(false), player_visibility()
	{
	}

	const UnitPointer Model::HexMapCell::getUnit()
	{
		if (unitInside)
		{
			return UnitHere;
		}
		return UnitPointer(nullptr);
	}

	bool HexMapCell::isUnitInside()
	{
		return unitInside;
	}

	const terrain& Model::HexMapCell::getTerrain()
	{
		return Terrain;
	}
	HexViewMapCell HexMapCell::makeView(int playerToShow)
	{
		if (unitInside)
		{
			if (playerToShow >= 0 && playerToShow < player_visibility.size()) {
				return HexViewMapCell(Terrain.getViewOfThis(), UnitHere->getViewOfThis(), player_visibility[playerToShow]);
			}
			else
			{
				throw ViewCreationException();
			}
		}
		else
		{
			if (playerToShow >= 0 && playerToShow < player_visibility.size()) 
			{
				return HexViewMapCell(Terrain.getViewOfThis(), unit_map_view_charset(), player_visibility[playerToShow]);
			}
			else
			{
				throw ViewCreationException();
			}
		}
	}
	HexViewMapCell::HexViewMapCell(terrain_view_charset t, unit_map_view_charset u, bool isV)
		: terrview(t), unitview(u), isVisible(isV)
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