#include "UnitPaintingFunctions.h"


void unit_shader_1(const Model::unit_map_view_charset& uch, QPainter* painter, Hexagon& hg)
{
	painter->setPen(QColor(28, 199, 237));
	painter->setBrush(Qt::NoBrush);
	QRect rect(find_corner(hg.center, hg.size * 0.6, 4), QSize(hg.size * 1.0, hg.size * 0.7));
	painter->drawRect(rect);
	painter->drawLine(rect.topLeft(), rect.bottomRight());
	painter->drawLine(rect.bottomLeft(), rect.topRight());
	rect = QRect(find_corner(hg.center, hg.size * 0.55, 2, false), QSize(hg.size * 0.5, hg.size * 0.3));
	painter->setBrush(QColor(135, 78, 163));
	painter->setPen(Qt::black);
	painter->drawRect(rect);
	painter->setPen(Qt::white);
	QPoint textPoint = rect.bottomLeft();
	painter->drawText(textPoint, QString::number(10));
	painter->setPen(QPen());
	painter->setBrush(QBrush());
}



void View::drawUnit(const Model::unit_map_view_charset& uch, QPainter* painter, Hexagon & hg)
{
	if (!uch.isTextured)
	{
		switch (uch.shaderId)
		{
		case 1:
			unit_shader_1(uch, painter, hg);
		default:
			break;
		}
	}
}
