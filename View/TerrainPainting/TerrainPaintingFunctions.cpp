#include "TerrainPaintingFunctions.h"



void terrain_shader_1(const Model::terrain_view_charset& uch, QPainter* painter, Hexagon& hg)
{
	painter->setBrush(QColor(224, 245, 186));
	painter->setPen(Qt::black);
	painter->drawPolygon(hg.polygonOfThis);
	painter->setBrush(QColor(Qt::white));
}




void View::drawTerrain(const Model::terrain_view_charset& uch, QPainter* painter, Hexagon & hg)
{
	if (!uch.isTexturised)
	{
		switch (uch.tshader_id)
		{
		case 1:
			terrain_shader_1(uch, painter, hg);
		default:
			break;
		}
	}
}
