#pragma once
#include <QPolygon>
#include <Model/CubeCoords.h>
#include <vector>

/*
	Этот файл содержит класс шестиугольника. Он важен для View, поэтому частично основан на Qt, но его можно легко перенести на обычные плюсы
*/
struct Hexagon
{
	QPolygon polygonOfThis;	// вектор из точек шестиугольника
	QPoint center;			// центр
	int size;				// радиус
	Hexagon(const QPoint& center = QPoint(0,0), const int size = 2);
};
// эта неприметная функция рассчитывает точку на расстоянии size от точки center, при угле равном corner_num * 60 (-30 если заострённый треугольник)
// она очень активно используется в рисовании, поскольку сама задача "найти точку" используется очень часто
QPoint	find_corner(const QPoint center, const int size,
	const int corner_num, const bool pointy = true);
QPoint  find_point_on(const QPoint center, const int distance, const double grades);