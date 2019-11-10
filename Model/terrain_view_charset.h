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
		terrain_view_charset(int, int, bool, std::wstring);
	};
	namespace DefaultCharsets
	{
		extern terrain_view_charset GrassTerrainCharset;
	};
};