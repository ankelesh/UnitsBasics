#pragma once
#include <QPolygon>
#include <Model/CubeCoords.h>
#include <vector>
struct Hexagon
{
	QPolygon polygonOfThis;
	QPoint center;
	int size;
	Hexagon(const QPoint& center = QPoint(0,0), const int size = 0);
};
QPoint	find_corner(const QPoint center, const int size,
	const int corner_num, const bool pointy = true);