#pragma once
#include "VisibleGameObject.h"

class Cloud : public VisibleGameObject
{
public:
	enum CloudType
	{
		Big,
		Small
	};

	Cloud(CloudType type, float _opacity);
	Cloud();
	~Cloud();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);
	void SetYSpeedMultiplier(float multiplier) { _BGmultiplier = multiplier; }

private:
	static int totalNumClouds;
	static const float CONTRAIL_DEL_TIME;

	CloudType _type;

	float _opacity;			//From 0 - 1
	float _BGmultiplier;	//Controls how fast the cloud will go, compared to the background speed
	float _contrailDeletionTime;	//Amount of time before a contrail is deleted

	void Init();
};