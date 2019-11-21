#pragma once
#include <string>
#include "Charsets/terrain_view_charset.h"

/*
	Этот класс содержит местность - а именно её стандартные элементы, такие как стоимость прохода по ней
	и стандартные укрепления которые она предоставляет.
	Пока класс очень неполный. Нужны: табличная проходимость
*/

namespace Model {
	class terrain
	{
	public:
		enum TerrainType { grass };
	protected:
		TerrainType type;		// тип местности
		int baseFortification;	// укрепления, которые сразу выставляются юниту входящему на эту клетку
		int passCost;			// стоимость прохода
		terrain_view_charset view;	// вид данной местоности
	public:
		terrain(TerrainType t = grass, int bf=0, int pc=1);
		void setCharset(terrain_view_charset);
		const terrain_view_charset& getViewOfThis() const;

	};
}