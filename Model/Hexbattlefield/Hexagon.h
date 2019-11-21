#pragma once
#include <QPolygon>
#include <Model/CubeCoords.h>
#include <vector>

/*
	���� ���� �������� ����� ��������������. �� ����� ��� View, ������� �������� ������� �� Qt, �� ��� ����� ����� ��������� �� ������� �����
*/
struct Hexagon
{
	QPolygon polygonOfThis;	// ������ �� ����� ��������������
	QPoint center;			// �����
	int size;				// ������
	Hexagon(const QPoint& center = QPoint(0,0), const int size = 2);
};
// ��� ����������� ������� ������������ ����� �� ���������� size �� ����� center, ��� ���� ������ corner_num * 60 (-30 ���� ���������� �����������)
// ��� ����� ������� ������������ � ���������, ��������� ���� ������ "����� �����" ������������ ����� �����
QPoint	find_corner(const QPoint center, const int size,
	const int corner_num, const bool pointy = true);
QPoint  find_point_on(const QPoint center, const int distance, const double grades);