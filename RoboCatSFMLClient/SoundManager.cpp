#include "RoboCatClientPCH.hpp"

std::unique_ptr< SoundManager >		SoundManager::sInstance;

void SoundManager::StaticInit()
{
	sInstance.reset(new SoundManager());
}

SoundManager::SoundManager()
{
	CacheSound("cat", "../Assets/man.png");
	CacheSound("Zombie", "../Assets/zombie.png");
	CacheSound("yarn", "../Assets/bullet.png");
	CacheSound("bg", "../Assets/bg.png");
}

SoundPtr	SoundManager::GetSound(const string& inSoundName)
{
	return mSoundMap[inSoundName];
}

bool SoundManager::CacheSound(string inSoundName, const char* inFileName)
{
	SoundPtr newSound(new sf::Sound());
	if (!newSound->loadFromFile(inFileName))
	{
		return false;
	}

	mSoundMap[inSoundName] = newSound;

	return true;

}
