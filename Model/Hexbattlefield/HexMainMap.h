#pragma once
#include "Model/CubeCoords.h"
#include "MapCell.h"
#include <map>

/*
	Этот файл содержит две вариации одной и той же гексагональной карты. Одна - предназначена для операций с реальными объектами
	и содержит в себе клетки которые могут содержать игровые объекты, вторая - предназначена для константного отображения без влияния на 
	состояние игры. Обе основаны на std::map, так как это - лучший способ содержать пластичные карты.
*/

namespace Model {
	using namespace HexCoords;
	// внутренние тайпдефы чтобы не писать часто полное название
	typedef std::map<ICube, HexMapCell> HexMap;
	typedef std::map<ICube, HexViewMapCell> HexVMap;
	class HexViewMap
		// класс карты видов, не может изменить состояние реальной карты, но несёт в себе заодно прямоугольник который займёт карта после отрисовки.
	{
	private:
		HexVMap mainMap;
		QRect occupiedRect;

		void refresh_border_coords(const ICube& coord);
	public:
		HexViewMap();
		void insert(const ICube&, const HexViewMapCell&);
		HexViewMapCell& insertingGet(const ICube&);
		HexViewMapCell& operator[](const ICube&);	// полная поддержка обычных операций над std::map
		HexVMap::iterator begin();
		HexVMap::iterator end();
		HexViewMapCell& at(const ICube&);
		QRect& offsetBorder();
	};
	class HexMainMap
		// класс обычной карты. Сейчас он полупустой, но скоро появятся разнообразные операции над её содержимым в методах
	{
	private:
		HexMap mainMap;

	public:
		HexMainMap();
		void insert(const ICube&, const HexMapCell&);
		HexMapCell& operator[](const ICube&);
		HexMap::iterator begin();
		HexMap::iterator end();
		HexViewMap createViewMap();
	};
}