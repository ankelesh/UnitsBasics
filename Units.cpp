



#include <iostream>
#include "Model/infantry_weapons_predefined.h"

int main()
{
	using namespace Model;
	using std::wcout;
	std::vector<int> res({ 0,0,0,0,0,0 });
	std::vector<int> arm({ 2,0,0,0,0,0 });
	damagePointer spear = damagePointer(EGA_SAR_2->clone());
	Defences shield(res, arm, DynamicDefence(), Fortifications());
	wcout << "Testing weapon SAR-2 vs:\n" << shield.about() << '\n';
	wcout << "range testing: " << spear->inRange(5) << " << range 5, " << spear->inRange(1)
		<< " << range 1\n";
	wcout << "Damage testing: " << spear->countDamage(shield, 10, true) << " dealt by squad with force 10.\n";
	wcout << "Damage testing: " << spear->countDamage(shield, 5, true) << "dealt by squad with force 5\n";
	system("PAUSE");
}
