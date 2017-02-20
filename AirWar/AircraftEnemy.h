#pragma once
#include "Aircraft.h"

class AircraftEnemy : public Aircraft
{
public:
	enum AI_Type
	{
		LEFT_AND_RIGHT,
		UP_AND_DOWN,
		DIAGONAL_BACK_AND_FORTH,
		PEEKABOO_L,
		PEEKABOO_R,
		PEEKABOO_T,
		PEEKABOO_B,
		SAW_SHAPED_LR,
		SAW_SHAPED_UD
	};

	AircraftEnemy(const std::string& image, float maxVelocity, float mass, float health, WeaponsManager::WeaponType weaponType, WeaponsManager::WeaponType weaponType2, AI_Type type);
	~AircraftEnemy();
	void Update(const float& elapsedTime);			//Updates plane position
	void Damage(const float& damageAmount);

private:
	enum PeekABooType{TOP, BOTTOM, LEFT, RIGHT};

	void MoveUpAndDown(int topEdge, int bottomEdge);
	void MoveLeftAndRight(int leftEdge, int rightEdge);
	void PeekABoo(PeekABooType peekType);

	void EnforceSpeedLimit();

	static int numEnemies;
	void FiringRules(const float& elapsedTime);		//Rules for firing weapon
	AI_Type _AI_type;

	bool b_goUp;
	bool b_goRight;
};