#pragma once
#include <string>

/*
	Charset - это набор характеристик описывающих будущий вид. Тут не содержатся отрисовывающие функции или картинки, только пути 
	и\или настройки шейдеров
*/
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