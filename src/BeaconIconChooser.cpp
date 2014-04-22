#include "BeaconIconChooser.h"


BeaconIconChooser::BeaconIconChooser(Assets* assets) : 
	InputView(assets),
	mIcons(new std::vector<BeaconIconButtonRef>)
{
	//setup
	int num_of_col = 4;
	int num_of_row = 2;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );
	
	int y = 0, spacing = 100;

	//build icons
	for(int i = 1; i <= 7; ++i)
	{
		//next row
		if(i == 5) y++;

		BeaconIconButtonRef icon = BeaconIconButtonRef( new BeaconIconButton(assets, i) );
		
		if( y == 0 )
		{
			icon->setPosition(ci::Vec3f( (i-1) * spacing, 0.f, 0.f));
			selectionGrid[i-1][y] = icon;
		}
		else
		{
			icon->setPosition(ci::Vec3f((i-5) * spacing, spacing, 0.f));
			selectionGrid[i-5][y] = icon;
		}

		this->addChild(icon);
		mIcons->push_back(icon);
	}

}


BeaconIconChooser::~BeaconIconChooser(void)
{
	delete mIcons;
	mIcons = NULL;
}

void BeaconIconChooser::setActive(bool active)
{	
	setSelection(ci::Vec2i(-1,-1));

	this->useSelection = active;	

	if(active)
		setSelection(ci::Vec2i(0,0));
}

void BeaconIconChooser::handleInput(INPUT_TYPE type)
{
	InputView::handleInput(type);
}


void BeaconIconChooser::playMovementSound(ci::Vec2i selection)
{
	SoundController::getInstance().playSound(SoundController::MOVE_VERTICAL);
}
