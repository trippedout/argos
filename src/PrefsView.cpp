#include "PrefsView.h"

using namespace ci;
using namespace ci::app;

PrefsView::PrefsView(Assets* assets) : InputView(assets)
{
	//select back
	mSelectBack = ImageViewRef( new ImageView( assets->getTextureByID("select_back") ) );
	mSelectBack->setPosition(ci::Vec3f(180,950,0));
	this->addChild(mSelectBack);

	//set all basic positioning
	float base = 365.0f;
	float spacing = 58.0f;

	mAnimMap["title_in"] = ci::Vec3f(213.0f,558.0f,0);
	mAnimMap["title_top"] = ci::Vec3f(220.0f,149.0f,0);
	mAnimMap["subtitles"] = ci::Vec3f(475.0f,base,0);
	mAnimMap["closed_cap"] = ci::Vec3f(475.0f,base + (spacing*1.0f),0);
	mAnimMap["notifs"] = ci::Vec3f(475.0f,base + (spacing*2.0f),0);
	mAnimMap["social_notifs"] = ci::Vec3f(475.0f,base + (spacing*3.0f),0);
	mAnimMap["voice_ctrls"] = ci::Vec3f(475.0f,base + (spacing*4.0f),0);
	mAnimMap["auto_share"] = ci::Vec3f(475.0f,base + (spacing*5.0f),0);

	
	//setup all views
	mTitle = TextViewRef(new TextView("PREFERENCES", mAssets->getFontByID(Assets::FileID::DEFAULT_FONT), ci::ColorA(1.0f,1.0f,1.0f,1.0f)));
	mTitle->setPosition(mAnimMap["title_top"]);//mAnimMap["title_in"]);
	this->addChild(mTitle);

	mSubtitles = ButtonViewRef(new TextButtonView(mAssets, "SUBTITLES"));
	mSubtitles->setPosition(mAnimMap["subtitles"]);
	mSubtitles->setCenter(*mRightAlignVec3f);
	this->addChild(mSubtitles);

	mClosedCap = ButtonViewRef(new TextButtonView(mAssets, "CLOSED CAPTIONS"));
	mClosedCap->setPosition(mAnimMap["closed_cap"]);
	mClosedCap->setCenter(*mRightAlignVec3f);
	this->addChild(mClosedCap);
	
	mNotifs = ButtonViewRef(new TextButtonView(mAssets, "NOTIFICATIONS"));
	mNotifs->setPosition(mAnimMap["notifs"]);
	mNotifs->setCenter(*mRightAlignVec3f);
	this->addChild(mNotifs);

	mSocialNotifs = ButtonViewRef(new TextButtonView(mAssets, "SOCIAL NOTIFICATIONS"));
	mSocialNotifs->setPosition(mAnimMap["social_notifs"]);
	mSocialNotifs->setCenter(*mRightAlignVec3f);
	this->addChild(mSocialNotifs);

	mVoiceCtrls = ButtonViewRef(new TextButtonView(mAssets, "VOICE CONTROLS"));
	mVoiceCtrls->setPosition(mAnimMap["voice_ctrls"]);
	mVoiceCtrls->setCenter(*mRightAlignVec3f);
	this->addChild(mVoiceCtrls);

	mAutoShare = ButtonViewRef(new TextButtonView(mAssets, "AUTO SHARE"));
	mAutoShare->setPosition(mAnimMap["auto_share"]);
	mAutoShare->setCenter(*mRightAlignVec3f);
	this->addChild(mAutoShare);


	//setup selection
	int num_of_col = 1;
	int num_of_row = 6;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	selectionGrid[0][0] = mSubtitles;
	selectionGrid[0][1] = mClosedCap;
	selectionGrid[0][2] = mNotifs;
	selectionGrid[0][3] = mSocialNotifs;
	selectionGrid[0][4] = mVoiceCtrls;
	selectionGrid[0][5] = mAutoShare;

	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));


	//toggle switches
    ci::Vec3i toggleOffset = Vec3i(105,-20,0);

	mToggleSubtitles		=	PrefsViewToggleButtonRef( new PrefsViewToggleButton(assets) );
	mToggleSubtitles->setPosition(mAnimMap["subtitles"] + toggleOffset);
	this->addChild(mToggleSubtitles);
	mToggleMap[mSubtitles] = mToggleSubtitles;

	mToggleClosedCap		=	PrefsViewToggleButtonRef( new PrefsViewToggleButton(assets) );
	mToggleClosedCap->setPosition(mAnimMap["closed_cap"] + toggleOffset);
	this->addChild(mToggleClosedCap);
	mToggleMap[mClosedCap] = mToggleClosedCap;

	mToggleNotifs			=	PrefsViewToggleButtonRef( new PrefsViewToggleButton(assets) );
	mToggleNotifs->setPosition(mAnimMap["notifs"] + toggleOffset);
	this->addChild(mToggleNotifs);
	mToggleMap[mNotifs] = mToggleNotifs;

	mToggleSocialNotifs		=	PrefsViewToggleButtonRef( new PrefsViewToggleButton(assets) );
	mToggleSocialNotifs->setPosition(mAnimMap["social_notifs"] + toggleOffset);
	this->addChild(mToggleSocialNotifs);
	mToggleMap[mSocialNotifs] = mToggleSocialNotifs;

	mToggleVoiceCtrls		=	PrefsViewToggleButtonRef( new PrefsViewToggleButton(assets) );
	mToggleVoiceCtrls->setPosition(mAnimMap["voice_ctrls"] + toggleOffset);
	this->addChild(mToggleVoiceCtrls);
	mToggleMap[mVoiceCtrls] = mToggleVoiceCtrls;

	mToggleAutoShare		=	PrefsViewToggleButtonRef( new PrefsViewToggleButton(assets) );
	mToggleAutoShare->setPosition(mAnimMap["auto_share"] + toggleOffset);
	this->addChild(mToggleAutoShare);
	mToggleMap[mAutoShare] = mToggleAutoShare;


	//language list
	mLanguageList			=	PrefsLanguageList( new PrefsViewLanguageList(assets) );
	mLanguageList->setPosition(mToggleSubtitles->getPosition()->value() + ci::Vec3f(362,0,0));
	this->addChild(mLanguageList);


	//styling
	//guide = new ImageView( new ci::gl::Texture( loadImage( loadAsset( "guides/prefs.png"))));
	//createParams();
	//params->addParam("title", &mClosedCap->getPosition()->value() );
}

