#include "SignalDispatcher.h"

using namespace boost::signals2;

SignalDispatcher::SignalDispatcher(void)
{
}

SignalDispatcher::~SignalDispatcher(void)
{
}

// <summary>
// This is a direct patch to our signal, just as if you were calling boost::signal.connect() directly.
// That being said, it takes boost::bind(...) as its input param.
// </summary>
void SignalDispatcher::registerSignal(const Signal::slot_type& slot)
{
	//make initial connection
	connection c = mInputSignal.connect(slot);
}

//TODO - this shit breaks EVERYTHING even though it should not be a dick and just fucking work.
void SignalDispatcher::unregisterSignal(const Signal::slot_type& slot)
{
	//mInputSignal.disconnect(slot);	
}