#include "stdafx.h"
#include "Boom.h"
#include "Game.h"
#include "VisibleGameObject.h"

const float Boom::ANIMATION_TIME = 0.05f;
int Boom::totalNumBooms = 0;

Boom::Boom(BoomType type) : _animationCellDisplayed(0), _timeToChangeCell(ANIMATION_TIME)
{
	LoadBooms(type);
}

Boom::Boom() : _animationCellDisplayed(0), _timeToChangeCell(ANIMATION_TIME)
{
	//C11 style random number generation
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, static_cast<float> (TotalBoomTypes));
	BoomType type = static_cast<BoomType>(static_cast<int>((dist(mt) - 1.0f)));

	LoadBooms(type);
}

Boom::~Boom()
{

}

void Boom::LoadBooms(const BoomType& type)
{
	//LOAD UP THE BOOMS!
	switch (type)
	{
	case BoomStandard:
		_fileNameList.push_back("images/Booms/Boom1.png");
		_fileNameList.push_back("images/Booms/Boom2.png");
		_fileNameList.push_back("images/Booms/Boom3.png");
		_fileNameList.push_back("images/Booms/Boom4.png");
		_fileNameList.push_back("images/Booms/Boom5.png");
		_fileNameList.push_back("images/Booms/Boom6.png");
		_fileNameList.push_back("images/Booms/Boom7.png");
		_fileNameList.push_back("images/Booms/Boom8.png");
		break;
	case BoomBigShort:
		_fileNameList.push_back("images/Booms/BoomB1.png");
		_fileNameList.push_back("images/Booms/BoomB2.png");
		_fileNameList.push_back("images/Booms/BoomB3.png");
		_fileNameList.push_back("images/Booms/BoomB4.png");
		_fileNameList.push_back("images/Booms/BoomB5.png");
		_fileNameList.push_back("images/Booms/BoomB6.png");
		break;
	case BoomSmall:
		_fileNameList.push_back("images/Booms/BoomC1.png");
		_fileNameList.push_back("images/Booms/BoomC2.png");
		_fileNameList.push_back("images/Booms/BoomC3.png");
		_fileNameList.push_back("images/Booms/BoomC4.png");
		_fileNameList.push_back("images/Booms/BoomC5.png");
		_fileNameList.push_back("images/Booms/BoomC6.png");
		_fileNameList.push_back("images/Booms/BoomC7.png");
		_fileNameList.push_back("images/Booms/BoomC8.png");
		_fileNameList.push_back("images/Booms/BoomC9.png");
		break;
	case BoomStandard2:
		_fileNameList.push_back("images/Booms/BoomD1.png");
		_fileNameList.push_back("images/Booms/BoomD2.png");
		_fileNameList.push_back("images/Booms/BoomD3.png");
		_fileNameList.push_back("images/Booms/BoomD4.png");
		_fileNameList.push_back("images/Booms/BoomD5.png");
		_fileNameList.push_back("images/Booms/BoomD6.png");
		_fileNameList.push_back("images/Booms/BoomD7.png");
		_fileNameList.push_back("images/Booms/BoomD8.png");
		_fileNameList.push_back("images/Booms/BoomD9.png");
		_fileNameList.push_back("images/Booms/BoomD10.png");
		_fileNameList.push_back("images/Booms/BoomD11.png");
	case BoomBig:
		_fileNameList.push_back("images/Booms/BoomE1.png");
		_fileNameList.push_back("images/Booms/BoomE2.png");
		_fileNameList.push_back("images/Booms/BoomE3.png");
		_fileNameList.push_back("images/Booms/BoomE4.png");
		_fileNameList.push_back("images/Booms/BoomE5.png");
		_fileNameList.push_back("images/Booms/BoomE6.png");
		_fileNameList.push_back("images/Booms/BoomE7.png");
		_fileNameList.push_back("images/Booms/BoomE8.png");
		_fileNameList.push_back("images/Booms/BoomE9.png");
		_fileNameList.push_back("images/Booms/BoomE10.png");
		break;
	}

	Load(_fileNameList[_animationCellDisplayed]);
	assert(IsLoaded());

	//Figure out what the center of the boom is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

/*	//Set rotation of the boom randomly
	srand(static_cast<unsigned int>(time(NULL)));
	_rotation = static_cast<float>(std::rand() % 359);
	GetSprite().setRotation(_rotation);*/

	//C11 style random number generation
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, 359.9f);
	_rotation = dist(mt);
	GetSprite().setRotation(_rotation);


	totalNumBooms++;
	_name = "ZBoom" + std::to_string(totalNumBooms);
}

void Boom::Update(const float& elapsedTime)
{
	//Update the animation cell by cell
	if (_timeToChangeCell <= 0.0)
	{
		_animationCellDisplayed++;
		if (_animationCellDisplayed < _fileNameList.size())
		{
			Load(_fileNameList[_animationCellDisplayed]);
		}


		//Keep all sprite rotation the same randomly generated number
		GetSprite().setRotation(_rotation);

		//Figure out what the center of the plane is, and use the X and Y of this position as the origin
		GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
			GetSprite().getGlobalBounds().height / 2);
		assert(IsLoaded());
		_timeToChangeCell = ANIMATION_TIME;
	}
	else
	{
		_timeToChangeCell -= elapsedTime;
	}

	//Delete the boom once it has run it's course
	if (_animationCellDisplayed >= _fileNameList.size())
	{
		Game::GetGameObjectManager().AddToDeletionDecorationQueue(_name);
	}

	//Update y velocity so that the booms scroll down the screen based on background speed
	SetYVelocity(10 * elapsedTime);

	//Update location
	GetSprite().move(GetXVelocity() * elapsedTime, GetYVelocity() * elapsedTime);
}