PrefsView::~PrefsView(void)
{

}

void PrefsView::render()
{
	InputView::render();

}

void PrefsView::draw()
{
	InputView::draw();
}

void PrefsView::moveSelection(ci::Vec2i dir)
{
	if(this->useSelection)
		InputView::moveSelection(dir);
	else
		mLanguageList->moveSelection(dir);
}

void PrefsView::onAnimateIn()
{
	mTitle->setPosition(mAnimMap["title_in"]);
	timeline().apply(mTitle->getPosition(), mAnimMap["title_in"], mAnimMap["title_top"], 0.5f, EaseOutCubic());

	int size = children.size();
	for(int i = 0; i < size; i++ )
	{
		DisplayObjectRef ptr = children[i];

		if(ptr != mLanguageList && ptr != mTitle)
		{
			ptr->setAlpha(0.0f);

			//check to see if ptr is a DOC and use setInternalAlpha if so
			DisplayObjectContainerRef doc = boost::dynamic_pointer_cast<DisplayObjectContainer>(ptr);
			
			if(doc)
			{
				timeline().apply(doc->getAlpha(), 1.0f, 0.5f)
					.delay(0.2f + i*0.05f)
					.updateFn(std::bind(&DisplayObjectContainer::setInternalAlpha, doc))
					.finishFn(std::bind(&View::onAnimateInComplete, this));
			}
			else
			{
				timeline().apply(ptr->getAlpha(), 1.0f, 0.5f)
					.delay(0.2f + i*0.05f)
					.finishFn(std::bind(&View::onAnimateInComplete, this));
			}
		}
	}

	//check if languagelist is active and animate it in if it is
	if(mLanguageList->isShowing())
	{
		mLanguageList->setAlpha(0.0f);

		timeline().apply(mLanguageList->getAlpha(), 0.5f, 0.5f)
			.delay(1.0f)
			.updateFn(std::bind(&DisplayObjectContainer::setInternalAlpha, mLanguageList));
	}
}

void PrefsView::onAnimateOut()
{
	View::onAnimateOut();
}

void PrefsView::resetPositions()
{

}

