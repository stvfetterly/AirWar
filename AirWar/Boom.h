#pragma once
#include "VisibleGameObject.h"
#include "WeaponsManager.h"

class Boom : public VisibleGameObject
{
public:
	enum BoomType
	{
		BoomStandard = 0,
		BoomStandard2 = 1,
		BoomBigShort = 2,
		BoomBig = 3,
		BoomSmall = 4,
		TotalBoomTypes = 5
	};
	
	Boom(BoomType type);
	Boom();
	~Boom();

	void Update(const float& elapsedTime);

//	void SetPosition(const float& x, const float& y);
//	void Draw(sf::RenderWindow& renderWindow);

private:
	static int totalNumBooms;
	const static float ANIMATION_TIME;

	std::vector<std::string> _fileNameList;
	unsigned int _animationCellDisplayed;
	float _timeToChangeCell;
	float _rotation;

	void LoadBooms(const BoomType& type);

};