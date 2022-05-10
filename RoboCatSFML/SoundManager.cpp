#include "RoboCatPCH.hpp"

std::unique_ptr< SoundManager >		SoundManager::sInstance;

void SoundManager::StaticInit()
{
	sInstance.reset(new SoundManager());
}

SoundManager::SoundManager()
{
	LoadSoundFromFile(shoot, shootB, "../Assets/Pistol.wav");
	LoadSoundFromFile(death, deathB, "../Assets/Splatter.wav");
	LoadSoundFromFile(music, musicB, "../Assets/MissionTheme.wav");
	LoadSoundFromFile(death2, death2B, "../Assets/death.wav");
	//LoadMusicFromFile(bgMusic, "../Assets/MissionTheme.wav");

}

void SoundManager::LoadSoundFromFile(sf::Sound& p_sound, sf::SoundBuffer& p_buffer, string p_file)
{
	if (p_buffer.loadFromFile(p_file))
	{
		p_sound.setBuffer(p_buffer);
		p_sound.setVolume(35);
	}
}

void SoundManager::LoadMusicFromFile(sf::Music& p_music, string p_file)
{
	p_music.openFromFile(p_file);
	p_music.setLoop(true);
	p_music.setVolume(20);
}

void SoundManager::PlayMusic()
{
	//bgMusic.play();
}

void SoundManager::PlaySound(SoundToPlay p_sound)
{
	

	switch (p_sound)
	{
	//case SoundManager::STP_Pickup:
	//	pickup.setRelativeToListener(true);
	//	pickup.play();
	//	break;
	case SoundManager::STP_Shoot:
		shoot.play();
		break;

	case SoundManager::STP_Music:
		music.play();
		break;
	case SoundManager::STP_Death:
		death.setRelativeToListener(true);
		death.play();
		break;

	case SoundManager::STP_Death2:
		death2.setRelativeToListener(true);
		death2.play();
		break;
	}
}

void SoundManager::PlaySoundAtLocation(SoundToPlay p_sound, sf::Vector3f p_location)
{
	switch (p_sound)
	{
	case SoundManager::STP_Shoot:
		shoot.setAttenuation(0.15);
		shoot.setPosition(p_location);
		shoot.play();
		break;
	}
}
