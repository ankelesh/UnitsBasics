#include "SimpliestController.h"
#include "Model/Units/infantry_units.h"
#include "Model/Units/Damage/infantry_weapons_predefined.h"

namespace Controller {
	SimpliestController::SimpliestController(QObject* parent)
		: QObject(parent), mainMap(), viewMap(), innerWidget(new FieldWidget(viewMap, Q_NULLPTR))
	{
		std::vector<int> res({ 0,0,0,0,0,0 });
		std::vector<int> arm({ 2,0,0,0,0,0 });
		Defences def(res, arm, DynamicDefence(), Fortifications());
		frontmaps fmap;
		std::wstring nm = L"ing";
		std::vector<damagePointer> wp({ damagePointer(EGA_SAR_2->clone()) });
		UnitPointer up(new EGA_infantry(10U, wp, def, fmap, std::wstring(L"Infantry")));
		mainMap[ICube(1, 1, -2)] = HexMapCell(terrain(), up);
		viewMap = mainMap.createViewMap();
	}

	void SimpliestController::show()
	{
		innerWidget->show();
	}
}
