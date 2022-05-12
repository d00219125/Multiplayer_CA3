#include "RoboCatClientPCH.hpp"

std::unique_ptr< RenderManager >	RenderManager::sInstance;

RenderManager::RenderManager() : mSprite()
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	//view.setCenter()
	WindowManager::sInstance->setView(view);

	TexturePtr texture = TextureManager::sInstance->GetTexture("bg");
	auto tSize = texture->getSize();
	mSprite.setTexture(*texture);
	mSprite.setOrigin(tSize.x / 2, tSize.y / 2);

	//sf::IntRect rect = sf::IntRect(1, 1, tSize.x, tSize.y);
	//mSprite.setTextureRect(rect);

	mSprite.setPosition(960, 544);
}


void RenderManager::StaticInit()
{
	sInstance.reset(new RenderManager());
}


void RenderManager::AddComponent(SpriteComponent* inComponent)
{
	mComponents.emplace_back(inComponent);
}

void RenderManager::RemoveComponent(SpriteComponent* inComponent)
{
	int index = GetComponentIndex(inComponent);

	if (index != -1)
	{
		int lastIndex = mComponents.size() - 1;
		if (index != lastIndex)
		{
			mComponents[index] = mComponents[lastIndex];
		}
		mComponents.pop_back();
	}
}

int RenderManager::GetComponentIndex(SpriteComponent* inComponent) const
{
	for (int i = 0, c = mComponents.size(); i < c; ++i)
	{
		if (mComponents[i] == inComponent)
		{
			return i;
		}
	}

	return -1;
}


//this part that renders the world is really a camera-
//in a more detailed engine, we'd have a list of cameras, and then render manager would
//render the cameras in order
void RenderManager::RenderComponents()
{
	WindowManager::sInstance->draw(mSprite);
	//Get the logical viewport so we can pass this to the SpriteComponents when it's draw time
	for (SpriteComponent* c : mComponents)
	{	
		WindowManager::sInstance->draw(c->GetSprite());	
	}
}

void RenderManager::Render()
{
	//
	// Clear the back buffer
	//
	WindowManager::sInstance->clear(sf::Color(100, 149, 237, 255));

	RenderManager::sInstance->RenderComponents();

	HUD::sInstance->Render();

	//
	// Present our back buffer to our front buffer
	//
	WindowManager::sInstance->display();

}
