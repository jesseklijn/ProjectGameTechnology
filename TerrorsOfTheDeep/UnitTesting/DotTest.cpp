////Needs to always be included and at the top
#include "stdafx.h"
////Needs to always be included
#include "catch.hpp"
////specific cpp file where the unit test is
#include "../TerrorsOfTheDeep/UnitTests.cpp"

TEST_CASE("Dot Products", "[Dot]")
{
	// Should pass
	REQUIRE(Dot(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, 0, 0)) == 0);
	REQUIRE(Dot(irr::core::vector3df(1, 1, 1), irr::core::vector3df(3, 4, 5)) == 12);
	REQUIRE(Dot(irr::core::vector3df(2, 3, 4), irr::core::vector3df(4, 5, 6)) == 47);

	// Should fail
	REQUIRE(Dot(irr::core::vector3df(1, 2, 3), irr::core::vector3df(4, 5, 6)) == 100);
}
