#include "SoundController.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace FMOD;

SoundController::SoundController(void) :
	mBackground(NULL),
	mBgVolume(1.f),
	mNumChannels(10),
	mCurChannel(0),
	mChannels(new std::vector<FMOD::Channel*>)
{
	loadAssets();

	//create pool
	for(int i = 0; i < mNumChannels; ++i)
	{
		FMOD::Channel* channel;
		mChannels->push_back(channel);
	}
}

void SoundController::loadAssets()
{
	//setup the system
	FMOD::System_Create( &mSystem );
    mSystem->init( 32, FMOD_INIT_NORMAL | FMOD_INIT_ENABLE_PROFILE, NULL );
	
	//Add all of your sounds to the map here, but dont forget to add to the ENUM either!
	//Sound* button1(NULL);			addSound( "audio/button-1.wav", button1, BUTTON_1 );
	
	//background loop!
	mSystem->createSound( getAssetPath( "audio/background.wav" ).string().c_str(), FMOD_SOFTWARE, NULL, &mBackground );
	mBackground->setMode( FMOD_LOOP_NORMAL );


	//everything else
	Sound* select(NULL);				addSound( "audio/end_share_select.mp3", select, SELECT ); //they wanted to use this mp3 specifically
	Sound* moveSelection(NULL);			addSound( "audio/move_vertical.mp3", moveSelection, MOVE_VERTICAL );
	Sound* moveHorizontal(NULL);		addSound( "audio/move_horizontal.mp3", moveHorizontal, MOVE_HORIZONTAL );
	Sound* changePage(NULL);			addSound( "audio/argos_change_page.mp3", changePage, CHANGE_PAGE ); //not sure when to play

	Sound* startEnter(NULL);			addSound( "audio/misc/start_enter.mp3", startEnter, START_ENTER );
//	Sound* pauseEnter(NULL);			addSound( "audio/misc/pause_enter.mp3", startEnter, START_ENTER );

	Sound* hbSelectItem(NULL);			addSound( "audio/hb_select_item.mp3", hbSelectItem, HB_SELECT_ITEM );
	Sound* hbSelectDropdown(NULL);		addSound( "audio/hb_select_dropdown.mp3", hbSelectDropdown, HB_SELECT_DROPDOWN );
	Sound* hbBackToMain(NULL);			addSound( "audio/hb_back_to_main.mp3", hbBackToMain, HB_BACK_TO_MAIN ); //not sure when to play
	Sound* hbGlobeSpin(NULL);			addSound( "audio/hb_globe_spin.mp3", hbGlobeSpin, HB_GLOBE_SPIN ); //add

	Sound* locBackMain(NULL);			addSound( "audio/loc_back_to_main.mp3", locBackMain, LOC_BACK_TO_MAIN );
	Sound* locChangePage(NULL);			addSound( "audio/loc_change_page.mp3", locChangePage, LOC_CHANGE_PAGE );
	Sound* locEnter(NULL);				addSound( "audio/loc_enter.mp3", locEnter, LOC_ENTER );
	Sound* locCamMove(NULL);			addSound( "audio/loc_camera_move.mp3", locCamMove, LOC_CAMERA_MOVE );
	
	Sound* endShareSelect(NULL);		addSound( "audio/end_share_select.mp3", endShareSelect, END_SHARE_SELECT );

	Sound* dataExitMovie(NULL);			addSound( "audio/data_exit_movie.mp3", dataExitMovie, DATA_EXIT_MOVIE );
	
	Sound* engEnter(NULL);				addSound( "audio/engage/enter.mp3", engEnter, ENG_ENTER );
	Sound* engFail(NULL);				addSound( "audio/engage/fail.mp3", engFail, ENG_FAIL );
	Sound* engMove(NULL);				addSound( "audio/engage/move.mp3", engMove, ENG_MOVE );
	Sound* engSelect(NULL);				addSound( "audio/engage/select.mp3", engSelect, ENG_SELECT );
	Sound* engTimer(NULL);				addSound( "audio/engage/timer.mp3", engTimer, ENG_TIMER );
	Sound* engWin(NULL);				addSound( "audio/engage/win.mp3", engWin, ENG_WIN );
	
	Sound* bugCombined(NULL);			addSound( "audio/misc/bug_combined.mp3", bugCombined, BUG_COMBINED );
	
	Sound* uiNotified(NULL);			addSound( "audio/misc/ui_notify.mp3", uiNotified, UI_NOTIFY );

	//Sound* scrubLoop(NULL);				addSound( "audio/misc/scrub_loop.mp3", scrubLoop, SCRUB_LOOP );
	//Sound* scrubStart(NULL);				addSound( "audio/misc/scrub_start.mp3", scrubLoop, SCRUB_START );
	//Sound* scrubStop(NULL);				addSound( "audio/misc/scrub_stop.mp3", scrubLoop, SCRUB_STOP );
	
	Sound* scrubLoop(NULL);
	mSystem->createSound( getAssetPath( "audio/misc/scrub_loop.mp3" ).string().c_str(), FMOD_SOFTWARE, NULL, &scrubLoop );
	scrubLoop->setMode( FMOD_LOOP_NORMAL );
	allSounds[SCRUB_LOOP] = scrubLoop;
}

/*
Used to play a simple single sound, on a channel managed by the SoundController channel pool.
*/
void SoundController::playSound(SoundId id)
{
	mSystem->playSound( FMOD_CHANNEL_REUSE, allSounds[id], true, &mChannels->at(mCurChannel) );
	mChannels->at(mCurChannel)->setVolume(.35);
	mChannels->at(mCurChannel)->setPaused(false);

	if(mCurChannel++ == mNumChannels-1) mCurChannel = 0;
}

/*
Plays a single sound on a channel passed from the calling view
*/
void SoundController::playSound(SoundId id, FMOD::Channel **channel)
{
	mSystem->playSound( FMOD_CHANNEL_REUSE, allSounds[id], false, channel );
}

void SoundController::startBackgroundLoop()
{
	mSystem->playSound(FMOD_CHANNEL_REUSE, mBackground, true, &mBgChannel);
	mBgChannel->setVolume(.45f);
	mBgChannel->setPaused(false);
}

/*
Tweens the background volume for 1 second
*/
void SoundController::setBgVolume(float volume)
{
	setBgVolume(volume, 1.f);
}

/*
Tweens background volume for specified duration
*/
void SoundController::setBgVolume(float volume, float duration)
{
	float max = .45f;
	
	if(volume > max) volume = max;

	timeline().apply(&mBgVolume, volume, duration)
		.updateFn(boost::bind(&SoundController::updateBgVolume, this));
}

void SoundController::updateBgVolume()
{
	mBgChannel->setVolume(mBgVolume.value());
}

void SoundController::addSound( std::string path, Sound* sound, SoundId id )
{
	mSystem->createSound( getAssetPath( path ).string().c_str(), FMOD_SOFTWARE, NULL, &sound );
	sound->setMode( FMOD_DEFAULT );
	allSounds[id] = sound;
}