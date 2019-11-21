#pragma once
#include <string>
#include <array>
#include "Model/Units/Defences/frontmaps.h"


/*
	Стандартный набор характеристик для отрисовки юнита.
*/
namespace Model {
	class unit_map_view_charset
	{
	public:
		int unitType;		// Тип юнита
		int shaderId;		// id шейдера который будет использоваться для отрисовки
		bool isTextured;	// флаг, отмечающий какой способ отрисовки использовать - шейдер или текстуру
		std::wstring texturePath;		// путь к текстуре
		unsigned int textureHash;		// хэш пути к текстуре, который использовается для оптимизации количества одинаковых объектов
		std::array<short, 6> frontmap;	// преобразованная в простенький массив фронткарта
		unsigned int force;				// сила
		short teamcolor;				// цвет команды 
		unit_map_view_charset(const frontmaps& fmap ,std::wstring tp, int t = 0, int si = 0, bool iT = false, 
			unsigned int force = 10, short teamcolor = 1);
		unit_map_view_charset();
	};
	namespace DefaultCharsets {
		extern const unit_map_view_charset EGA_Infantry_default_charset;
	}
}