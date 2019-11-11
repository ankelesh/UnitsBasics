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
		unit_map_view_charset(int t = 0, int si = 0, bool iT = false, std::wstring nm = L"");
	};
	namespace DefaultCharsets {
		extern const unit_map_view_charset EGA_Infantry_default_charset;
	}
}