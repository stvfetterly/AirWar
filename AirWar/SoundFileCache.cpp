#include "stdafx.h"
#include "SoundFileCache.h"

std::map<std::string, sf::SoundBuffer*> SoundFileCache::_sounds;
std::map<std::string, sf::Music*> SoundFileCache::_music;

SoundFileCache::SoundFileCache(void)
{
	//Load commonly used sounds into cache upon creation to avoid delays during gameplay
	AddSound("sounds/CasingSound3.wav");
	AddSound("sounds/Explosion.wav");
	AddSound("sounds/Explosion1.wav");
	AddSound("sounds/Explosion2.wav");
	AddSound("sounds/Explosion3.wav");
	AddSound("sounds/Explosion4.wav");
	AddSound("sounds/ExplosionDouble.wav");
	AddSound("sounds/ExplosionwPlaneNoise.wav");
	AddSound("sounds/MultiShot.wav");
	AddSound("sounds/MultiShot3.wav");
	AddSound("sounds/Multishot5.wav");
	AddSound("sounds/Multishot6.wav");
	AddSound("sounds/Multishot8.wav");
	AddSound("sounds/MultishotB.wav");
	AddSound("sounds/MultiShotLaser6.wav");
	AddSound("sounds/MultiShotMinigun.wav");
	AddSound("sounds/PlaneNoise.wav");
	AddSound("sounds/PlaneNoise2.wav");
	AddSound("sounds/PlaneNoise3.wav");
	AddSound("sounds/PlaneNoise4.wav");
	AddSound("sounds/PlaneNoise5.wav");
	AddSound("sounds/PlaneNoise6.wav");
	AddSound("sounds/PlaneNoise7.wav");
	AddSound("sounds/PlaneNoise8.wav");
	AddSound("sounds/PlaneNoiseSquad.wav");
	AddSound("sounds/RadioStatic.wav");
	AddSound("sounds/RadioTune.wav");
	AddSound("sounds/Reload1.wav");
	AddSound("sounds/Reload2.wav");
	AddSound("sounds/Rocket.wav");
	AddSound("sounds/Rocket2.wav");
	AddSound("sounds/Rocket3.wav");
	AddSound("sounds/SingleShot.wav");
	AddSound("sounds/SingleShot2.wav");
	AddSound("sounds/SingleShot3.wav");
	AddSound("sounds/SingleShot4.wav");
	AddSound("sounds/SingleShotLaser.wav");
}

//Delete all the stuff we've created and saved in the maps
SoundFileCache::~SoundFileCache(void)
{
	std::for_each(_sounds.begin(), _sounds.end(), Deallocator<sf::SoundBuffer*>());
	std::for_each(_music.begin(), _music.end(), Deallocator<sf::Music*>());
}

//Adds a new sound to the cache
void SoundFileCache::AddSound(const std::string& soundName) const
{
	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	if (!soundBuffer->loadFromFile(soundName))
	{
		//sound file can't be found, that's a problem
		delete soundBuffer;
		throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSound");
	}

	//Sound was found, add it to the map
	std::map<std::string, sf::SoundBuffer*>::iterator res =
		_sounds.insert(std::pair<std::string, sf::SoundBuffer*>(soundName, soundBuffer)).first;

	sf::Sound sound;
	sound.setBuffer(*soundBuffer);
}

sf::Sound SoundFileCache::GetSound(const std::string& soundName) const
{
	std::map<std::string, sf::SoundBuffer*>::iterator itr = _sounds.find(soundName);

	//Check if the sound is already in the map.  If it is missing we add it to the map and return it.
	if (itr == _sounds.end())
	{
		AddSound(soundName);

		//Find the newly added sound
		itr = _sounds.find(soundName);

		//Return it
		sf::Sound sound;
		sound.setBuffer(*itr->second);
		return sound;
	}
	else    //return the sound
	{
		sf::Sound sound;
		sound.setBuffer(*itr->second);
		return sound;
	}

	//Should never get here
	throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSound");
}

sf::Music* SoundFileCache::GetSong(const std::string& soundName) const
{
	std::map<std::string, sf::Music*>::iterator itr = _music.find(soundName);

	//Check if the song is already in the map.  If it is, we return it.  Otherwise we add it to the map and return it.
	if (itr == _music.end())
	{
		sf::Music* song = new sf::Music();
		if (!song->openFromFile(soundName))
		{
			//If the song can't be found, we have a problem
			delete song;
			throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSong");
		}
		else
		{
			//Song found, add it to the music map
			std::map<std::string, sf::Music*>::iterator res =
				_music.insert(std::pair<std::string, sf::Music*>(soundName, song)).first;
			return res->second;
		}
	}
	else
	{
		return itr->second;
	}

	//Should never get here
	throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSong");
}