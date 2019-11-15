#pragma once
#include "Model/Hexbattlefield/HexMainMap.h"
#include "Model/Hexbattlefield/HexaGraph.h"


namespace Model
{

	class MovingEngine
	{
	private:
		HexMainMap& mainMap;
		
	public:
		MovingEngine(HexMainMap& mmap);
		std::pair<ICube, ICube> moveToNearest(ICube from, ICube to);
		bool tryMovingTo(ICube from, ICube to);



	};
}