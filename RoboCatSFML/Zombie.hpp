class Zombie : public GameObject
{
public:
	CLASS_IDENTIFICATION('ZOMB', GameObject)

	enum EZombieReplicationState
	{
		EMRS_Pose = 1 << 0,
		EMRS_Color = 1 << 1,
		EYRS_PlayerId = 1 << 2,
		ECRS_Health = 1 << 1,

		EMRS_AllState = EMRS_Pose | EMRS_Color | EYRS_PlayerId | ECRS_Health
	};

	static	GameObject* StaticCreate() { return new Zombie(); }

	virtual uint32_t	GetAllStateMask()	const override { return EMRS_AllState; }

	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	virtual void		Read(InputMemoryBitStream& inInputStream) override;

	void		SetPlayerId(int inPlayerId) { mPlayerId = inPlayerId; }
	int			GetPlayerId() const { return mPlayerId; }

	void			SetVelocity(const Vector3& inVelocity) { mVelocity = inVelocity; }
	const Vector3& GetVelocity()						const { return mVelocity; }

	void Update() override;

	void ProcessCollisions();

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	Zombie();

	int			mPlayerId;
	int			mHealth;

private:

	Vector3				mVelocity;

	float				mWallRestitution;
	float				mCatRestitution;
};