void PrefsView::handleInput(INPUT_TYPE type)
{	
	//immediately check to see if we're in language list and just pass it
	if( mLanguageList->isShowing() && mLanguageList->useSelection ) mLanguageList->handleInput(type);

	switch(type)
	{
	case SELECT:		
		if( mLanguageList->isShowing() && mLanguageList->useSelection ) 
			mLanguageList->setLanguage();
		else
			toggleButton();
		return;

	case BACK:
		if( mLanguageList->isShowing() && mLanguageList->useSelection ) 
			this->activateLanguageList(false);
		else
			signal(this, Settings::ViewID::BACK);
		return;
		
	case DPAD_UP:
	case UP:
		this->moveSelection(ci::Vec2i(0,-1));
		return;
	
	case DPAD_DOWN:
	case DOWN:
		this->moveSelection(ci::Vec2i(0, 1));
		return;

		//go into language list if its active
	case DPAD_RIGHT:
	case RIGHT:
		if( selectedObject == mSubtitles && this->useSelection && mLanguageList->isShowing() )
			activateLanguageList(true);
		else
			toggleButton();
		return;

	case DPAD_LEFT:
	case LEFT:
		if( mLanguageList->isShowing() && mLanguageList->useSelection )
			activateLanguageList(false);
		else
			toggleButton();
		return;
	
	case HANDBOOK:
		signal(this, Settings::ViewID::HANDBOOK);
		break;
	}

	//VOICE ACTIONS, LOTS OF BULLSHIT

	//use switch to highlight correct button
	switch(type)
	{
		case SUBTITLES_OFF:
		case SUBTITLES_ON:
			setSelection(ci::Vec2i(0,0));
			break;
		case CLOSED_CAPTIONS_OFF:
		case CLOSED_CAPTIONS_ON:
			setSelection(ci::Vec2i(0,1));
			break;
		case NOTIFICATIONS_OFF:
		case NOTIFICATIONS_ON:
			setSelection(ci::Vec2i(0,2));
			break;
		case SOCIAL_NOTIFICATIONS_OFF:
		case SOCIAL_NOTIFICATIONS_ON:
			setSelection(ci::Vec2i(0,3));
			break;
		case VOICE_COMMAND_OFF:
		case VOICE_COMMAND_ON:
			setSelection(ci::Vec2i(0,4));
			break;
		case AUTOSHARE_OFF:
		case AUTOSHARE_ON:
			setSelection(ci::Vec2i(0,5));
			break;
	}
	
	//get ptr for toggle 	
	PrefsViewToggleButtonRef toggleBtn = boost::dynamic_pointer_cast<PrefsViewToggleButton>( mToggleMap[selectedObject] );
	
	//use switch again to check for specifics
	switch(type)
	{
	//special cases
	case SUBTITLES_OFF:
		if(!toggleBtn->isOff()) 
		{
			toggleButton();
			//toggleBtn->toggle();
			///handleInput(SELECT);
		}
		break;
	case SUBTITLES_ON:
		if(toggleBtn->isOff())
		{
			//toggleBtn->toggle();
			handleInput(SELECT);
		}
		break;

	case CLOSED_CAPTIONS_OFF:
	case NOTIFICATIONS_OFF:
	case SOCIAL_NOTIFICATIONS_OFF:
	case VOICE_COMMAND_OFF:
	case AUTOSHARE_OFF:
		if(!toggleBtn->isOff()) toggleBtn->toggle();
		break;

	case CLOSED_CAPTIONS_ON:
	case NOTIFICATIONS_ON:
	case SOCIAL_NOTIFICATIONS_ON:
	case VOICE_COMMAND_ON:
	case AUTOSHARE_ON:
		if(toggleBtn->isOff()) toggleBtn->toggle();
		break;

	}
}

void PrefsView::toggleButton()
{
	SoundController::getInstance().playSound(SoundController::SELECT);

	if( selectedObject == mSubtitles && this->useSelection )
	{
		mToggleSubtitles->toggle();

		if(mToggleSubtitles->isOff())
			hideLanguageList();
		else
			showLanguageList();
	}
	else if( selectedObject == mSubtitles && !this->useSelection )
	{
		mToggleSubtitles->toggle();

		if(mToggleSubtitles->isOff())
			hideLanguageList();
		else
			showLanguageList();
	}
	else
	{
		mToggleMap[selectedObject]->toggle();
	}

}

void PrefsView::showLanguageList()
{
	this->activateLanguageList(true);
	mLanguageList->show();
}

void PrefsView::hideLanguageList()
{
	this->activateLanguageList(false);
	mLanguageList->hide();
}

void PrefsView::activateLanguageList(bool activate)
{	
	SoundController::getInstance().playSound(SoundController::MOVE_HORIZONTAL);

	mLanguageList->setActive(activate);
	
	if(activate)
		this->useSelection = false;
	else
		this->useSelection = true;
}

