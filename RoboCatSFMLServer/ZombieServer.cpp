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

	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
}




