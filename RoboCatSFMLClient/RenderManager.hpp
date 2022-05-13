//I take care of rendering things!

class RenderManager
{
public:

	static void StaticInit();
	static std::unique_ptr< RenderManager >	sInstance;

	void Render();
	void RenderComponents();
	//void UpdateView();
	void EndGame();

	//vert inefficient method of tracking scene graph...
	void AddComponent(SpriteComponent* inComponent);
	void RemoveComponent(SpriteComponent* inComponent);
	int	 GetComponentIndex(SpriteComponent* inComponent) const;

	//sf::Vector2f FindCatCentre();

private:

	RenderManager();

	//this can't be only place that holds on to component- it has to live inside a GameObject in the world
	vector< SpriteComponent* >		mComponents;
	sf::Sprite mSprite;
	sf::View view;
	sf::Sprite mGameOver;

	float time;

	sf::Vector2f m_lastCatPos;
};


