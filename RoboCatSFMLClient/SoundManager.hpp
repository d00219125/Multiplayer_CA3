class SoundManager
{
public:
	static void StaticInit();

	static std::unique_ptr<SoundManager>		sInstance;

	SoundPtr	GetSound(const string& inSoundName);

private:
	SoundManager();

	bool CacheSound(string inName, const char* inFileName);

	unordered_map< string, SoundPtr >	mSoundMap;
};