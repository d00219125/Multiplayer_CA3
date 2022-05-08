class ZombieClient : public Zombie
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new ZombieClient()); }

protected:
	ZombieClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};
