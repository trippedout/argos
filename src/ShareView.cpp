#include "ShareView.h"


ShareView::ShareView(Assets* assets) : 
	InputView(assets),
	mWaypoint( ShareViewButtonRef( new ShareViewButton(assets, "WAYPOINT") ) ), 
	mFacebook( ShareViewButtonRef( new ShareViewButton(assets, "FACEBOOK") ) ), 
	mTwitter( ShareViewButtonRef( new ShareViewButton(assets, "TWITTER") ) ),
	mTitle( ImageViewRef( new ImageView(assets->getTextureByID("end_share_title") ) ) ),
	mSelectBack( ImageViewRef( new ImageView(assets->getTextureByID("select_back") ) ) ),
	mIsShowing(false)
{
	this->addChild(mWaypoint);

	mTitle->setPosition(ci::Vec3f(0.f, -200.f, 0.f));
	this->addChild(mTitle);

	mSelectBack->setPosition(ci::Vec3f(0.f, 450.f, 0.f));
	this->addChild(mSelectBack);

	mFacebook->setPosition(ci::Vec3f(385.f,0.f,0.f));
	this->addChild(mFacebook);
	
	mTwitter->setPosition(ci::Vec3f(770.f,0.f,0.f));
	this->addChild(mTwitter);

	int num_of_col = 3;
	int num_of_row = 1;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	// hmm better way? maybe hard coding is the safest... not sure. 
	selectionGrid[0][0] = mWaypoint; 
	selectionGrid[1][0] = mFacebook; 
	selectionGrid[2][0] = mTwitter; 
	
	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));

	onAnimateOut();
}

ShareView::~ShareView(void)
{
	mWaypoint.reset();
	mFacebook.reset();
	mTwitter.reset();
}

void ShareView::onAnimateIn()
{
	this->setAlpha(1.f);
	mIsShowing = true;

	onAnimateInComplete();
}

void ShareView::onAnimateOut()
{
	this->setAlpha(0.f);
	mIsShowing = false;

	onAnimateOutComplete();
}

void ShareView::handleInput(INPUT_TYPE type)
{
	InputView::handleInput(type);

	switch(type)
	{
	case SELECT:
		SoundController::getInstance().playSound(SoundController::END_SHARE_SELECT);
		break;
            
        default:
            break;
	}
}