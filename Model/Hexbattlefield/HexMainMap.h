#pragma once
#include "Model/CubeCoords.h"
#include "MapCell.h"
#include <map>


namespace Model {
	using namespace HexCoords;
	typedef std::map<ICube, HexMapCell> HexMap;
	typedef std::map<ICube, HexViewMapCell> HexViewMap;
	class HexMainMap
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