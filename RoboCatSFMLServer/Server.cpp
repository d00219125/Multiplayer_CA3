
#include "RoboCatServerPCH.hpp"
#include <iostream>



//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	s_instance.reset(new Server());

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('ZOMB', ZombieServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnServer::StaticCreate);

	InitNetworkManager();

	// Setup latency
	float latency = 0.0f;
	string latencyString = StringUtils::GetCommandLineArg(2);
	if (!latencyString.empty())
	{
		latency = stof(latencyString);
	}
	NetworkManagerServer::sInstance->SetSimulatedLatency(latency);
	timePassed = 0;
	timeSinceLastSpawn = 0;
}


int Server::Run()
{
	SetupWorld();
	//delay.restart();
	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg(1);
	uint16_t port = stoi(portString);

	return NetworkManagerServer::StaticInit(port);
}


namespace
{

	void CreateRandomMice(int inZombieCount)
	{
		Vector3 ZombieMin(100.f, 100.f, 0.f);
		Vector3 ZombieMax(1180.f, 620.f, 0.f);
		GameObjectPtr go;

		//make a Zombie somewhere- where will these come from?
		for (int i = 0; i < inZombieCount; ++i)
		{
			go = GameObjectRegistry::sInstance->CreateGameObject('ZOMB');
			Vector3 ZombieLocation = RoboMath::GetRandomVector(ZombieMin, ZombieMax);
			go->SetLocation(ZombieLocation);
		}
		
	}
}


void Server::SetupWorld()
{
	//spawn some random mice
	//CreateRandomMice(10);
	//SetZombieTarget();
	file.open("../Assets/txt.txt");
	file << "WriteThis\n";
	file.close();
	//spawn more random mice!
	//CreateRandomMice(10);
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

	
	timePassed += Timing::sInstance.GetDeltaTime();
	timeSinceLastSpawn += Timing::sInstance.GetDeltaTime();
	if (timePassed > 45 && (int)timeSinceLastSpawn > 15 )
	{
		timeSinceLastSpawn = 0;
		CreateRandomMice(6);
		SetZombieTarget();
	}
}

void Server::HandleNewClient(ClientProxyPtr inClientProxy)
{

	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnCatForPlayer(playerId);
	//CreateRandomMice(10);
}

void Server::SpawnCatForPlayer(int inPlayerId)
{
	RoboCatPtr cat = std::static_pointer_cast<RoboCat>(GameObjectRegistry::sInstance->CreateGameObject('RCAT'));
	cat->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerId)->GetColor());
	cat->SetPlayerId(inPlayerId);
	//gotta pick a better spawn location than this...
	cat->SetLocation(Vector3(600.f - static_cast<float>(inPlayerId), 400.f, 0.f));
	//SetZombieTarget();

}

void Server::SetZombieTarget()
{
//	ZombiePtr zombie = std::static_pointer_cast<Zombie>();
		std::vector<RoboCat*> players;
		std::vector<Zombie*> zombies;
		const auto & gameObjects = World::sInstance->GetGameObjects();
		//populate the Vectors with zombies and players
		for (int i = 0; i < gameObjects.size(); ++i) 
		{
			GameObjectPtr go = gameObjects[i];
			Zombie* zom = go->GetAsZombie();
			if (zom) 
			{
				zombies.push_back(zom);
			}
			RoboCat* player = go->GetAsCat();
			if (player) 
			{
				players.push_back(player);
			}
		}
		//assigns player as zombies target
		for (int i = 0; i < zombies.size(); i++)
		{
			if (players.size() > 0) {
				int choice = rand() % players.size();
				RoboCat* player = players[choice];
				zombies[i]->SetTarget(player);
				LOG("Zombie Targeted ", 0);
			}
		}
}

void Server::GameEnd()
{
	LOG("END", 0);
}

void Server::HandleLostClient(ClientProxyPtr inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	RoboCatPtr cat = GetCatForPlayer(playerId);
	if (cat)
	{
		cat->SetDoesWantToDie(true);
	}
}

RoboCatPtr Server::GetCatForPlayer(int inPlayerId)
{
	//run through the objects till we find the player...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (int i = 0, c = gameObjects.size(); i < c; ++i)
	{
		GameObjectPtr go = gameObjects[i];
		RoboCat* cat = go->GetAsCat();
		if (cat && cat->GetPlayerId() == inPlayerId)
		{
			return std::static_pointer_cast<RoboCat>(go);
		}
	}
	SetZombieTarget();
	return nullptr;

}
