#pragma once
#include "stdafx.h"

class BackgroundManager
{
	
public:
	BackgroundManager();
	~BackgroundManager();

	static int SPEED_OF_BACKGROUND_IMAGE;
	const static int NUM_BACKGROUNDS = 5;
	enum Backgrounds { NEW_YORK, OAHU, LOUISIANA, KANSAS, SIBERIA };

	static void LoadTextures();
	void Update(const float& timeChange);
	void SetBackground(Backgrounds newBackground) { _type = newBackground; }
	void GenerateClouds(const float& timeChange);

private:
	static sf::Texture _backgroundImage[NUM_BACKGROUNDS];
	static sf::Sprite _background;
	static sf::Sprite _backgroundInv;

	static const float TIME_BETWEEN_CLOUDS;
	static float _cloudTimer;
	static float _backgroundImagePixels;
	static bool _backgroundInverted;
	
	Backgrounds _type;
};