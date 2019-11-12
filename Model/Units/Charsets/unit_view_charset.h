#pragma once
#include <string>
#include <array>
#include "Model/Units/Defences/frontmaps.h"
namespace Model {
	class unit_map_view_charset
	{
	public:
		int unitType;
		int shaderId;
		bool isTextured;
		std::wstring texturePath;
		unsigned int textureHash;
		std::array<short, 6> frontmap;
		unsigned int force;
		short teamcolor;
		unit_map_view_charset(const frontmaps& fmap ,std::wstring tp, int t = 0, int si = 0, bool iT = false, 
			unsigned int force = 10, short teamcolor = 1);
		unit_map_view_charset();
	};
	namespace DefaultCharsets {
		extern const unit_map_view_charset EGA_Infantry_default_charset;
	}
}