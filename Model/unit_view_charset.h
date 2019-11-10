#pragma once
#include <string>
namespace Model {
	class unit_map_view_charset
	{
	public:
		int unitType;
		int shaderId;
		bool isTextured;
		std::wstring texturePath;
		unit_map_view_charset(int, int, bool, std::wstring);
	};
	namespace DefaultCharsets {
		extern const unit_map_view_charset EGA_Infantry_default_charset;
	}
}