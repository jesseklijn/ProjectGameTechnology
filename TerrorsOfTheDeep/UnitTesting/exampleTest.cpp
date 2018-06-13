////Needs to always be included and at the top
#include "stdafx.h"
////Needs to always be included
#include "catch.hpp"
////specific cpp file where the unit test is
#include "../TerrorsOfTheDeep/UnitTests.cpp"

/// Test case for drawing triangles in the GridMesh class
TEST_CASE("GridMesh: DrawTriangles", "[Array<u16>]")
{
	// Array contains the numbers for drawing 2 triangles (1 quad)
	irr::core::array<irr::u16> arrayTriangles;
	arrayTriangles.push_back(0);
	arrayTriangles.push_back(2);
	arrayTriangles.push_back(1);

	arrayTriangles.push_back(1);
	arrayTriangles.push_back(2);
	arrayTriangles.push_back(3);

	// Should return null 
	REQUIRE(DrawTriangles(0, 0) == NULL);

	// Testing if 2 triangles (1 quad) are drawn in the function
	REQUIRE(DrawTriangles(1,1) == arrayTriangles);
}

/// Note: SColor makes use of (A,R,G,B) with values between 0 and 255
/// A is alpha, 
/// R is red, 
/// G is green, 
/// B is blue
TEST_CASE("NoiseGenerator: GetPixelColor", "[SColor]")
{
	// Should return null (black SColor value) 
	REQUIRE(GetPixelColor(driverUnitTest->getTexture("../media/heightMapUnitTest255.bmp"),-1,-1) == NULL);

	// Should return a black color (255, 0,0,0,0)
	REQUIRE(GetPixelColor(driverUnitTest->getTexture("../media/heightMapUnitTest000.bmp"), 1, 1) == SColor(255, 0, 0, 0));

	// Should return a grey color (255, 255,127,127)
	REQUIRE(GetPixelColor(driverUnitTest->getTexture("../media/heightMapUnitTest127.bmp"), 1, 1) == SColor(255,127,127,127));

	// Should return a white color (255,255,255,255)
	REQUIRE(GetPixelColor(driverUnitTest->getTexture("../media/heightMapUnitTest255.bmp"), 1, 1) == SColor(255, 255, 255, 255));
}

TEST_CASE("GridMesh: GenerateGridSize", "[Bool]")
{
	// parameter size is negative so it should return false
	REQUIRE(GenerateGridSize(-1, -1, -1) == false);

	// parameter size is positive so it should return true
	REQUIRE(GenerateGridSize(1, 1, 1) == true);
}
//to run the unit test, set this project as startup project and pres ctrl + f5
//see the ExampleUnitTest.cpp file in the TerrorsOfTheDeep project for reference