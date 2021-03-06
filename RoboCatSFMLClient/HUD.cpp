#include "RoboCatClientPCH.hpp"

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
	mScoreBoardOrigin(40.f, 10.f, 0.0f),
	mBandwidthOrigin(400.f, 10.f, 0.0f),
	mRoundTripTimeOrigin(980.f, 10.f, 0.0f),
	mScoreOffset(0.f, 50.f, 0.0f),
	mHealthOffset(1700, 10.f, 0.0f),
	mAmmoOffset(1700, 50.f, 0.0f),
	mHealth(0),
	mAmmo(0)
{
}


void HUD::StaticInit()
{
	sInstance.reset(new HUD());
}

void HUD::Render()
{
	RenderBandWidth();
	RenderRoundTripTime();
	RenderScoreBoard();
	RenderHealth();
	//RenderAmmo();
	//WriteScoresToFile();
	DisplayScore();
}

void HUD::RenderHealth()
{
	if (mHealth > 0)
	{
		string healthString = StringUtils::Sprintf("Health %d", mHealth);
		RenderText(healthString, mHealthOffset, Colors::Red);
	}

	//TexturePtr texture = TextureManager::sInstance->GetTexture("heart");
	//auto tSize = texture->getSize();
	//mHeart.setTexture(*texture);
	//mHeart.setOrigin(tSize.x / 2, tSize.y / 2);

	//mHeart.setPosition(0, 0);
}

void HUD::RenderAmmo()
{
		string ammoString = StringUtils::Sprintf("Ammo %d", mAmmo);
		RenderText(ammoString, mAmmoOffset, Colors::Green);
}

void HUD::RenderBandWidth()
{
	string bandwidth = StringUtils::Sprintf("In %d  Bps, Out %d Bps",
		static_cast<int>(NetworkManagerClient::sInstance->GetBytesReceivedPerSecond().GetValue()),
		static_cast<int>(NetworkManagerClient::sInstance->GetBytesSentPerSecond().GetValue()));
	RenderText(bandwidth, mBandwidthOrigin, Colors::White);
}

void HUD::RenderRoundTripTime()
{
	float rttMS = NetworkManagerClient::sInstance->GetAvgRoundTripTime().GetValue() * 1000.f;

	string roundTripTime = StringUtils::Sprintf("RTT %d ms", (int)rttMS);
	RenderText(roundTripTime, mRoundTripTimeOrigin, Colors::White);
}

void HUD::RenderScoreBoard()
{
	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	Vector3 offset = mScoreBoardOrigin;
	
	for (const auto& entry : entries)
	{
		RenderText(entry.GetFormattedNameScore(), offset, entry.GetColor());
		offset.mX += mScoreOffset.mX;
		offset.mY += mScoreOffset.mY;
	}

}

void HUD::RenderText(const string& inStr, const Vector3& origin, const Vector3& inColor)
{
	sf::Text text;
	text.setString(inStr);
	text.setFillColor(sf::Color(inColor.mX, inColor.mY, inColor.mZ, 255));
	text.setCharacterSize(50);
	text.setPosition(origin.mX, origin.mY);
	text.setFont(*FontManager::sInstance->GetFont("carlito"));
	WindowManager::sInstance->draw(text);
}

void HUD::WriteScoresToFile()
{
	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	Vector3 offset = mScoreBoardOrigin;
	file.open("../Assets/txt.txt");
	for (const auto& entry : entries)
	{
		file << 	entry.GetFormattedNameScore() << "\n";
	}
	file.close();

	
}

void HUD::DisplayScore()
{
	std::string s, highScorer;
	int highScore = 0;
	fileIn.open("../Assets/txt.txt");
	while (std::getline(fileIn, s)) 
	{
		int whitespaceIndex = s.find_first_of(' ');
		int i = std::stoi(s.substr(whitespaceIndex+1)) ;
		if (i > highScore) 
		{
			highScorer = s;
		}
	}
	fileIn.close();
	string hs = "Last score: " + highScorer;
	RenderText(hs, Vector3(1500, 60, 0), Vector3(255, 255, 255));
	//RenderText(highScorer, Vector3(1700, 60, 0), Vector3(255, 255, 255));
}

