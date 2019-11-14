#include "infantry_units_predefined.h"
#include "Damage/infantry_weapons_predefined.h"
#include <string>

namespace Model
{
	std::vector<damagePointer> EGA_REGULAR_WEAPONS{ damagePointer(EGA_SAR_2->clone()),damagePointer(EGA_SLMG_1->clone()) };
	Defences EGA_REGULAR_DEFENCES(std::vector<int>{2, 0, 0, 0, 0, 0}, std::vector<int>{0, 80, 50, 50, 0, 0}, DynamicDefence(0, 0, false), Fortifications()
);
	frontmaps EGA_REGULAR_FRONTMAP(CubeDirections::Right, front::strong, front::reinforced, front::unprotected, front::vulnerable, front::unprotected, front::reinforced);

	class generateName
	{
	private:
		inline static int counter =0;
	public:
		generateName() {};
		std::wstring operator()()
		{
			++counter;
			return std::wstring(L"squad ") + std::to_wstring(counter);
		}
	};

	const UnitPointer EGA_regular_infantry(new EGA_infantry(10U, EGA_REGULAR_WEAPONS, EGA_REGULAR_DEFENCES,EGA_REGULAR_FRONTMAP, generateName()()));
}