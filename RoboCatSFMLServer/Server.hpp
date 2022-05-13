#include <iostream>
#include <fstream>
class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient(ClientProxyPtr inClientProxy);
	void HandleLostClient(ClientProxyPtr inClientProxy);

	RoboCatPtr	GetCatForPlayer(int inPlayerId);
	void	SpawnCatForPlayer(int inPlayerId);

	void SetZombieTarget();

private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();

	bool PeopleIn = false;
	float timePassed;
	float timeSinceLastSpawn;

	std::ofstream file;
	std::ofstream tegst;
	//std::vector<RoboCat*> players;
	//std::vector<Zombie*> zombies;
	//sf::Clock delay;
	//sf::Time time;

};

