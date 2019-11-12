#include "SimpliestController.h"
#include "Model/Units/infantry_units.h"
#include "Model/Units/Damage/infantry_weapons_predefined.h"
#include <QTimer>
namespace Controller {
	SimpliestController::SimpliestController(QObject* parent)
		: QObject(parent), mainMap(), viewMap(), innerWidget(new FieldWidget(viewMap, Q_NULLPTR))
	{
		std::vector<int> res({ 0,0,0,0,0,0 });
		std::vector<int> arm({ 2,0,0,0,0,0 });
		Defences def(res, arm, DynamicDefence(), Fortifications());
		frontmaps fmap(CubeDirections::Right, front::strong, front::reinforced, front::unprotected, front::vulnerable, front::unprotected, front::reinforced);
		std::wstring nm = L"ing";
		std::vector<damagePointer> wp({ damagePointer(EGA_SAR_2->clone()) });
		UnitPointer up(new EGA_infantry(8U, wp, def, fmap, std::wstring(L"Infantry")));
		mainMap[ICube(3,0,-3)] = HexMapCell(terrain(), UnitPointer(up->clone()));
		mainMap[ICube(1, 1, -2)] = HexMapCell(terrain(), up);
		mainMap[ICube(3, 1, -4)] = HexMapCell(terrain(), UnitPointer(up->clone()));
		mainMap[ICube(2, 2, -4)] = HexMapCell(terrain(), UnitPointer(up->clone()));
		UnitPointer sunit(up->clone());
		mainMap[ICube(1, 2, -3)] = HexMapCell(terrain(), sunit);
		mainMap[ICube(2, 0, -2)] = HexMapCell(terrain(), UnitPointer(up->clone()));
		mainMap[ICube(2, 1, -3)] = HexMapCell(terrain());
		viewMap = mainMap.createViewMap();
		QTimer::singleShot(5000, this, SLOT(do_thing()));
	}

	void SimpliestController::show()
	{
		innerWidget->show();
	}
	SimpliestController::~SimpliestController()
	{
		delete innerWidget;
	}
	void SimpliestController::do_thing()
	{
		mainMap[ICube(2, 0, -2)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Lefttop);
		viewMap[ICube(2, 0, -2)].getUnitView() = mainMap[ICube(2, 0, -2)].getUnit()->getViewOfThis();
		mainMap[ICube(3, 0, -3)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Righttop);
		viewMap[ICube(3, 0, -3)].getUnitView() = mainMap[ICube(3, 0, -3)].getUnit()->getViewOfThis();
		mainMap[ICube(1, 1, -2)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Left);
		viewMap[ICube(1, 1, -2)].getUnitView() = mainMap[ICube(1, 1, -2)].getUnit()->getViewOfThis();
		mainMap[ICube(3, 1, -4)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Right);
		viewMap[ICube(3, 1, -4)].getUnitView() = mainMap[ICube(3, 1, -4)].getUnit()->getViewOfThis();
		mainMap[ICube(1, 2, -3)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Leftlow);
		viewMap[ICube(1, 2, -3)].getUnitView() = mainMap[ICube(1, 2, -3)].getUnit()->getViewOfThis();
		mainMap[ICube(2, 2, -4)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Rigthlow);
		viewMap[ICube(2, 2, -4)].getUnitView() = mainMap[ICube(2, 2, -4)].getUnit()->getViewOfThis();
		innerWidget->update();
	}
}
