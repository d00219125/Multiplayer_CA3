class ZombieServer : public Zombie
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new ZombieServer()); }
	void HandleDying() override;

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	ZombieServer();
};

