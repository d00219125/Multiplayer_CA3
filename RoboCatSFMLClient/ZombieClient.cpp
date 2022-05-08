#include "RoboCatClientPCH.hpp"

ZombieClient::ZombieClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("Zombie"));
}
