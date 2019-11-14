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
			temp.insert(start->first,start->second.makeView(0));
			++start;
		}
		return temp;
	}
	void HexViewMap::refresh_border_coords(const ICube& coord)
	{
		auto cpair = coord.toPixel(50, true);
		if (!occupiedRect.contains(cpair.first, cpair.second))
		{
			QPoint leftTopMostPoint, rightLowMostPoint;
			if (cpair.first < leftTopMostPoint.x())
			{
				leftTopMostPoint.rx() = cpair.first;
			}
			else
			{
				if (cpair.first > rightLowMostPoint.x())
				{
					rightLowMostPoint.rx() = cpair.first;
				}
			}
			if (cpair.second < leftTopMostPoint.y())
			{
				leftTopMostPoint.ry() = cpair.second;
			}
			else
			{
				if (cpair.second > rightLowMostPoint.y())
				{
					rightLowMostPoint.ry() = cpair.second;
				}
			}
			occupiedRect = QRect(leftTopMostPoint, rightLowMostPoint);
		}
	}
	HexViewMap::HexViewMap()
	{
	}
	void HexViewMap::insert(const ICube& coord, const HexViewMapCell& view)
	{
		refresh_border_coords(coord);
		mainMap.insert_or_assign(coord, view);
	}
	HexViewMapCell& HexViewMap::insertingGet(const ICube& coord)
	{
		return mainMap[coord];
	}
	HexViewMapCell& HexViewMap::operator[](const ICube& coord)
	{
		refresh_border_coords(coord);
		return mainMap[coord];
	}
	HexVMap::iterator HexViewMap::begin()
	{
		return mainMap.begin();
	}
	HexVMap::iterator HexViewMap::end()
	{
		return mainMap.end();
	}
	HexViewMapCell& HexViewMap::at(const ICube& coord)
	{
		return mainMap.at(coord);
	}
	QRect& HexViewMap::offsetBorder()
	{
		return occupiedRect;
	}
}