#pragma once
#include <string>


namespace Model
{
	class terrain_view_charset
	{
	public:
		int terrain_type;
		int tshader_id;
		int isTexturised;
		std::wstring texturePath;
		terrain_view_charset(int tt = 0, int ti = -1, bool iT = false, std::wstring nm = L"" );
	};
	namespace DefaultCharsets
	{
		extern terrain_view_charset GrassTerrainCharset;
	};
	
};