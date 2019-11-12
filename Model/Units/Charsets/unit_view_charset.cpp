#include "unit_view_charset.h"
#include <utility>
namespace Model {
	Model::unit_map_view_charset::unit_map_view_charset(const frontmaps& fmap, std::wstring tp, int t, int si, bool iT,
		unsigned int f, short tc)
		: unitType(t), shaderId(si), isTextured(iT), texturePath(tp), textureHash(std::hash<std::wstring>{}(tp)), frontmap(), force(f), teamcolor(tc)
	{
		std::copy(fmap.begin(), fmap.end(), frontmap.begin());
	}

	namespace DefaultCharsets {
		const unit_map_view_charset EGA_Infantry_default_charset;
	}
	unit_map_view_charset::unit_map_view_charset()
		: unitType(0), shaderId(0), isTextured(false), texturePath(), frontmap(), force(0), teamcolor(0)
	{

	}
}