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
	//kill yourself!
	SetDoesWantToDie(true);

	ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	return false;
}






