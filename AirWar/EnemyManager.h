#pragma once
#include "Aircraft.h"

class EnemyManager
{
public:
	enum PlaneType
	{
		AS2,
		Dornier335,
		F15blue,
		F15navy,
		F22b,
		F35b,
		FuturePlane,
		SBJ,
		SU27,
		SU27b,
		SU30,
		SU47,
		Zero
	};
	
	void CreateWave(int numPlanes);

	static Aircraft* CreateEnemyPlane(PlaneType type);


};