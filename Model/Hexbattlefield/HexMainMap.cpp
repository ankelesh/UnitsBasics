#include "HexMainMap.h"

namespace Model {

	Model::HexMainMap::HexMainMap()
	{
	}

	void Model::HexMainMap::insert(const ICube& c, const HexMapCell& Cell)
	{
		mainMap.insert_or_assign(c, Cell);
	}

	HexMapCell& Model::HexMainMap::operator[](const ICube& c)
	{
		return mainMap[c];
	}

	HexMap::iterator Model::HexMainMap::begin()
	{
		return mainMap.begin();
	}

	HexMap::iterator Model::HexMainMap::end()
	{
		return mainMap.end();
	}
	HexViewMap HexMainMap::createViewMap()
	{
		HexViewMap temp;
		HexMap::iterator start = begin();
		while (start != end())
		{
			temp[start->first] = start->second.makeView(0);
			++start;
		}
		return temp;
	}
}