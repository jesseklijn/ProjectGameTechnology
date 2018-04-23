//Needs to always be included and at the top
#include "stdafx.h"
//Needs to always be included
#include "catch.hpp"
//specific cpp file where the unit test is
#include "C:/Program Files (x86)/Jenkins/workspace/Terrors of the Deep/TerrorsOfTheDeep/TerrorsOfTheDeep/Fish.h"

TEST_CASE("Example: vectors", "[CalculateDistance]")
{

	REQUIRE(Fish::CalculateDistance(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, 0, 0)) == 5);
}

//to run the unit test, set this project as startup project and pres ctrl + f5

//see the ExampleUnitTest.cpp file in the TerrorsOfTheDeep project for reference