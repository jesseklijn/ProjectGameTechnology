//Needs to always be included and at the top
#include "stdafx.h"
//Needs to always be included
#include "catch.hpp"
//specific cpp file where the unit test is
#include "../TerrorsOfTheDeep/Fish.cpp"
#include "../TerrorsOfTheDeep/Fish.h"



//unit test
TEST_CASE("Example: ints are multiplied", "[Fish]")
{
	REQUIRE(Fish::CalculateDistance(vector3df(0, 0, 0), vector3df(0, 0, 0)) == 5);
	
}

//to run the unit test, set this project as startup project and pres ctrl + f5

//see the ExampleUnitTest.cpp file in the TerrorsOfTheDeep project for reference