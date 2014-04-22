#pragma once

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include "InputTypes.h"

//using namespace boost::signals2;

class SignalDispatcher
{
public:
	SignalDispatcher(void);
	~SignalDispatcher(void);

	typedef			boost::signals2::signal<void (INPUT_TYPE)> Signal;
	
	void			registerSignal(const Signal::slot_type& slot);
	void			unregisterSignal(const Signal::slot_type& slot);

protected:

	Signal			mInputSignal;
};

