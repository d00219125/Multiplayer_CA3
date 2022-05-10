class ZombieClient : public Zombie
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new ZombieClient()); }
	virtual void Update();
protected:
	ZombieClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};
