#pragma once
#include "cocos2d.h"
#include "Singleton.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
struct MusicEffectInfo
{
	float delay;
	float currentTime;
	string filepath;
	MusicEffectInfo()
	{
		delay = 0;
		currentTime = 0;
	}
};
class GameMusicTools:public Singleton<GameMusicTools>
{
public:
	GameMusicTools();
	~GameMusicTools();
	void setMusicVolume(float volume);
	void setEffectVolume(float volume);
	void stopAllEffect();
	void stopBGMusic();
	void pauseBGM();
	void resumeBGM();
	void pauseAllEffect();
	void resumeAllEffect();
	void playBgMusic(const char* pszFilePath);
	void playSoundEffect(const char* pszFilePath, int nativeId = 0,bool isloop = false, float delayTime = 0.1);
	void stopSoundEffect(const char* pszFilePath ,int nativeId = 0);
	void SoundEffectControl(float delayTime);

	void preloadResource();
	void unloadResource();
	void addPreloadEffResource(const char* filepath);
	void addPreloadBgResource(const char* filepath);
private:
	void doPlayMusic(const char* musicpath, bool isloop = false);
	void doPlayEffect(const char* effectpath,int nativeId=0,bool isloop = false);
protected:
	map<unsigned int, MusicEffectInfo> soundCt_map;

	std::map<int,int> constantEffectIDs; //ѭ�����ŵ���ЧID



	unsigned int _HashStr(const char *key)
	{
		unsigned int len = strlen(key);
		const char *end = key + len;
		unsigned int hash;

		for (hash = 0; key < end; key++)
		{
			hash *= 16777619;
			hash ^= (unsigned int)(unsigned char)toupper(*key);
		}
		return (hash);
	}

private:
	std::vector<std::string> preloadEffFiles; //Ԥ���ص���Ч��Դ
	std::vector<std::string> preloadBgFiles; //Ԥ���صı�����Դ
};

