#pragma once
#include <string>
#include "Charsets/terrain_view_charset.h"



namespace Model {
	class terrain
	{
	public:
		enum TerrainType { grass };
	protected:
		TerrainType type;
		int baseFortification;
		int passCost;
		terrain_view_charset view;
	public:
		terrain(TerrainType t = grass, int bf=0, int pc=1);
		void setCharset(terrain_view_charset);
		const terrain_view_charset& getViewOfThis() const;

	};
}