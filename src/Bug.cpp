#include "Bug.h"

using namespace ci;
using namespace ci::app;

Bug::Bug(Assets* assets) : DisplayObjectContainer()
{
	state = BugState::HIDDEN; 
	voiceActive = false;
	angle = 0;

	dataController = assets->getData(); 
	itemsUnlocked= charactersUnlocked = locationsUnlocked = onisUnlocked = 0; 
	blue = ci::Vec3f(0.067f, 0.69f, 0.98f );
	yellow = ci::Vec3f(0.98f, 0.796f, 0.067f);
	
	itemColor = ci::Vec3f(0.9f, 0.7f, 0.01f);
	characterColor = ci::Vec3f(0.03f, 0.52f, 0.94f);
	locationColor = ci::Vec3f(0.36f, 0.77f, 0.36f);

	int size = 4;
	int i; 
	for( i=0; i<size; i++){
		bars.push_back( BugElemDisplayObjectRef(new BugElement( assets->getTextureByID("bug_bar"), assets->getTextureByID("bug_bar_glow") )));
		bars[i]->setRotation(ci::Vec3f(0, 0, -90.0f * i)); 
		bars[i]->select();
		bars[i]->setGlowColor(yellow); 
		addChild(bars[i]);
	}

	for( i=0; i<size; i++){
		arrows.push_back( BugElemDisplayObjectRef(new BugElement( assets->getTextureByID("bug_arrow"), assets->getTextureByID("bug_arrow_glow") )));
		arrows[i]->setRotation(ci::Vec3f(0, 0, -90.0f * i)); 
		arrows[i]->select();
		arrows[i]->setGlowColor(blue); 
		addChild(arrows[i]);
	}

	//add 5 extra dots for the voice shit
	for( i=0; i<size + 5; i++){
		dots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_dot") ) ));
		dots[i]->setCenter( ci::Vec3f(0.5f, 0.5f, 0) );
		dots[i]->setAlpha( 0 );
		dots[i]->setAdditiveBlending(true);
		addChild(dots[i]);
	}


	oniGlow = BugElemDisplayObjectRef(new BugElement( assets->getTextureByID("bug_oni"), assets->getTextureByID("bug_oni_glow") ));
	oniGlow->hide();
	oniGlow->setGlowColor(yellow);
	addChild(oniGlow);

	dots[0]->setPosition(ci::Vec3f(0,25,0));
	dots[1]->setPosition(ci::Vec3f(25,0,0));
	dots[2]->setPosition(ci::Vec3f(0,-25,0));
	dots[3]->setPosition(ci::Vec3f(-25,0,0));

	base = ImageViewRef(new ImageView( assets->getTextureByID("bug_base") ) );
	base->setCenter( ci::Vec3f(0.5f, 0.5f, 0) );
	base->setAdditiveBlending(true);
	addChild(base);

	/*
	std::vector<ImageViewRef> glowDots;  
	addTexture("bug_progress_glow", new ci::gl::Texture( loadImage(loadAsset("bug/bug_progress_glow.png"))) );
	*/

	progressBase = ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_base") ) );
	progressBase->setCenter( ci::Vec3f(0.5f, 0.5f, 0) );
	progressBase->setAdditiveBlending(true);
	addChild(progressBase);

	for( i=0; i<size; i++){
		onis.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_highlight") ) ));
		onis[i]->setCenter( ci::Vec3f(0.5f, 0.5f, 0) );
		onis[i]->setRotation(ci::Vec3f(0, 0, 90.0f * i)); 
		onis[i]->setAlpha( 0.0f );
		onis[i]->setAdditiveBlending(true);
		addChild(onis[i]);
	}

	
	progressBG = ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_bg") ) );
	progressBG->setCenter( ci::Vec3f(1.0f, 0.5f, 0) );
	progressBG->setPosition( ci::Vec3f(10.0f, 0, 0) );
	progressBG->setAdditiveBlending(true);
	progressBG->setAlpha(0);
	addChild(progressBG);

	item = TextViewRef(new TextView("ITEMS", assets->getFontByID("bug_progress"), ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f)));
	item->setPosition( ci::Vec3f(-350.0f,-50.0f + 3.0f,0) );
	item->setAlpha(0);
	item->setAdditiveBlending(true);
	addChild(item);

	character = TextViewRef(new TextView("CHARACTERS", assets->getFontByID("bug_progress"), ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f)));
	character->setPosition( ci::Vec3f(-350.0f,-25.0f + 3.0f,0) );
	character->setAlpha(0);
	character->setAdditiveBlending(true);
	addChild(character);

	location = TextViewRef(new TextView("LOCATIONS", assets->getFontByID("bug_progress"), ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f)));
	location->setPosition( ci::Vec3f(-350.0f, 0 + 3.0f,0) );
	location->setAlpha(0);
	location->setAdditiveBlending(true);
	addChild(location);

	oni = TextViewRef(new TextView("ONI SYMBOLS", assets->getFontByID("bug_progress"), ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f)));
	oni->setPosition( ci::Vec3f(-350.0f,25.0f + 3.0f,0) );
	oni->setAlpha(0);
	oni->setAdditiveBlending(true);
	addChild(oni);

	itemNum = TextViewRef(new TextView("0/5", assets->getFontByID("bug_progress"), ci::ColorA(0.9f, 0.7f, 0.01f, 1.0f)));
	itemNum->setPosition( ci::Vec3f(-130.0f,-50.0f + 3.0f,0) );
	itemNum->setCenter( ci::Vec3f(1.0f,0,0) );
	itemNum->setAlpha(0);
	itemNum->setAdditiveBlending(true);
	addChild(itemNum);

	characterNum = TextViewRef(new TextView("0/1000", assets->getFontByID("bug_progress"), ci::ColorA(0.03f, 0.52f, 0.94f, 1.0f)));
	characterNum->setPosition( ci::Vec3f(-130.0f,-25.0f + 3.0f,0) );
	characterNum->setCenter( ci::Vec3f(1.0f,0,0) );
	characterNum->setAlpha(0);
	characterNum->setAdditiveBlending(true);
	addChild(characterNum);

	locationNum = TextViewRef(new TextView("0/100", assets->getFontByID("bug_progress"), ci::ColorA(0.36f, 0.77f, 0.36f, 1.0f)));
	locationNum->setPosition( ci::Vec3f(-130.0f,0 + 3.0f,0) );
	locationNum->setCenter( ci::Vec3f(1.0f,0,0) );
	locationNum->setAlpha(0);
	locationNum->setAdditiveBlending(true);
	addChild(locationNum);

	oniNum = TextViewRef(new TextView("0/10", assets->getFontByID("bug_progress"), ci::ColorA(0.69f, 0.85f, 0.97f, 1.0f)));
	oniNum->setPosition( ci::Vec3f(-130.0f,25.0f + 3.0f,0) );
	oniNum->setCenter( ci::Vec3f(1.0f,0,0) );
	oniNum->setAlpha(0);
	oniNum->setAdditiveBlending(true);
	addChild(oniNum);


	float x, y, ox, oy;
	//all them dots. 
	//right top
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(-1,-62,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(4,-62,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	size = 14;
	x = 8; y=-61; ox = 4; oy = 4; 
	for( i=0; i<size; i++){
		glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(x,y,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
		x+= ox; y+= oy; 
	}

	//right bottom
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(62,-5,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(62,-1,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(62,4,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	x= 60; y=9; ox = -4; oy = 4; 
	for( i=0; i<size; i++){
		glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(x,y,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
		x+= ox; y+= oy; 
	}
	
	//left bottom
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(4,62,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(-1,62,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(-5,62,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	x = -10; y=61; ox = -4; oy = -4; 
	for( i=0; i<size; i++){
		glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(x,y,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
		x+= ox; y+= oy; 
	}

	//left top
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(-62,4,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(-62,-1,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(-62,-5,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
	x = -62; y=-9; ox = 4; oy = -4; 
	for( i=0; i<size; i++){
		glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(x,y,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );
		x+= ox; y+= oy; 
	}
	//last dot on the top. 
	glowDots.push_back( ImageViewRef(new ImageView( assets->getTextureByID("bug_progress_glow"), ci::Vec3f(-5,-62,0), ci::Vec3f(0.5f, 0.5f, 0) ) ) );

	size = glowDots.size();
	for(i=0; i<size; i++){
		glowDots[i]->setAdditiveBlending(true);
		glowDots[i]->setAlpha(0);
		addChild(glowDots[i]);
	}

	hide(); 
	
}


void Bug::checkProgress()
{
	DataController::UnlockedInfo info = dataController->getUnlocked(); 

	onisUnlocked = info.onis; 
	itemsUnlocked = info.items; 
	charactersUnlocked = info.characters; 
	locationsUnlocked = info.locations; 

	itemNum->setText( ci::toString(itemsUnlocked) + "/" + ci::toString(info.itemsTotal) ); 
	characterNum->setText( ci::toString(charactersUnlocked) + "/" + ci::toString(info.charactersTotal) ); 
	locationNum->setText( ci::toString(locationsUnlocked) + "/" + ci::toString(info.locationsTotal) ); 
	oniNum->setText( ci::toString(onisUnlocked) + "/" + ci::toString(info.onisTotal) ); 
}


void Bug::selectDown()
{
	if(voiceActive) return; 

	//ci::app::console() << "[Bug](selectDown)" << std::endl;
	idle();
	arrows[0]->select();
	bars[0]->pulse();
	bars[3]->pulse();
	timeline().apply( dots[0]->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
}

void Bug::selectRight()
{
	if(voiceActive) return; 

	idle();
	arrows[1]->select();
	bars[0]->pulse();
	bars[1]->pulse();
	timeline().apply( dots[1]->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
}

void Bug::selectUp()
{
	if(voiceActive) return; 

	idle();
	arrows[2]->select();
	bars[1]->pulse();
	bars[2]->pulse();
	timeline().apply( dots[2]->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
}

void Bug::selectLeft()
{
	if(voiceActive) return; 

	idle();
	arrows[3]->select();
	bars[0]->pulse();
	bars[3]->pulse();
	timeline().apply( dots[3]->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
}

void Bug::pulseDown(bool isOni)
{
	if(voiceActive) return;
	pulseDirection(isOni, 0);
}

void Bug::pulseRight(bool isOni)
{
	if(voiceActive) return; 
	pulseDirection(isOni, 1);
}

void Bug::pulseUp(bool isOni)
{
	if(voiceActive) return; 
	pulseDirection(isOni, 2);
}

void Bug::pulseLeft(bool isOni)
{
	if(voiceActive) return;
	pulseDirection(isOni, 3);
}

void Bug::pulseDirection(bool isOni, int arrowIndex)
{	
	for(int  i=0; i<4; i++){
		bars[i]->fancyPulse(0);
	}

	if(isOni){
		pulseOni();
		arrows[arrowIndex]->setGlowColor(yellow);
		arrows[arrowIndex]->fancyGlowPulse3X(0);
		arrows[arrowIndex]->fancyGlowPulse(0);
	}
	else{
		arrows[arrowIndex]->setGlowColor(blue);
		arrows[arrowIndex]->fancyGlowPulse(1.0f);
	}
}

void Bug::pulseOni()
{
	oniGlow->fancyGlowPulse3X(1.0f);
}

void Bug::update(float dt)
{
	DisplayObjectContainer::update(dt);

	angle += dt * (float)M_PI*2.0f; 
	float max = (float)M_PI*2.0f;
	if(angle > max) angle -= max;

	if(voiceActive){
		for( unsigned int i=0; i<dots.size(); i++){
			float s = sin(angle) * 0.4 + 0.6; 
			if(i >= 5) s = cos(angle) * 0.4 + 0.6; 
			dots[i]->setScale(ci::Vec3f(s,s,s)); 
		}
	}
}

void Bug::voiceOn()
{
	idle(); 

	for( unsigned int i=0; i<dots.size(); i++){
		timeline().apply( dots[i]->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	}

	int size = 4;

	for( int i=0; i<size; i++){
		arrows[i]->hide();
	}
	for( int i=0; i<size; i++){
		bars[i]->selectGlowLoop(0);
	}
	for( int i=0; i<size; i++){
		timeline().apply( onis[i]->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	}
	
	timeline().apply( dots[5]->getPosition(), ci::Vec3f(-15,-15,0), 1.0f, EaseOutQuint() );
	timeline().apply( dots[6]->getPosition(), ci::Vec3f(-15,15,0), 1.0f, EaseOutQuint() );
	timeline().apply( dots[7]->getPosition(), ci::Vec3f(15,-15,0), 1.0f, EaseOutQuint() );
	timeline().apply( dots[8]->getPosition(), ci::Vec3f(15,15,0), 1.0f, EaseOutQuint() );


	voiceActive = true;
}

void Bug::voiceSuccess()
{
}

void Bug::voiceOff()
{
	voiceActive = false;
	
	for( unsigned int i=0; i<dots.size(); i++){
		timeline().apply( dots[i]->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
		timeline().apply( dots[i]->getScale(), ci::Vec3f(1.0f,1.0f,1.0f), 1.0f, EaseOutQuint() );
	}
	timeline().apply( dots[5]->getPosition(), ci::Vec3f(0,0,0), 1.0f, EaseOutQuint() );
	timeline().apply( dots[6]->getPosition(), ci::Vec3f(0,0,0), 1.0f, EaseOutQuint() );
	timeline().apply( dots[7]->getPosition(), ci::Vec3f(0,0,0), 1.0f, EaseOutQuint() );
	timeline().apply( dots[8]->getPosition(), ci::Vec3f(0,0,0), 1.0f, EaseOutQuint() );

	switch(state){
	case BugState::HIDDEN:
		hide();
		break;

	case BugState::PROGRESS:
		progress();
		break;

	case BugState::IDLE:
	default:
		idle();
		break;
	}
}

void Bug::hide()
{
	state = BugState::HIDDEN; 

	//ci::app::console() << "[Bug](hide)" << std::endl;
	if(voiceActive) return; 

	int size = 4;
	int i; 

	timeline().apply( base->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	timeline().apply( progressBase->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );

	for( i=0; i<size; i++){
		timeline().apply( dots[i]->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	}
	for( i=0; i<size; i++){
		arrows[i]->hide();
	}
	for( i=0; i<size; i++){
		bars[i]->hide();
	}
	for( i=0; i<size; i++){
		timeline().apply( onis[i]->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	}

	size = glowDots.size();
	for(i=0; i<size; i++){
		timeline().apply( glowDots[i]->getAlpha(), 0.0f, 0.3f, EaseOutQuint() ).delay(i/100.0f);
	}
	
	timeline().apply( progressBG->getPosition(), ci::Vec3f(10.0f,0,0), 0.5f, EaseInQuint() );
	timeline().apply( progressBG->getAlpha(), 0.0f, 0.5f, EaseInQuint() );

	timeline().apply( item->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.1f);
	timeline().apply( itemNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.1f);

	timeline().apply( character->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.15f);
	timeline().apply( characterNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.15f);

	timeline().apply( location->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.2f);
	timeline().apply( locationNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.2f);

	timeline().apply( oni->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.25f);
	timeline().apply( oniNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.25f);
}


void Bug::idle()
{

	state = BugState::IDLE; 

	if(voiceActive) return; 

	int size = 4;
	int i; 

	timeline().apply( base->getAlpha(), 0.75f, 1.0f, EaseOutQuint() );
	timeline().apply( progressBase->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );

	for( i=0; i<size; i++){
		timeline().apply( dots[i]->getAlpha(), 0.3f, 1.0f, EaseOutQuint() );
	}
	for( i=0; i<size; i++){
		arrows[i]->hide();
		arrows[i]->setGlowColor(blue); 
	}
	for( i=0; i<size; i++){
		bars[i]->hide();
	}
	for( i=0; i<size; i++){
		timeline().apply( onis[i]->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	}
	
	size = glowDots.size();
	for(i=0; i<size; i++){
		timeline().apply( glowDots[i]->getAlpha(), 0.0f, 0.3f, EaseOutQuint() ).delay(i/100.0f);
	}

	timeline().apply( progressBG->getPosition(), ci::Vec3f(10.0f,0,0), 0.5f, EaseInQuint() );
	timeline().apply( progressBG->getAlpha(), 0.0f, 0.5f, EaseInQuint() );

	timeline().apply( item->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.1f);
	timeline().apply( itemNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.1f);

	timeline().apply( character->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.15f);
	timeline().apply( characterNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.15f);

	timeline().apply( location->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.2f);
	timeline().apply( locationNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.2f);

	timeline().apply( oni->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.25f);
	timeline().apply( oniNum->getAlpha(), 0.0f, 0.25f, EaseInQuint() ).delay(0.25f);

}


void Bug::progress()
{
	state = BugState::PROGRESS; 

	if(voiceActive) return; 

	//ci::app::console() << "[Bug](progress)" << std::endl;
	checkProgress(); 

	int size = 4;
	int i; 

	timeline().apply( base->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	timeline().apply( progressBase->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );

	for( i=0; i<size; i++){
		timeline().apply( dots[i]->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	}
	for( i=0; i<size; i++){
		arrows[i]->hide();
	}
	for( i=0; i<size; i++){
		bars[i]->hide();
	}

	for( i=0; i<onisUnlocked; i++){
		timeline().apply( onis[i]->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.5f + i/15.0f);
	}

	size = itemsUnlocked + charactersUnlocked + locationsUnlocked;
	for(i=0; i<size; i++){
		if(i < itemsUnlocked ) glowDots[i]->setColor(itemColor); 
		else if(i < itemsUnlocked + charactersUnlocked  )glowDots[i]->setColor(characterColor); 
		else glowDots[i]->setColor(locationColor); 

		timeline().apply( glowDots[i]->getAlpha(), 1.0f, 0.3f, EaseOutQuint() ).delay(0.5f + i/50.0f);
	}
	
	timeline().apply( progressBG->getPosition(), ci::Vec3f(0,0,0), 1.0f, EaseOutQuint() );
	timeline().apply( progressBG->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );

	timeline().apply( item->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.1f);
	timeline().apply( itemNum->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.1f);

	timeline().apply( character->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.15f);
	timeline().apply( characterNum->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.15f);

	timeline().apply( location->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.2f);
	timeline().apply( locationNum->getAlpha(),1.0f, 1.0f, EaseOutQuint() ).delay(0.2f);

	timeline().apply( oni->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.25f);
	timeline().apply( oniNum->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(0.25f);
}

Bug::~Bug(void)
{
}
