#include "unit_view_charset.h"
namespace Model {
	Model::unit_map_view_charset::unit_map_view_charset(int ut, int si, bool isT, std::wstring tp)
		: unitType(ut), shaderId(si), isTextured(isT), texturePath(tp)
	{
	}

	namespace DefaultCharsets {
		const unit_map_view_charset EGA_Infantry_default_charset(1, 1, false, L"");
	}
}