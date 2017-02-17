#pragma once
#include "AircraftEnemy.h"

class PlaneManager
{
public:
	enum PlaneType
	{
		Enemy_AS2,
		Enemy_Dornier335,
		Enemy_F15blue,
		Enemy_F15navy,
		Enemy_F22b,
		Enemy_F35b,
		Enemy_FuturePlane,
		Enemy_SBJ,
		Enemy_SU27,
		Enemy_SU27b,
		Enemy_SU30,
		Enemy_SU47,
		Enemy_Zero,
		Player_J31,
		Player_J20,
		Player_T50,
		Player_F22,
		Player_F35
	};
	
	static void CreateWave(PlaneType type);

	static Aircraft* CreatePlane(PlaneType type);

private:
	static void StraightTop(PlaneType type, float numberOfPlanes);
	static void StaggeredTop(PlaneType type, float numberOfPlanes);
	static void StaggeredRight(PlaneType type, float numberOfPlanes);
	static void StaggeredLeft(PlaneType type, float numberOfPlanes);
};