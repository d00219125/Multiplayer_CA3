class SoundPlayer : private sf::NonCopyable
{
public:
	SoundPlayer();

	void Play(SoundEffect effect);
	void Play(SoundEffect effect, sf::Vector2f position);

	void RemoveStoppedSounds();
	void SetListenerPosition(sf::Vector2f position);
	sf::Vector2f GetListenerPosition() const;


private:
	sf::SoundBuffer m_sound_buffers;
	//SoundBufferHolder m_sound_buffers;
	std::list<sf::Sound> m_sounds;
};
