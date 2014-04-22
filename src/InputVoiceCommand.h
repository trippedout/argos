#pragma once

#include "InputBase.h"
#include "OscListener.h"

#define LISTEN_PORT 7000

using namespace ci;

class InputVoiceCommand : public InputBase
{
public:
	InputVoiceCommand(void);
	~InputVoiceCommand(void);

	osc::Listener	m_pOscListener;

	void		setup();
	void		update(float dt);

private:
	typedef std::map<std::string, INPUT_TYPE> TypeMap;
	typedef TypeMap::iterator TypeMapIter;
	TypeMap mPhraseMap;
	TypeMapIter mMapIter;

	//timer
	const std::string	mXboxString;
	const float		mMaxTime;
	float			mTime;
	bool			mCanSendSignals;
	bool			mIsTimerRunning;
	
};

