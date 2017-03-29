#include "GameMusicTools.h"
#include "GameConfig.h"

GameMusicTools::GameMusicTools()
{
}


GameMusicTools::~GameMusicTools()
{
}



void GameMusicTools::playBgMusic(const char* pszFilePath)
{
	if (!GameConfig::getInstance()->getMusicOn()) return;
	std::string musicPath = pszFilePath;
	size_t pos = musicPath.find(".mp3");
	if (pos != std::string::npos)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		musicPath = musicPath.substr(0, pos + 1) + "mp3";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		musicPath = musicPath.substr(0, pos + 1) + "mp3";
#endif
		std::string res = FileUtils::getInstance()->fullPathForFilename(musicPath);
		if (res.length() < 0)return;
		
	}

	doPlayMusic(musicPath.c_str(), true);
}

void GameMusicTools::playSoundEffect(const char* pszFilePath, int nativeId,bool isloop, float delayTime)
{
	if (!GameConfig::getInstance()->getSoundOn()) return;
	std::string musicPath = pszFilePath;
//	size_t pos = musicPath.find(".wav");
//	if (pos != std::string::npos)
//	{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//		musicPath = musicPath.substr(0, pos + 1) + "ogg";
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//		musicPath = musicPath.substr(0, pos + 1) + "mp3";
//#endif
		std::string res = FileUtils::getInstance()->fullPathForFilename(musicPath);
		if (res.length() < 0)return;

//	}


	unsigned int soundId = _HashStr(musicPath.c_str());

	map<unsigned int, MusicEffectInfo>::iterator p = soundCt_map.find(soundId);
	if (p != soundCt_map.end())
	{
		if (p->second.delay <= p->second.currentTime)
		{
			doPlayEffect(p->second.filepath.c_str(), nativeId,isloop);
			p->second.currentTime = 0;
		}
	}
	else
	{
		MusicEffectInfo info;
		info.delay = delayTime;
		info.filepath = musicPath;
		info.currentTime = 0;

		soundCt_map.insert(pair<unsigned int, MusicEffectInfo>(soundId, info));
		doPlayEffect(musicPath.c_str(), nativeId, isloop);
	}

}
void GameMusicTools::stopSoundEffect(const char* pszFilePath, int nativeId)
{

	std::string musicPath = pszFilePath;
	size_t pos = musicPath.find(".wav");
//	if (pos != std::string::npos)
//	{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//		musicPath = musicPath.substr(0, pos + 1) + "ogg";
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//		musicPath = musicPath.substr(0, pos + 1) + "mp3";
//#endif
		std::string res = FileUtils::getInstance()->fullPathForFilename(musicPath);
		if (res.length() < 0)return;

	//}


	unsigned int sid = _HashStr(musicPath.c_str()) + nativeId;
	if (constantEffectIDs.find(sid) != constantEffectIDs.end())
	{
		
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(constantEffectIDs.at(sid));
		constantEffectIDs.erase(sid);
	}
}

void GameMusicTools::SoundEffectControl(float delayTime)
{
	float delay = 0.0;
	map<unsigned int, MusicEffectInfo>::iterator iter;
	for (iter = soundCt_map.begin(); iter != soundCt_map.end(); iter++)
	{
		iter->second.currentTime += delayTime;
	}
}

void GameMusicTools::doPlayMusic(const char* musicpath, bool isloop)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(musicpath, isloop);
	
}
void GameMusicTools::doPlayEffect(const char* effectpath,int nativeId, bool isloop)
{
	int soundID = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectpath, isloop);
	if (isloop)
	{
		constantEffectIDs.insert(make_pair(_HashStr(effectpath) + nativeId, soundID));
	}
	
}
void GameMusicTools::stopBGMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}
void GameMusicTools::stopAllEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	soundCt_map.clear();
}

void GameMusicTools::pauseBGM()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void GameMusicTools::resumeBGM()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
void GameMusicTools::pauseAllEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}
void GameMusicTools::resumeAllEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void GameMusicTools::setMusicVolume(float volume)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

void GameMusicTools::setEffectVolume(float volume)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
	
}

void GameMusicTools::preloadResource()
{
	std::thread effThread([&]
	{
		for (auto& loadingpath : preloadEffFiles)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(loadingpath.c_str());
		}
	});
	effThread.detach();

	std::thread bgThread([&]
	{
		for (auto& loadingpath : preloadBgFiles)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(loadingpath.c_str());
		}
	});
	bgThread.detach();

}

void GameMusicTools::unloadResource()
{
	for (auto& loadingpath : preloadEffFiles)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(loadingpath.c_str());
	}
	preloadEffFiles.clear();
}
void GameMusicTools::addPreloadEffResource(const char* filepath)
{
	if (std::find(preloadEffFiles.begin(), preloadEffFiles.end(), filepath) != preloadEffFiles.end())
	{
		preloadEffFiles.push_back(filepath);
	}
}
void GameMusicTools::addPreloadBgResource(const char* filepath)
{
	if (std::find(preloadBgFiles.begin(), preloadBgFiles.end(), filepath) != preloadBgFiles.end())
	{
		preloadBgFiles.push_back(filepath);
	}
}
