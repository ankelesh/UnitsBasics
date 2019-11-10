#pragma once
#include <string>
#include "terrain_view_charset.h"




class terrain
{
public:
	enum TerrainType {grass};
protected:
	std::wstring name;
	int baseFortification;
	int passCost;
	


};