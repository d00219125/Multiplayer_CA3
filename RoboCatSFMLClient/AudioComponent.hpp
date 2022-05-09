typedef shared_ptr< sf::Sound > SoundPtr;

class AudioComponent
{
public:

	AudioComponent(GameObject* inGameObject);
	~AudioComponent();


	void Setsound(SoundPtr insound);
	//virtual sf::audio& Getaudio();



protected:

	//sf::audio m_audio;

	//don't want circular reference...
	//GameObject* mGameObject;
};

typedef shared_ptr< AudioComponent >	AudioComponentPtr;
