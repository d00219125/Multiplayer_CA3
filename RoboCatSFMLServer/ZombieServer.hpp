class ZombieServer : public Zombie
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new ZombieServer()); }
	void HandleDying() override;

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

	void TakeDamage(int inDamagingZombie);

	void SetTarget(RoboCat* r) override;

	void MoveTowardsTarget(float time) override;

	void StopTrackingDeadPlayers();

protected:
	ZombieServer();
};

