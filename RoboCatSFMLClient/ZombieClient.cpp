#include "RoboCatClientPCH.hpp"

void ZombieClient::Update()
{
	//const Move* pendingMove = InputManager::sInstance->GetAndClearPendingMove();

}

ZombieClient::ZombieClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("Zombie"));
}
