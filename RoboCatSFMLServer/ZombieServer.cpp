#include "RoboCatServerPCH.hpp"


ZombieServer::ZombieServer()
{
}

void ZombieServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool ZombieServer::HandleCollisionWithCat(RoboCat* inCat)
{
	if (inCat->GetPlayerId() != GetPlayerId())
	{
		//kill yourself!
		SetDoesWantToDie(true);

		static_cast<RoboCatServer*>(inCat)->TakeDamage(GetPlayerId());

	}


	return false;
}


void ZombieServer::TakeDamage(int inDamagingZombie)
{
	mHealth--;
	if (mHealth <= 0.f)
	{
		ScoreBoardManager::sInstance->IncScore(inDamagingZombie, 1);

		SetDoesWantToDie(true);


	}

	//static_cast<ZombieServer*>(inCat)->TakeDamage(GetPlayerId());

	//NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ZRS_Health);
}

void ZombieServer::SetTarget(RoboCat* r)
{
	mTargetLocation = r->GetLocation();
	hasTarget = true;
	mTrackedPlayer = r;
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ZRS_Behaviour);
	
}


void ZombieServer::MoveTowardsTarget(float time)
{
	Vector3 oldPos = GetLocation();
	Zombie::MoveTowardsTarget(time);
	if (!RoboMath::Is2DVectorEqual(oldPos, GetLocation())) 
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ZRS_Pose);
	}
}

