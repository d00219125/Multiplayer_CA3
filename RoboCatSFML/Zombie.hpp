class Zombie : public GameObject
{
public:
	CLASS_IDENTIFICATION('ZOMB', GameObject)

	enum ZombieReplicationState
	{
		ZRS_Pose = 1 << 0,
		ZRS_Color = 1 << 1,
		ZRS_Behaviour = 1<<4,

		ZRS_AllState = ZRS_Pose | ZRS_Color   | ZRS_Behaviour
	};

	static	GameObject* StaticCreate() { return new Zombie(); }

	virtual uint32_t	GetAllStateMask()	const override { return ZRS_AllState; }

	virtual Zombie* GetAsZombie() override { return this; }

	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	virtual void		Read(InputMemoryBitStream& inInputStream) override;

	void		SetPlayerId(int inPlayerId) { mPlayerId = inPlayerId; }
	int			GetPlayerId() const { return mPlayerId; }

	void			SetVelocity(const Vector3& inVelocity) { mVelocity = inVelocity; }
	const Vector3& GetVelocity()						const { return mVelocity; }

	void Update() override;

	void ProcessCollisions();

	virtual void MoveTowardsTarget(float inDeltaTime);
	virtual void SetTarget(RoboCat *r);


	virtual bool GetHasTarget();

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	Zombie();

	int			mPlayerId;
	int			mHealth;

	Vector3				mVelocity;

	float				mWallRestitution;
	float				mCatRestitution;

	Vector3 mTargetLocation;
	RoboCat *mTrackedPlayer;
	float	mMaxLinearSpeed =30;
	bool hasTarget = false;
	
};
typedef shared_ptr< Zombie >	ZombiePtr;