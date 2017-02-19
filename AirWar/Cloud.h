#pragma once
#include "VisibleGameObject.h"

class Cloud : public VisibleGameObject
{
public:
	enum CloudType
	{
		Dark,
		Med,
		Light
	};

	Cloud(CloudType type, float _opacity);
	Cloud();
	~Cloud();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);
	void SetYSpeedMultiplier(float multiplier) { _BGmultiplier = multiplier; }

private:
	static int totalNumClouds;

	CloudType _type;

	float _rotation;		//From 0 - 359
	float _opacity;			//From 0 - 1
	float _BGmultiplier;	//Controls how fast the cloud will go, compared to the background speed

	void Init();
};