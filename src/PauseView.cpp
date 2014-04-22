#include "PauseView.h"

using namespace ci;
using namespace ci::app;

PauseView::PauseView(Assets* assets, VideoViewRef vid, VideoSnapshotRef snap) : InputView(assets)
{

	//setup all views
	title = TextViewRef(new TextView("FORWARD UNTO DAWN", mAssets->getFontByID(Assets::FileID::DEFAULT_FONT), ci::ColorA(1.0f,1.0f,1.0f,1.0f)));
	title->setPosition( ci::Vec3f(220.0f - 28.0f,149.0f,0) );
	this->addChild(title);

	subtitle = TextViewRef(new TextView("SUBTITLE", mAssets->getFontByID("pause_subtitle"), ci::ColorA(0.16f,0.67f,0.88f,1.0f)));
	subtitle->setPosition( ci::Vec3f(220.0f - 25.0f,149.0f + title->getSize()->value().y - 5.0f,0) );
	this->addChild(subtitle);

	video = vid; 
	handbook = ButtonViewRef( new ARGOSButtonView(assets, "HANDBOOK") );
	handbook->setPosition(ci::Vec3f(200,220 + 30.0f,0)); 

	snapshot = snap;

	prefs = ButtonViewRef( new ARGOSButtonView(assets, "SETTINGS") );
	prefs->setPosition(ci::Vec3f(200,280 + 30.0f,0)); 

	pauseTimeline = boost::shared_ptr< PauseTimeline >( new PauseTimeline( assets, video->getDuration() ) );
	pauseTimeline->setPosition(ci::Vec3f(0,450 + 87,0)); 

	int num_of_col = 1;
	int num_of_row = 3;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	// hmm better way? maybe hard coding is the safest... not sure. 
	selectionGrid[0][0] = handbook; 
	selectionGrid[0][1] = prefs;
	selectionGrid[0][2] = pauseTimeline;
	
	useSelection = true; 
	setSelection(ci::Vec2i(0,0));
	
	bgBox = ImageViewRef( new ImageView(assets->getTextureByID("bgBox")) );
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setAlpha(0);

	/*
	gl::Fbo::Format format;
	format.setSamples(16);
	fbo = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );
	*/

	mAddBack = ImageViewRef( new ImageView( assets->getTextureByID("select_back") ) );
	mAddBack->setPosition(ci::Vec3f(180,950,0));
	this->addChild(mAddBack);

	this->addChild(handbook);
	this->addChild(prefs);
	this->addChild(pauseTimeline);
	this->addChild(bgBox);
	this->addChild(snapshot);
}

PauseView::~PauseView(void)
{

}

void PauseView::onAnimateIn(Settings::ViewID from)
{
	setSelection(ci::Vec2i(0,2)); 

	SoundController::getInstance().playSound(SoundController::PAUSE_ENTER);

	int time = (int)floor( video->getTime() ); 
	int hour = (time / 60 / 60) % 24;
    int min = (time / 60) % 60;
    int sec = time % 60;

	std::string s; 
	if( hour < 10 ) s+= "0";
	s+= toString(hour) + ":";
	if( min < 10 ) s += "0";
	s+= toString(min) + ":";
	if( sec < 10) s += "0";
	s +=  toString(sec);

	subtitle->setText("EPISODE_3_CHAPTER_1 _" + s);
	pauseTimeline->setInitialPercent( video->getTime() / video->getDuration() ); 
	pauseTimeline->checkData();

	pauseTimeline->forceAnimateIn(); 
	pauseTimeline->onEnterView();

	bgBox->setAlpha(1.0f);
	timeline().apply(bgBox->getAlpha(), 0.0f, 0.5f, EaseInQuint());

	if(from == Settings::ViewID::PLAY){
		snapshot->updateSnapshot(); 
		snapshot->setAlpha(1.0f);
		snapshot->setBlur(0.0f);
		timeline().apply(snapshot->getAlpha(), 0.0f, 0.75f, EaseInQuint());
		timeline().apply(snapshot->getBlur(), 0.75f, 0.5f, EaseInQuint());
	}


}

void PauseView::onAnimateOut(Settings::ViewID to)
{
	//onAnimateOutComplete();
	timeline().apply(bgBox->getAlpha(), 1.0f, 0.25f, EaseInQuint()).finishFn( boost::bind(&PauseView::onAnimateOutComplete, this) );
	video->setAlpha(0);
	timeline().apply(video->getAlpha(), 1.0f, 0.25f, EaseOutQuint()).delay(0.25); 

	pauseTimeline->onExitView();
}

