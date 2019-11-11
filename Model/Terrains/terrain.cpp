#include "terrain.h"
namespace Model {
	Model::terrain::terrain(TerrainType t, int bf, int pc)
		: type(t), baseFortification(bf), passCost(pc), view(DefaultCharsets::GrassTerrainCharset)
	{

	}

	void terrain::setCharset(terrain_view_charset v)
	{
		view = v;
	}

	const terrain_view_charset& terrain::getViewOfThis() const
	{
		return view;
	}


}