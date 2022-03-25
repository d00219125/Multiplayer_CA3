#pragma once
class PlayerSpriteComponent : public SpriteComponent
{
public:
	PlayerSpriteComponent(GameObject* inGameObject);
	sf::Sprite& GetSprite();
};
typedef shared_ptr<PlayerSpriteComponent >	PlayerSpriteComponentPtr;

