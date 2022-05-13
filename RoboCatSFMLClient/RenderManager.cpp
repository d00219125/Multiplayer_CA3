#include "RoboCatClientPCH.hpp"

std::unique_ptr< RenderManager >	RenderManager::sInstance;

RenderManager::RenderManager() : mSprite()
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	//view.setCenter()
	WindowManager::sInstance->setView(view);
	mGameOver.setTexture(*TextureManager::sInstance->GetTexture("gameOver"));

	TexturePtr texture = TextureManager::sInstance->GetTexture("bg");
	auto tSize = texture->getSize();
	mSprite.setTexture(*texture);
	mSprite.setOrigin(tSize.x / 2, tSize.y / 2);

	//sf::IntRect rect = sf::IntRect(1, 1, tSize.x, tSize.y);
	//mSprite.setTextureRect(rect);

	mSprite.setPosition(960, 544);
}

//Center camera on player

//void RenderManager::UpdateView()
//{
//
//	float rate = .02f;
//	if (FindCatCentre() != sf::Vector2f(-1, -1))
//	{
//		sf::Vector2f player = FindCatCentre();
//		sf::Vector2f newCentre = view.getCenter() + ((player - view.getCenter()) * rate);
//		view.setCenter(newCentre);
//	}
//	WindowManager::sInstance->setView(view);
//}
//
//
//sf::Vector2f RenderManager::FindCatCentre()
//{
//	uint32_t catID = (uint32_t)'RCAT';
//	for (auto obj : World::sInstance->GetGameObjects())
//	{
//		// Find a cat.
//		if (obj->GetClassId() == catID)
//		{
//			RoboCat* cat = dynamic_cast<RoboCat*>(obj.get());
//			auto id = cat->GetPlayerId();
//			auto ourID = NetworkManagerClient::sInstance->GetPlayerId();
//			if (id == ourID)
//			{
//
//				auto centre = cat->GetLocation();
//				m_lastCatPos.x = centre.mX;
//				m_lastCatPos.y = centre.mY;
//				return sf::Vector2f(centre.mX, centre.mY);
//			}
//		}
//	}
//	return sf::Vector2f(-1, -1);
//}

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

void RenderManager::EndGame()
{
	sf::Vector2f gameOver(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2);
	WindowManager::sInstance->draw(mGameOver);
	WindowManager::sInstance->display();
	HUD::sInstance->WriteScoresToFile();
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
