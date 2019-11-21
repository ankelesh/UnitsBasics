#include "Hexagon.h"



const float PI = 3.141592653589793238463;
const float RadMult = PI / 180;

QPoint	find_corner(const QPoint center, const int size,
	const int corner_num, const bool pointy)
{
	float arad = RadMult * ((pointy) ? 60 * corner_num - 30 : 60 * corner_num);
	return QPoint(center.x() + size * cos(arad), center.y() + size * sin(arad));
}

QPoint find_point_on(const QPoint center, const int distance, const double grades)
{
	double arad = RadMult * grades;
	return QPoint(center.x() + distance * cos(arad), center.y() + distance * sin(arad));
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
