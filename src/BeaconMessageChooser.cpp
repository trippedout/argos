#include "BeaconMessageChooser.h"


BeaconMessageChooser::BeaconMessageChooser(Assets* assets) :
	InputView(assets),
	mMessages(new std::vector<BeaconReceivedListItemRef>)
{
	const std::string messages[] = {
		"Need help here",
		"Look around!",
		"Wow. Did you see that?",
		"I want to enroll at C.A.M.S.",
		"This is cool!",
		"U.N.S.C. forever!"
	};
	
	int num_of_col = 1;
	int num_of_row = 6;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );
	BeaconReceivedListItemRef message;

	for(unsigned int i = 0; i < 6; ++i)
	{
		message = BeaconReceivedListItemRef( new BeaconReceivedListItem(mAssets, messages[i]) ); 
		message->setPosition(ci::Vec3f(0.f, message->getSize()->value().y * i, 0.f));
		this->addChild(message);

		selectionGrid[0][i] = message;
		
		mMessages->push_back(message);
	}
	ci::Vec3f* msgSize = &message->getSize()->value();
	this->setSize(ci::Vec3f(msgSize->x, 6 * msgSize->y, 0.f));
}


BeaconMessageChooser::~BeaconMessageChooser(void)
{
}

void BeaconMessageChooser::setActive(bool active)
{
	this->setSelection(ci::Vec2i(-1,-1));

	this->useSelection = active;

	if(active)
		setSelection(ci::Vec2i(0,0));
}

void BeaconMessageChooser::handleInput(INPUT_TYPE type)
{
	if(this->useSelection)
	{
		InputView::handleInput(type);
	}
}