#include "RoboCatPCH.hpp"
#include <math.h>
#include <iostream>

Yarn::Yarn() :
	mMuzzleSpeed(600.f),
	mVelocity(Vector3::Zero),
	mPlayerId(0)
{
	SetScale(GetScale() * 0.3f);
	SetCollisionRadius(20.f);
}

float Yarn::AngleShooting(Vector3 vec)
{
	if (vec.mX < 0) 
	{
		return RoboMath::ToRadians( 360) - (atan2(vec.mX,-vec.mY) * -1.0f);
	}
	else 
	{
		return  (atan2(vec.mX, -vec.mY));
	}


	//return 360 - RoboMath::ToDegrees((atan2(vec.mX, vec.mY) * si sgn(vec.mX)));
}



uint32_t Yarn::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & EYRS_Pose)
	{
		inOutputStream.Write((bool)true);

		//Player bullet spawn position
		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		Vector3 velocity = GetVelocity();
		inOutputStream.Write(velocity.mX);
		inOutputStream.Write(velocity.mY );

		inOutputStream.Write(GetRotation());

		writtenState |= EYRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & EYRS_Color)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(GetColor());

		writtenState |= EYRS_Color;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & EYRS_PlayerId)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(mPlayerId, 8);

		writtenState |= EYRS_PlayerId;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	return writtenState;
}



bool Yarn::HandleCollisionWithCat(RoboCat* inCat)
{
	(void)inCat;

	//you hit a cat, so look like you hit a cat
	return false;
}

bool Yarn::HandleCollisionWithZombie(Zombie* inZombie)
{
	(void)inZombie;

	return false;
}


void Yarn::InitFromShooter(RoboCat* inShooter)
{
	SetColor(inShooter->GetColor());
	SetPlayerId(inShooter->GetPlayerId());

	Vector3 forward = inShooter->GetForwardVector();
	SetVelocity(inShooter->GetVelocity() + forward * mMuzzleSpeed);
	//SetLocation(inShooter->GetLocation() + ( forward *mBulletOffset) );
	float angle = AngleShooting(forward);
	float bulletX = cos(angle * .5) *10;// *-100;//100 is half of player width
	float bulletY = sin(angle * .5) *10;// *-100;


	SetLocation(inShooter->GetLocation() + Vector3(bulletX+10, bulletY+10, 0)); 
	SetRotation(inShooter->GetRotation());
}

void Yarn::Update()
{

	float deltaTime = Timing::sInstance.GetDeltaTime();

	SetLocation(GetLocation() + mVelocity * deltaTime);


	//we'll let the cats handle the collisions
}

