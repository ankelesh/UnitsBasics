#include "terrain_view_charset.h"
namespace Model {
	terrain_view_charset::terrain_view_charset(int tt, int ti, bool isT, std::wstring tp)
		: terrain_type(tt), tshader_id(ti), isTexturised(isT), texturePath(tp)
	{
	}
	namespace DefaultCharsets
	{
		terrain_view_charset GrassTerrainCharset(1, 1, false, L"");
	}
}