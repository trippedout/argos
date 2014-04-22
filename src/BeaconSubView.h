#pragma once

#include "InputView.h"

class BeaconSubView : public InputView
{
public:
	enum CompletionType {
		COMPLETE, ESCAPED,
	};
protected:
	typedef boost::signals2::signal<void(CompletionType)> CompletionSignal;
	CompletionSignal mCompletionSignal;
public:
	BeaconSubView(Assets* assets);
	virtual ~BeaconSubView(void);

	virtual void setActive(bool active) = 0;
	CompletionSignal* getCompletionSignal() { return &mCompletionSignal; };
};
