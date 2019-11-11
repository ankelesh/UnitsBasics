#pragma once
#include "Model/Terrains/Charsets/terrain_view_charset.h"
#include <QPainter>
#include "Model/Hexbattlefield/Hexagon.h"

namespace View
{
	void drawTerrain(const Model::terrain_view_charset& uch, QPainter* painter, Hexagon & hg);
}