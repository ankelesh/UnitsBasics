#pragma once
#include "Model/CubeCoords.h"
#include "MapCell.h"
#include <map>

/*
	���� ���� �������� ��� �������� ����� � ��� �� �������������� �����. ���� - ������������� ��� �������� � ��������� ���������
	� �������� � ���� ������ ������� ����� ��������� ������� �������, ������ - ������������� ��� ������������ ����������� ��� ������� �� 
	��������� ����. ��� �������� �� std::map, ��� ��� ��� - ������ ������ ��������� ���������� �����.
*/

namespace Model {
	using namespace HexCoords;
	// ���������� �������� ����� �� ������ ����� ������ ��������
	typedef std::map<ICube, HexMapCell> HexMap;
	typedef std::map<ICube, HexViewMapCell> HexVMap;
	class HexViewMap
		// ����� ����� �����, �� ����� �������� ��������� �������� �����, �� ���� � ���� ������ ������������� ������� ����� ����� ����� ���������.
	{
	private:
		HexVMap mainMap;
		QRect occupiedRect;

		void refresh_border_coords(const ICube& coord);
	public:
		HexViewMap();
		void insert(const ICube&, const HexViewMapCell&);
		HexViewMapCell& insertingGet(const ICube&);
		HexViewMapCell& operator[](const ICube&);	// ������ ��������� ������� �������� ��� std::map
		HexVMap::iterator begin();
		HexVMap::iterator end();
		HexViewMapCell& at(const ICube&);
		QRect& offsetBorder();
	};
	class HexMainMap
		// ����� ������� �����. ������ �� ����������, �� ����� �������� ������������� �������� ��� � ���������� � �������
	{
	private:
		HexMap mainMap;

	public:
		HexMainMap();
		void insert(const ICube&, const HexMapCell&);
		HexMapCell& operator[](const ICube&);
		HexMap::iterator begin();
		HexMap::iterator end();
		HexViewMap createViewMap();
	};
}