void PauseView::handleInput(INPUT_TYPE type)
{
	//pass through the movement stuff
	InputView::handleInput(type);

	switch(type)
	{
	case SELECT:
		Settings::ViewID view; 
		if(selectedObject == handbook) view = Settings::ViewID::HANDBOOK;
		if(selectedObject == prefs) view = Settings::ViewID::PREFS;
		
		if(selectedObject == pauseTimeline){
			view = Settings::ViewID::PLAY;
			video->setTime(pauseTimeline->getPercent() * video->getDuration()); 
		}
		
		signal(this, view); 
		break;

	case BACK:
		signal(this, Settings::ViewID::PLAY);
		break;

	case PLAY:
		signal(this, Settings::ViewID::PLAY);
		break;

	case PREVIOUS:
		if(selectedObject == pauseTimeline){
			video->prevScene(); 
			pauseTimeline->setPercent( video->getTime() / video->getDuration() );  
		}
		break;

	case NEXT:
		if(selectedObject == pauseTimeline){
			video->nextScene(); 
			pauseTimeline->setPercent( video->getTime() / video->getDuration() );  
		}
		break;

	//voice specific
	//case DPAD_UP:
	//case BEACON:
		//TODO: set dataController -> pauseScene 
	//	signal(this, Settings::ViewID::BEACON);
	//	break;

	case PRESSED_X:
	case HANDBOOK:
		signal(this, Settings::ViewID::HANDBOOK);
		break;

	case PREFERENCES:
		signal(this, Settings::ViewID::PREFS);
		break;
	/*
	case DATA:
		signal(this, Settings::ViewID::DATA);
		break;

	case LOCATION:
		signal(this, Settings::ViewID::LOCATION);
		break;
	*/
            
        default:
            break;
	}
}

#ifdef _WIN32
void PauseView::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	int max = 32000; //ish? 
	ci::Vec2f left = ci::Vec2f((float)gamepad.sThumbLX/(float)max, gamepad.sThumbLY/(float)max ); 
		
	// dpad overrides the left stick. 
	int dpadLeft = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	int dpadRight = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
	if(  dpadLeft > 0 ) {
		left.x = -0.8f; 
	} else if( dpadRight > 0 ) {
		left.x = 0.8f; 
	}
	//ci::app::console()<< "PauseView::handleGamepad  DPAD LEFT: " << toString(dpadLeft) << " / DPAD RIGHT: " << toString(dpadRight) << std::endl;

	//triggers override dpad and left stick. 
	int leftTrig = (int)gamepad.bLeftTrigger;
	int rightTrig = (int)gamepad.bRightTrigger;
	float leftPer = (float)leftTrig / 255.0f;
	float rightPer = (float)rightTrig / 255.0f;
	ci::Vec2f triggers = ci::Vec2f( -leftPer + rightPer, 0); 

	if( selectedObject == pauseTimeline ){
		
		if(abs(triggers.x) > 0.1f){
			pauseTimeline->move(triggers.x); 
		}else{
			pauseTimeline->move(left.x); 
		}
	}
}
#endif


/*
void PauseView::render()
{
	fbo.bindTexture();
	
	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * FBO_WIDTH, scale.value().y * FBO_HEIGHT, scale.value().z);
		
		glColor4f(1.0f,1.0f,1.0f,1.0f); 

		glBegin( GL_QUADS );
			glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
			glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
			glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
			glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
		glEnd();

	glPopMatrix();
}

void PauseView::prerender()
{
	//bind FBO, render children. 

	gl::SaveFramebufferBinding bindingSaver;
	fbo.bindFramebuffer();

	float fov = 60.0f; 
	float camZ = (FBO_HEIGHT*0.5f) / tan(fov * 0.5f * (float)0.0174532925);
	
	CameraPersp cam( FBO_WIDTH, FBO_HEIGHT, fov );
	cam.setPerspective( 60.0f, (float)FBO_WIDTH/(float)FBO_HEIGHT, 1.0f, 10000.0f );
	ci::Vec3f camPos = ci::Vec3f( (float)FBO_WIDTH/2.0f, (float)FBO_HEIGHT/2.0f, camZ );
	ci::Vec3f camTarget = ci::Vec3f( (float)FBO_WIDTH/2.0f, (float)FBO_HEIGHT/2.0f, 0 );
	ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

	cam.lookAt( camPos, camTarget, camUp);
	gl::setMatrices( cam );
	gl::setViewport( fbo.getBounds() );
	gl::enableAlphaBlending();
	gl::clear(ci::ColorA(0,0,0,0));

	glEnable(GL_BLEND); 
	gl::enableAlphaBlending();
	
	glColor4f(1.0f,1.0f,1.0f,1.0f); 
	InputView::render(); 
	
	//unbind FBO
	fbo.unbindFramebuffer();
}
*/