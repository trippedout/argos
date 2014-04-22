#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"
#include "fmod.hpp"

//using namespace ci;
//using namespace ci::app;
//using namespace std;
//using namespace FMOD;

class SoundController
{
protected:
    FMOD::Sound*			mBackground;

public:
	static SoundController& getInstance()
	{
		static SoundController instance;
		return instance;
	}
	enum SoundId
	{
		BACKGROUND, SELECT,
		MOVE_VERTICAL, MOVE_HORIZONTAL, CHANGE_PAGE,
		HB_SELECT_ITEM, HB_SELECT_DROPDOWN, HB_BACK_TO_MAIN, HB_GLOBE_SPIN,
		LOC_BACK_TO_MAIN, LOC_CHANGE_PAGE, LOC_ENTER, LOC_CAMERA_MOVE,
		END_SHARE_SELECT, 
		DATA_EXIT_MOVIE,
		START_ENTER, PAUSE_ENTER, 
		ENG_ENTER, ENG_FAIL, ENG_MOVE, ENG_SELECT, ENG_TIMER, ENG_WIN,
		BUG_COMBINED, UI_NOTIFY,
		SCRUB_LOOP, SCRUB_START, SCRUB_STOP,

	};

	void startBackgroundLoop();

	void playSound(SoundId id);
	void playSound(SoundId id, FMOD::Channel **channel);

	void setBgVolume(float volume);
	void setBgVolume(float volume, float duration);
	
	void updateBgVolume();

private:
	typedef FMOD::Sound Sound;

	FMOD::System*	mSystem;
	//use pool
	FMOD::Channel*	mChannel;
	
	std::vector<FMOD::Channel*>* mChannels;
	int mNumChannels;
	int mCurChannel;
	
	FMOD::Channel*	mBgChannel;

	std::map<SoundId, Sound*> allSounds;
	ci::Anim<float> mBgVolume;

	SoundController(void);
	void loadAssets();
	void addSound(std::string path, Sound* sound, SoundId id);

	//adding these so that you can guarentee no copies will be made
	SoundController(SoundController const&);
	void operator=(SoundController const&);
};

