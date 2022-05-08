class SoundPlayer;

class SoundNode
{
public:
	explicit SoundNode(SoundPlayer& player);
	void PlaySound(SoundEffect sound, sf::Vector2f position);

	//virtual unsigned int GetCategory() const override;


private:
	SoundPlayer& m_sounds;
};