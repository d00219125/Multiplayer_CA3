#include "RoboCatPCH.hpp"

Zombie::Zombie()
{
	SetScale(GetScale() * 0.4f);
	SetCollisionRadius(20.f);
}


bool Zombie::HandleCollisionWithCat(RoboCat* inCat)
{
	(void)inCat;
	return false;
}

void Zombie::StopTrackingDeadPlayers()
{
	if (hasTarget && mTrackedPlayer->DoesWantToDie()) { hasTarget = false; }
}


void Zombie::Update()
{
	ProcessCollisions();
	float t = Timing::sInstance.GetDeltaTime();
	StopTrackingDeadPlayers();
	MoveTowardsTarget(t);
	
}


void Zombie::ProcessCollisions()
{


	float sourceRadius = GetCollisionRadius();
	Vector3 sourceLocation = GetLocation();

	//now let's iterate through the world and see what we hit...
	//note: since there's a small number of objects in our game, this is fine.
	//but in a real game, brute-force checking collisions against every other object is not efficient.
	//it would be preferable to use a quad tree or some other structure to minimize the
	//number of collisions that need to be tested.
	for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt)
	{
		GameObject* target = goIt->get();
		if (target != this && !target->DoesWantToDie())
		{
			//simple collision test for spheres- are the radii summed less than the distance?
			Vector3 targetLocation = target->GetLocation();
			float targetRadius = target->GetCollisionRadius();

			Vector3 delta = targetLocation - sourceLocation;
			float distSq = delta.LengthSq2D();
			float collisionDist = (sourceRadius + targetRadius);
			if (distSq < (collisionDist * collisionDist))
			{
				//first, tell the other guy there was a collision with a cat, so it can do something...

				if (target->HandleCollisionWithZombie(this))
				{
					//okay, you hit something!
					//so, project your location far enough that you're not colliding
					Vector3 dirToTarget = delta;
					dirToTarget.Normalize2D();
					Vector3 acceptableDeltaFromSourceToTarget = dirToTarget * collisionDist;
					//important note- we only move this cat. the other cat can take care of moving itself
					SetLocation(targetLocation - acceptableDeltaFromSourceToTarget);


					Vector3 relVel = mVelocity;

					//if other object is a cat, it might have velocity, so there might be relative velocity...
					Zombie* targetZombie = target->GetAsZombie();
					if (targetZombie)
					{
						relVel -= targetZombie->mVelocity;
					}

					//got vel with dir between objects to figure out if they're moving towards each other
					//and if so, the magnitude of the impulse ( since they're both just balls )
					float relVelDotDir = Dot2D(relVel, dirToTarget);

					if (relVelDotDir > 0.f)
					{
						Vector3 impulse = relVelDotDir * dirToTarget;

						if (targetZombie)
						{
							mVelocity -= impulse;
							mVelocity *= mCatRestitution;
						}
						else
						{
							mVelocity -= impulse * 2.f;
							mVelocity *= mWallRestitution;
						}

					}
				}
			}
		}
	}

}



void Zombie::SetTarget(RoboCat *r)
{
	//mTargetLocation = r->GetLocation();
	//hasTarget = true;
	//NetworkManagerServer
	//NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ZRS_Behaviour);
}

void Zombie::MoveTowardsTarget(float inDeltaTime)
{
	if (hasTarget)
	{

		//Vector3 GoalPosition = ; 
		//mVelocity += GoalPosition;
		Vector3 inputForwardDelta =  this->GetLocation() - mTrackedPlayer->GetLocation();
		//Vector3 thrustDirection = inputForwardDelta;
		//adjust by thrust
		Vector3 forwardVector = GetForwardVector();
		mVelocity = /*forwardVector **/ (inputForwardDelta * inDeltaTime *-.05/** mMaxLinearSpeed*/);
		SetLocation(GetLocation() + mVelocity );
	}
}

bool Zombie::GetHasTarget()
{
	return hasTarget;
}

uint32_t Zombie::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & ZRS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		inOutputStream.Write(GetRotation());


		writtenState |= ZRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & ZRS_Color)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(GetColor());

		writtenState |= ZRS_Color;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & ZRS_Behaviour)
	{
		inOutputStream.Write(true);

		//inOutputStream.Write(mTargetLocation.mX);
		//inOutputStream.Write(mTargetLocation.mY);
		inOutputStream.Write(mTrackedPlayer->GetPlayerId());
		inOutputStream.Write(hasTarget);

		writtenState |= ZRS_Behaviour;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}


	return writtenState;
}

void Zombie::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 location;
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);
		SetLocation(location);

		float rotation;
		inInputStream.Read(rotation);
		SetRotation(rotation);
	}


	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);
	}

	inInputStream.Read(stateBit);
	if ( stateBit)
	{
		//inInputStream.Read(mTargetLocation.mX);
		//inInputStream.Read(mTargetLocation.mY);
		
		int playerNetId = 0;
		inInputStream.Read(playerNetId);
		const auto& gameObjects = World::sInstance->GetGameObjects();
		for (GameObjectPtr r : gameObjects) 
		{
			if (r->GetAsCat()->GetPlayerId() == playerNetId) 
			{
				mTrackedPlayer = r->GetAsCat();
				break;
			}
		}
		
		inInputStream.Read(hasTarget);

	}
}


