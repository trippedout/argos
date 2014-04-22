#include "InputVoiceCommand.h"

using namespace ci;
using namespace std;

InputVoiceCommand::InputVoiceCommand(void) :
	mXboxString("XBOX"),
	mMaxTime(6.5f),
	mTime(0.f),
	mCanSendSignals(false),
	mIsTimerRunning(false)
{
	
}

InputVoiceCommand::~InputVoiceCommand(void)
{

}

void InputVoiceCommand::setup()
{
	m_pOscListener.setup(LISTEN_PORT);

	//set all types to string
	
	mPhraseMap[mXboxString] = XBOX;

	//evergreen
	mPhraseMap["UP"] = UP;
	mPhraseMap["DOWN"] = DOWN;
	mPhraseMap["LEFT"] = LEFT;
    mPhraseMap["RIGHT"] = RIGHT;
	mPhraseMap["SELECT"] = SELECT;
	mPhraseMap["BACK"] = BACK;
	mPhraseMap["NEXT"] = NEXT;
	mPhraseMap["PREVIOUS"] = PREVIOUS;
	mPhraseMap["PLAY"] = PLAY;

	//always on
	mPhraseMap["HANDBOOK"] = HANDBOOK;
	mPhraseMap["BEACON"] = BEACON;
	mPhraseMap["LOCATION"] = LOCATION;

	//start view
	mPhraseMap["START EPISODE"] = START_EPISODE;
	mPhraseMap["HOST A PARTY"] = HOST_A_PARTY;
	mPhraseMap["PREFERENCES"] = PREFERENCES;

	//hostview
	mPhraseMap["INVITE"] = INVITE;
	mPhraseMap["UNINVITE"] = UNINVITE;

	//prefs
	mPhraseMap["SUBTITLES OFF"] = SUBTITLES_OFF;
	mPhraseMap["SUBTITLES ON"] = SUBTITLES_ON;
	mPhraseMap["CLOSED CAPTIONS OFF"] = CLOSED_CAPTIONS_OFF;
	mPhraseMap["CLOSED CAPTIONS ON"] = CLOSED_CAPTIONS_ON;
	mPhraseMap["NOTIFICATIONS OFF"] = NOTIFICATIONS_OFF;
	mPhraseMap["NOTIFICATIONS ON"] = NOTIFICATIONS_ON;
	mPhraseMap["SOCIAL NOTIFICATIONS OFF"] = SOCIAL_NOTIFICATIONS_OFF;
	mPhraseMap["SOCIAL NOTIFICATIONS ON"] = SOCIAL_NOTIFICATIONS_ON;
	mPhraseMap["VOICE COMMAND OFF"] = VOICE_COMMAND_OFF;
	mPhraseMap["VOICE COMMAND ON"] = VOICE_COMMAND_ON;
	mPhraseMap["AUTO SHARE OFF"] = AUTOSHARE_OFF;
	mPhraseMap["AUTO SHARE ON"] = AUTOSHARE_ON;

	//longform
	mPhraseMap["DATA"] = DATA;
	mPhraseMap["LOCATION"] = LOCATION;
	mPhraseMap["ONI"] = ONI;
	mPhraseMap["PAUSE"] = PAUSE;
	mPhraseMap["SETTINGS"] = PREFERENCES;

	//location
	mPhraseMap["ORBITAL"] = ORBITAL;
	mPhraseMap["AERIAL"] = AERIAL;
	mPhraseMap["LOCAL"] = LOCAL;

	//data
	mPhraseMap["OVERVIEW"] = OVERVIEW;
	mPhraseMap["WAYPOINT"] = WAYPOINT;
	//reuse DATA from before


}

void InputVoiceCommand::update(float dt)
{
	if(mCanSendSignals)
	{
		mTime += dt;

		if( mTime >= mMaxTime )
		{
			ci::app::console() << "Time expired! Send VOICE_DEACTIVATE" << std::endl;
			mCanSendSignals = false;
			mInputSignal(VOICE_DEACTIVATE);
			mTime = 0.f;
			return;
		}
	}

	while( m_pOscListener.hasWaitingMessages() ) 
	{
		osc::Message message;
		m_pOscListener.getNextMessage( &message );
		
		if( message.getAddress() == "/voice_command" )
		{
			//message received from voice command app
			std::string phrase = message.getArgAsString(0);

			ci::app::console() << "Phrase received: " << phrase << std::endl;
			
			if( !mCanSendSignals && phrase == mXboxString )
			{
				ci::app::console() << "Send VOICE_ACTIVATE!" << std::endl;
				mCanSendSignals = true;
				mInputSignal(VOICE_ACTIVATE);
				return;
			}

			//dont do shit unless we allow signals to be sent
			if( !mCanSendSignals ) return;

			//check for specific up and down commands from voice
			if(phrase == "UP") mInputSignal(VOICE_UP);
			if(phrase == "DOWN") mInputSignal(VOICE_DOWN);
			if(phrase == "LEFT") mInputSignal(VOICE_LEFT);
			if(phrase == "RIGHT") mInputSignal(VOICE_RIGHT);

			//only send these once xbox has been said 
			for( mMapIter = mPhraseMap.begin(); mMapIter != mPhraseMap.end(); mMapIter++ )
			{
				if (mMapIter->first == phrase)
				{ 
					ci::app::console() << "Send: " << mMapIter->first << " and reset time!" << std::endl;
					mInputSignal(mMapIter->second);
					mTime = 0.f;
					break;
				}
			}			
		}
	}
}