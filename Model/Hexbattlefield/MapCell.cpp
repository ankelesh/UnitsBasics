#include "MapCell.h"
#include <exception>


class ViewCreationException : public std::exception
	// простое исключение, которое бросается в случае если невозможно создать view
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
		: Terrain(t), UnitHere(std::move(u)), unitInside(true), player_visibility()
	{
		if (UnitHere->belongsTo() >= 0 && UnitHere->belongsTo() < player_visibility.size())
		{
			// попытка выставить видимость используя юнита
			player_visibility[UnitHere->belongsTo()] = true;
		}
	}


	HexMapCell::HexMapCell(terrain t)
		: Terrain(t), UnitHere(nullptr), unitInside(false), player_visibility()
	{
	}

	const UnitPointer Model::HexMapCell::getUnit()
		// геттер юнита. Небезопасный, его нужно проверять. Зато не бросает эксепшнов
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
				// кидает эксепшн если нет такого игрока чтобы видимость работала
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
	UnitPointer HexMapCell::removeUnit()
	{
		if (unitInside) {
			// затирает местный поинтер
			unitInside = false;
			UnitPointer tmp(UnitHere);
			UnitHere = nullptr;
			return tmp;
		}
		return nullptr;
	}
	bool HexMapCell::killUnitInside()
	{
		if (unitInside)
		{
			unitInside = false;
			UnitHere = nullptr;
			return true;
		}
		return false;
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