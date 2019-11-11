#pragma once
#include "Model/Units/Charsets/unit_view_charset.h"
#include <QPainter>
#include "Model/Hexbattlefield/Hexagon.h"


namespace View
{
	void drawUnit(const Model::unit_map_view_charset & uch, QPainter* painter, Hexagon & hg);
}