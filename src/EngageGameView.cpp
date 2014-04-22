#include "EngageGameView.h"

/*

	ImageViewRef		mDashes;
	ImageViewRef		mOutline1, mOutline1Select1, mOutline1Select2;
	ImageViewRef		mOutline2, mOutline2Select2, mOutline2Select2;
*/

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

EngageGameView::EngageGameView(Assets* assets) : 
	InputView(assets),
	mOutline1( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_border") ) ) ),
	mOutline1Select1( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_border") ) ) ),
	mOutline1Select2( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_border") ) ) ),
	
	mOutline2( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_border") ) ) ),
	mOutline2Select1( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_border") ) ) ),
	mOutline2Select2( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_border") ) ) ),
	
	mDashes( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_dashes") ) ) ),

	mTimerText( TextViewRef( new TextView( "10.00", assets->getFontByID("ARGOS_button"), ci::ColorA(1.f,205.f/255.f,44.f/255.f,1.f) ) ) ),
	mTimerStaticText( TextViewRef( new TextView( "SECONDS REMAINING", assets->getFontByID("ARGOS_button"), ci::ColorA(1.f,205.f/255.f,44.f/255.f,1.f) ) ) ),
	mTitleText( TextViewRef( new TextView( "INPUT OVERRIDE SEQUENCE", assets->getFontByID("ARGOS_button"), ci::ColorA(1.0f,1.0f,1.0f) ) ) ),

	mIsPlaying(false),
	mTimerCount(10.f),
	mLetterContainer(DisplayObjectContainerRef( new DisplayObjectContainer ) ),
	mGameLetters(new std::vector<KeypadButtonRef>),
	mTotalLettersPlayed(0),
	mResult(0),
	mShader(assets->getShaderByID("blur")),
	mFbo1(), mFbo2(),
	mBlurAmount(1.f),
	mAlphaAmount(1.f),
	selectTop(true)
{

	mOutline1->setAlpha(0.85f); 
	mOutline2->setAlpha(0.85f); 

	mOutline1Select1->setAlpha(0.75f); 
	mOutline1Select2->setAlpha(0.75f); 
	mOutline1Select2->setColor( ci::Vec3f(192.0f/255.0f, 250.0f/255.0f, 1.0) ); 

	mOutline2Select1->setAlpha(0); 
	mOutline2Select1->setRotation( ci::Vec3f(0,0,0) );
	mOutline2Select2->setAlpha(0); 
	mOutline2Select2->setColor( ci::Vec3f(192.0f/255.0f, 250.0f/255.0f, 1.0) ); 
	
	mOutline1->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mOutline1Select1->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mOutline1Select2->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mOutline2->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mOutline2Select1->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mOutline2Select2->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	
	mOutline1->setPosition(ci::Vec3f(192,280,0)); 
	mOutline1Select1->setPosition(ci::Vec3f(192,280,15)); 
	mOutline1Select2->setPosition(ci::Vec3f(192,280,-15)); 

	mOutline2->setPosition(ci::Vec3f(192,570,0)); 
	mOutline2Select1->setPosition(ci::Vec3f(192,570,15)); 
	mOutline2Select2->setPosition(ci::Vec3f(192,570,-15)); 

	this->addChild(mOutline1);
	this->addChild(mOutline1Select1);
	this->addChild(mOutline1Select2);
	
	this->addChild(mOutline2);
	this->addChild(mOutline2Select1);
	this->addChild(mOutline2Select2);

	mDashes->setPosition(ci::Vec3f(73, 121,0)); 
	this->addChild(mDashes);

	mLetterContainer->setPosition(ci::Vec3f(48.f, 46.f, 0.f));
	this->addChild(mLetterContainer);


	//DONT ADDCHILD TEXT! its rendered over the FBO. 
	mTimerText->setCenter(ci::Vec3f( 0.f, 0.f, 0.f) );
	mTimerText->setPosition(ci::Vec3f(10.f, 765.f, 0.f) );
	//this->addChild(mTimerText);

	mTimerStaticText->setPosition(ci::Vec3f(90.f, 765.f, 0.f) );
	//this->addChild(mTimerStaticText);

	mTitleText->setCenter(ci::Vec3f(0.5f,0,0)); 
	mTitleText->setPosition(ci::Vec3f(192,0,0)); 
	//this->addChild(mTitleText);

	mTitleText->setAlpha(0);
	mTimerText->setAlpha(0);
	mTimerStaticText->setAlpha(0);

	createKeypad();	

	//fbo
	gl::Fbo::Format format;
	format.setSamples(16);
	//format.enableMipmapping(true);

	mFbo1 = Fbo( 1920, 1080, format );
	mFbo2 = Fbo( 1920, 1080, format );

	setSize( ci::Vec3f(1920.0f, 1080.0f, 0) );
}

EngageGameView::~EngageGameView(void)
{
	mOutline1.reset();
	mOutline1Select1.reset();
	mOutline1Select2.reset(); 
	    
	mOutline2.reset();
	mOutline2Select1.reset(); 
	mOutline2Select2.reset();

	mTimerText.reset();

	delete mGameLetters;
}

void EngageGameView::createKeypad()
{
	const float col1 = 28.f, col2 = 140.f, col3 = 253.f,
		row1 = 176.f, row2 = 289.f, row3 = 462.f, row4 = 575.f;

	const float zed = 0.f;

	mOne = getKey(KeyType::ONE);
	mOne->setPosition(ci::Vec3f( col1, row1, zed ) );
	this->addChild(mOne);

	mTwo = getKey(KeyType::TWO);
	mTwo->setPosition(ci::Vec3f( col2, row1, zed ) );
	this->addChild(mTwo);
	
	mThree = getKey(KeyType::THREE);
	mThree->setPosition(ci::Vec3f( col3, row1, zed ) );
	this->addChild(mThree);

	mFour = getKey(KeyType::FOUR);
	mFour->setPosition(ci::Vec3f( col1, row3, zed ) );
	this->addChild(mFour);

	mFive = getKey(KeyType::FIVE);
	mFive->setPosition(ci::Vec3f( col2, row3, zed ) );
	this->addChild(mFive);

	mSix = getKey(KeyType::SIX);
	mSix->setPosition(ci::Vec3f( col3, row3, zed ) );
	this->addChild(mSix);

	mLeft1 = getKey(KeyType::LEFT);
	mLeft1->setPosition(ci::Vec3f( col1, row2, zed ) );
	this->addChild(mLeft1);

	mLeft2 = getKey(KeyType::LEFT);
	mLeft2->setPosition(ci::Vec3f( col1, row4, zed ) );
	this->addChild(mLeft2);

	mDown1 = getKey(KeyType::DOWN);
	mDown1->setPosition(ci::Vec3f( col2, row2, zed ) );
	this->addChild(mDown1);

	mDown2 = getKey(KeyType::DOWN);
	mDown2->setPosition(ci::Vec3f( col2, row4, zed ) );
	this->addChild(mDown2);

	mRight1 = getKey(KeyType::RIGHT);
	mRight1->setPosition(ci::Vec3f( col3, row2, zed ) );
	this->addChild(mRight1);

	mRight2 = getKey(KeyType::RIGHT);
	mRight2->setPosition(ci::Vec3f( col3, row4, zed ) );
	this->addChild(mRight2);

	int num_of_col = 3;
	int num_of_row = 4;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	selectionGrid[0][0] = mOne; 
	selectionGrid[1][0] = mTwo; 
	selectionGrid[2][0] = mThree;

	selectionGrid[0][1] = mLeft1; 
	selectionGrid[1][1] = mDown1; 
	selectionGrid[2][1] = mRight1;

	selectionGrid[0][2] = mFour; 
	selectionGrid[1][2] = mFive; 
	selectionGrid[2][2] = mSix;

	selectionGrid[0][3] = mLeft2; 
	selectionGrid[1][3] = mDown2; 
	selectionGrid[2][3] = mRight2;

	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));
}

boost::shared_ptr<EngageGameKeypadButton> EngageGameView::getKey(EngageGameKeypadButton::KeyType keyType)
{
	return KeypadButtonRef( new EngageGameKeypadButton(mAssets, keyType, false) );
}

void EngageGameView::resetGame()
{
	timeline().apply(mTitleText->getAlpha(), 0.0f, 0.1f, EaseInQuint());
	timeline().apply(mTimerText->getAlpha(), 0.0f, 0.1f, EaseInQuint()); 
	timeline().apply(mTimerStaticText->getAlpha(), 0.0f, 0.1f, EaseInQuint()); 

	this->setSelection(ci::Vec2i(0,0));

	selectTop = true;
	timeline().apply( mOutline1Select1->getAlpha(), 0.75f, 0.2f, EaseOutQuint() );
	timeline().apply( mOutline1Select2->getAlpha(), 0.75f, 0.2f, EaseOutQuint() );
	timeline().apply( mOutline2Select1->getAlpha(), 0.0f, 0.2f, EaseOutQuint() );
	timeline().apply( mOutline2Select2->getAlpha(), 0.0f, 0.2f, EaseOutQuint() );

	mTimerCount = 10.f;
	mTotalLettersPlayed = 0;
	mResult = 0;

	//clear container and start over
	for(unsigned int i = 0; i < mGameLetters->size(); ++i)
	{
		mLetterContainer->removeChild(mGameLetters->at(i));
	}

	mGameLetters->clear();
}

void EngageGameView::startGame()
{
	mIsPlaying = true;

	timeline().apply(mTitleText->getAlpha(), 1.0f, 0.25f, EaseInQuint()); 
	timeline().apply(mTimerText->getAlpha(), 1.0f, 0.25f, EaseInQuint()); 
	timeline().apply(mTimerStaticText->getAlpha(), 1.0f, 0.25f, EaseInQuint()); 
}

void EngageGameView::update(float dt)
{
	if(mIsPlaying)
	{

		if(selectionPos.y > 1 && selectTop){
			selectTop = false;
			timeline().apply( mOutline2Select1->getAlpha(), 0.75f, 0.4f, EaseOutQuint() );
			timeline().apply( mOutline2Select2->getAlpha(), 0.75f, 0.4f, EaseOutQuint() );

			timeline().apply( mOutline1Select1->getAlpha(), 0.0f, 0.4f, EaseOutQuint() );
			timeline().apply( mOutline1Select2->getAlpha(), 0.0f, 0.4f, EaseOutQuint() );

		}else if(selectionPos.y <= 1 && !selectTop){
			selectTop = true;
			timeline().apply( mOutline1Select1->getAlpha(), 0.75f, 0.4f, EaseOutQuint() );
			timeline().apply( mOutline1Select2->getAlpha(), 0.75f, 0.4f, EaseOutQuint() );
			
			timeline().apply( mOutline2Select1->getAlpha(), 0.0f, 0.4f, EaseOutQuint() );
			timeline().apply( mOutline2Select2->getAlpha(), 0.0f, 0.4f, EaseOutQuint() );
		}


		//ci::app::console() << "EngageGameView::update() timer! " << dt << std::endl;
		mTimerCount -= dt;

		if(mTimerCount <= 0.f)
		{
			mTimerCount = 0.f;
			mIsPlaying = false;
			
			//check winner
			checkGameResults();
		}

		mTimerText->setText( getTimerFormatted(mTimerCount) );
	}
}

void EngageGameView::prerender()
{
	
	gl::SaveFramebufferBinding bindingSaver;

	glPushMatrix(); 
	
		mFbo1.bindFramebuffer();
		{
			float fov = 60.0f; 
			float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
			CameraPersp cam( size.value().x, size.value().y, fov );
			cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, 10000.0f );
	
			ci::Vec3f camPos = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, camZ );
			ci::Vec3f camTarget = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, 0 );
			ci::Vec3f camUp = ci::Vec3f( 0, -1.0f, 0 );

			cam.lookAt( camPos, camTarget, camUp);
			gl::setMatrices( cam );
			gl::setViewport( mFbo1.getBounds() );
			
			glEnable(GL_BLEND);
			gl::enableAdditiveBlending();

			gl::clear(ci::ColorA(0,0,0,0));
			InputView::render();

			gl::enableAlphaBlending();
		}
		mFbo1.unbindFramebuffer();
	glPopMatrix();

	float blur = mBlurAmount.value();

	glPushMatrix(); 
		//first pass
		mFbo2.bindFramebuffer();
			gl::clear(ci::ColorA(0,0,0));
			
			glEnable(GL_BLEND);
			gl::enableAlphaBlending();

			if(blur > 0) mShader->bind();
			
			ci::gl::Texture* texture = &mFbo1.getTexture();
			texture->bind(0);
			
			if(blur > 0){
				mShader->uniform("inBlurAmount", blur);
				mShader->uniform("inAlphaAmount", mAlphaAmount.value());
				mShader->uniform("texture", 0);
				mShader->uniform("direction", 0);
			}else{
				glColor4f(1.0f, 1.0f, 1.0f, mAlphaAmount.value()); 
			}

			gl::draw(*texture, ci::Vec2i(0,0) );
	
			texture->unbind(0);

			if(blur > 0) mShader->unbind();
		
		mFbo2.unbindFramebuffer();
	glPopMatrix();

}

void EngageGameView::render()
{
	
	float blur = mBlurAmount.value();
	ci::gl::Texture* texture = &mFbo2.getTexture();
	
	glPushMatrix(); 
		gl::enableAdditiveBlending();

		//second pass
		if(blur > 0) mShader->bind();
		
		texture->bind(0);

		if(blur > 0){
			mShader->uniform("inBlurAmount", blur);
			mShader->uniform("inAlphaAmount", mAlphaAmount.value());
			mShader->uniform("texture", 0);
			mShader->uniform("direction", 1);
		}

		gl::draw(*texture, ci::Vec2i(0,0) );
	
		texture->unbind(0);

		if(blur > 0) mShader->unbind();
	
		gl::enableAlphaBlending();

		//delete texture;
		texture = NULL;
	glPopMatrix();

	//draw text on top
	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x, scale.value().y, scale.value().z);
	
		mTimerText->render();
		mTimerStaticText->render();
		mTitleText->render(); 
	glPopMatrix();
}

void EngageGameView::checkGameResults()
{
	const int correctAnswer = 
		KeyType::TWO + KeyType::RIGHT +
		KeyType::SIX + KeyType::LEFT +
		KeyType::FOUR + KeyType::DOWN;

	ci::app::console() << "EngageGameView::checkGameResults() correctAnswer: " << correctAnswer << " mResult: " << mResult << std::endl;

	timeline().apply(mTitleText->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 
	timeline().apply(mTimerText->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 
	timeline().apply(mTimerStaticText->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 


	if( mResult == correctAnswer )
		mGameResuleSignal(GAME_WIN);
	else
		mGameResuleSignal(GAME_LOSE);
}

std::string EngageGameView::getTimerFormatted(float time)
{
	char number[24];
	sprintf(number, "%.2f", time);
	return toString(number);
}

void EngageGameView::setGameLetter(KeyType type)
{
	if(mIsPlaying)
	{
		SoundController::getInstance().playSound(SoundController::ENG_SELECT);

		KeypadButtonRef letter = KeypadButtonRef( new EngageGameKeypadButton(mAssets, type, true) );

		letter->setPosition(ci::Vec3f(mTotalLettersPlayed * 40.f + 10.f, 10.f, 0.f));
		mLetterContainer->addChild(letter);
		mGameLetters->push_back(letter);

		mResult += type;

		mTotalLettersPlayed++;

		if( mTotalLettersPlayed >= 6 )
		{
			mTimerCount = 0.f;
		}
	}
}

void EngageGameView::moveSelection(ci::Vec2i dir)
{
	if(!useSelection) return; 

	ci::Vec2i p = selectionPos + dir; 
	
	int sizeX = (int) selectionGrid.size();
	int sizeY = (int) selectionGrid[0].size();

	if( p.x < 0 ){
		p.x = 0; 
		//ci::app::console() << "InputView::moveSelection BORDER_LEFT" << std::endl;
		mInputStateSignal(this, InputState::BORDER_LEFT); 

	} else if( p.x >= sizeX ){
		p.x = selectionGrid.size() - 1;
		//ci::app::console() << "InputView::moveSelection BORDER_RIGHT" << std::endl;
		mInputStateSignal(this, InputState::BORDER_RIGHT); 
	}

	if( p.y < 0 ) {
		p.y = 0;
		//ci::app::console() << "InputView::moveSelection BORDER_UP" << std::endl;
		mInputStateSignal(this, InputState::BORDER_UP); 

	} else if( p.y >= sizeY ){
		p.y = sizeY - 1;
		//ci::app::console() << "InputView::moveSelection BORDER_DOWN" << std::endl;
		mInputStateSignal(this, InputState::BORDER_DOWN); 

	}

	if(selectionPos != p ) 
	{
		SoundController::getInstance().playSound(SoundController::ENG_MOVE);	
		setSelection(p);
	}
}

void EngageGameView::handleInput(INPUT_TYPE type)
{
	InputView::handleInput(type);

	KeypadButtonRef btn = boost::dynamic_pointer_cast<EngageGameKeypadButton>(selectedObject);

	switch(type)
	{
	case NEXT:
		break;
	case SELECT:
		if(btn) setGameLetter(btn->getKeyType());
		break;
            
        default:
            break;
	}
}