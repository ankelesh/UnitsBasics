#include "MovingEngine.h"

Model::MovingEngine::MovingEngine(HexMainMap& mmap)
	: mainMap(mmap)
{
}

std::pair<ICube, ICube> Model::MovingEngine::moveToNearest(ICube from, ICube to)
{
	return std::pair<ICube, ICube>();
}

bool Model::MovingEngine::tryMovingTo(ICube from, ICube to)
{
	return false;
}
