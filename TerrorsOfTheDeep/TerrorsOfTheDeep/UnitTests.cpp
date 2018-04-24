

#include <vector3d.h>

int UnitTestExample(int testVar) 
{
	return testVar * 2;
}

float CalculateDistance(irr::core::vector3df A, irr::core::vector3df B)
{
	//distance is (b.x-a.x)^2 + (b.y-a.y)^2
	auto distX = (B.X - A.X) * (B.X - A.X);
	auto distY = (B.Y - A.Y) * (B.Y - A.Y);
	auto distZ = (B.Z - A.Z) * (B.Z - A.Z);
	auto lengthRoot = distX + distY + distZ;


	return sqrt(lengthRoot);

}