#include "SimpliestController.h"
#include "Model/Units/infantry_units_predefined.h"
#include <QTimer>
#include "debugtrace.h"
namespace Controller {
	SimpliestController::SimpliestController(QObject* parent)
		: QObject(parent), mainMap(), viewMap(), innerWidget(new FieldWidget(viewMap, Q_NULLPTR))
	{
		std::vector<int> res({ 0,0,0,0,0,0 });
		std::vector<int> arm({ 2,0,0,0,0,0 });
		UnitPointer up(EGA_regular_infantry->clone());
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
	void SimpliestController::attack(ICube attacker, ICube defender)
	{
		if (!(mainMap[attacker].isUnitInside() && mainMap[defender].isUnitInside()))
		{
			return;
		}
		if (!mainMap[attacker].getUnit()->canAttackOnRange(attacker.distanceTo(defender)))
		{
			return;
		}
		DamageDealingEngine dde;
		dde.processDamageBetween(
			mainMap[attacker].getUnit(), mainMap[defender].getUnit(),
			attacker.distanceTo(defender), directionTo(attacker, defender), 
			&mainMap[defender]);
		statisticsPtr result = dde.takeStatistics();
		if (result->unitDied)
		{
			mainMap[(result->targetDied)? defender: attacker].killUnitInside();
		}
		viewMap.at(attacker) = mainMap[attacker].makeView(0);
		viewMap.at(defender) = mainMap[defender].makeView(0);
		innerWidget->setFightResult(std::move(result), defender);
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
		mainMap[ICube(1, 2, -3)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Lefttop);
		viewMap[ICube(1, 2, -3)].getUnitView() = mainMap[ICube(1, 2, -3)].getUnit()->getViewOfThis();
		mainMap[ICube(2, 2, -4)].getUnit()->getFrontmap().turn(HexCoords::CubeDirections::Rigthlow);
		viewMap[ICube(2, 2, -4)].getUnitView() = mainMap[ICube(2, 2, -4)].getUnit()->getViewOfThis();
		attack(ICube(1, 1, -2), ICube(1, 2, -3));
		innerWidget->update();
	}
}
