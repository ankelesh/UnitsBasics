#include "Hexagon.h"



const double PI = 3.141592653589793238463;

QPoint	find_corner(const QPoint center, const int size,
	const int corner_num, const bool pointy)
{
	float adeg = (pointy) ? 60 * corner_num - 30 : 60 * corner_num;
	float arad = PI / 180 * adeg;
	return QPoint(center.x() + size * cos(arad), center.y() + size * sin(arad));
}
int		hex_width(const int& size)
{
	return size * sqrt(3);
}
int		hex_height(const int& size)
{
	return 2 * size;
}

Hexagon::Hexagon(const QPoint& c, const int sz)
	: polygonOfThis(), center(c), size(sz)
{
	QVector<QPoint> points;
	points.reserve(6);
	for (int i = 0; i < 6; ++i)
	{
		points.push_back(find_corner(center, size, i));
	}
	polygonOfThis = QPolygon(points);
}
