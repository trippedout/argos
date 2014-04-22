#include "HostButtonView.h"

using namespace ci;
using namespace ci::app;

HostButtonView::HostButtonView(Assets* assets, std::string copy, std::string iconID, bool lead):ARGOSButtonView(assets, copy)
{
	acceptTimer = 0; 
	rot = 0;

	invited = false; 
	leader = lead; 

	state = HostButtonState::IDLE;

	red = ci::Vec3f(0.92f, 0.09f, 0.08f); 
	green = ci::Vec3f(0.13f, 1.0f, 0.22f); 
	blue = ci::Vec3f(0, 0.5f, 0.99f); 
	yellow = ci::Vec3f(0.92f, 0.85f, 0.07f); 

	bg1->setSize(ci::Vec3f(560.0f,65.0f,0));
	bg2->setSize(ci::Vec3f(560.0f,65.0f,0));

	bg3->setSize(ci::Vec3f(560.0f,65.0f,0));

	//text = new TextView(copy, assets->getFontByID("default"), ci::ColorA(1,1,1,1));
	text->setCenter(ci::Vec3f(0,0,0)); 
	text->setPosition(ci::Vec3f(160.0f, 16.0f, 0)); 
	text->setScale(ci::Vec3f(0.8f, 0.8f, 0.8f)); 

	if(leader){
		leaderText = TextViewRef( new TextView("/ LEADER", assets->getFontByID("ARGOS_button"), ci::ColorA(0.9f,0.7f,0,1)) );
		leaderText->setPosition(ci::Vec3f(160.0f + text->getSize()->value().x - 4.0f, 16.0f, 0)); 
		leaderText->setScale(ci::Vec3f(0.8f, 0.8f, 0.8f)); 
	}

	icon	= ImageViewRef( new ImageView(assets->getTextureByID(iconID)) );
	iconBG1 = ImageViewRef( new ImageView(assets->getTextureByID("host_iconbg")) );
	iconBG2 = ImageViewRef( new ImageView(assets->getTextureByID("host_iconbg")) );

	spinner = SpriteSheetViewRef( new SpriteSheetView(assets->getTextureByID("host_spinner"), ci::Vec2i(64,64), 60, 60 ) );
	spinner->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	spinner->setPosition(ci::Vec3f(iconBG1->getSize()->value().x/2, iconBG1->getSize()->value().y/2, 0)); 
	spinner->setAlpha(0);
	spinner->setAdditiveBlending(true);

	icon->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	icon->setPosition(ci::Vec3f(iconBG1->getSize()->value().x/2, iconBG1->getSize()->value().y/2, 0)); 

	iconBG2->setPosition(ci::Vec3f(-10.0f, -10.0f, 0)); 

	iconBG1->setAlpha(0.3f);

	if(leader){
		iconBG1->setColor(blue); 
		iconBG2->setAlpha(0);
	}else{
		iconBG1->setColor(yellow); 
		iconBG2->setColor(red); 
		iconBG2->setAlpha(0.15f);
	}

	iconBG1->setAdditiveBlending(true);
	iconBG2->setAdditiveBlending(true);
	icon->setAdditiveBlending(true);

	addChild(bg1); 
	addChild(bg2); 
	addChild(text); 

	if(leader) addChild(leaderText); 

	addChild(iconBG1); 
	addChild(iconBG2); 
	addChild(icon); 
	addChild(spinner); 
	addChild(bg3);
}

void HostButtonView::activate()
{
	if(leader) return; 

	invited = !invited;
	
	if(invited)
	{
		timeline().apply( iconBG1->getColor(), blue, 0.3f, EaseOutQuint() );
		timeline().apply( iconBG2->getColor(), blue, 0.3f, EaseOutQuint() );
		timeline().apply( iconBG2->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
		timeline().apply( iconBG2->getPosition(), ci::Vec3f(0, 0, 0), 0.3f, EaseOutQuint() );
		timeline().apply( icon->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
		timeline().apply( spinner->getAlpha(), 1.0f, 0.3f, EaseOutQuint() );
		acceptTimer = 2.0f + (float)rand() / (float)RAND_MAX; 

	}else{
		timeline().apply( iconBG1->getColor(), yellow, 0.3f, EaseOutQuint() );
		timeline().apply( iconBG2->getColor(), red, 0.3f, EaseOutQuint() );
		timeline().apply( iconBG2->getAlpha(), 0.15f, 0.3f, EaseOutQuint() );
		timeline().apply( iconBG2->getPosition(), ci::Vec3f(-10.0f, -10.0f, 0), 0.3f, EaseOutQuint() );
		timeline().apply( icon->getAlpha(), 1.0f, 0.3f, EaseOutQuint() );
		timeline().apply( spinner->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
		acceptTimer = 0; 
	}
}

void HostButtonView::selectView(bool select)
{
	ARGOSButtonView::selectView(select); 

	ci::Vec3f p1 = ci::Vec3f(0, 0, 0); 
	ci::Vec3f p2 = ci::Vec3f(-10.0f, -10.0f, 0); 
	ci::Vec3f p3 = ci::Vec3f(-15.0f, -15.0f, 0); 
	ci::Vec3f p4 = ci::Vec3f(5.0f, 5.0f, 0); 

	if(select){
		timeline().apply( iconBG1->getPosition(), p4, 0.3f, EaseOutQuint() );

		if(invited){
			timeline().apply( iconBG2->getPosition(), p1, 0.3f, EaseOutQuint() );
		}else{
			timeline().apply( iconBG2->getPosition(), p3, 0.3f, EaseOutQuint() );
		}
	}else{
		timeline().apply( iconBG1->getPosition(), p1, 0.3f, EaseOutQuint() );

		if(invited){
			timeline().apply( iconBG2->getPosition(), p1, 0.3f, EaseOutQuint() );
		}else{
			timeline().apply( iconBG2->getPosition(), p2, 0.3f, EaseOutQuint() );
		}
	}
}

void HostButtonView::update(float dt)
{
	ARGOSButtonView::update(dt);

	if(acceptTimer <= 0) return; 

	acceptTimer -= dt; 

	rot+= dt * 500.0f;
	if( rot > 360.0f) rot -= 360.0f;
	//spinner->setRotation(ci::Vec3f(0,0,rot));


	if(acceptTimer<=0){
		if( (float)rand() / (float)RAND_MAX > 0.5f){
			//accept
			timeline().apply( iconBG1->getColor(), green, 0.3f, EaseOutQuint() );
			timeline().apply( iconBG2->getColor(), green, 0.3f, EaseOutQuint() );
		}else{
			//reject
			timeline().apply( iconBG1->getColor(), red, 0.3f, EaseOutQuint() );
			timeline().apply( iconBG2->getColor(), red, 0.3f, EaseOutQuint() );
		}

		timeline().apply( iconBG2->getAlpha(), 0.15f, 0.3f, EaseOutQuint() );
		if(selected){
			timeline().apply( iconBG2->getPosition(), ci::Vec3f(-15.0f, -15.0f, 0), 0.3f, EaseOutQuint() );
		}else{
			timeline().apply( iconBG2->getPosition(), ci::Vec3f(-10.0f, -10.0f, 0), 0.3f, EaseOutQuint() );
		}
		timeline().apply( icon->getAlpha(), 1.0f, 0.3f, EaseOutQuint() );
		timeline().apply( spinner->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
	}
}

void HostButtonView::deactivate()
{
}

HostButtonView::~HostButtonView(void)
{
}
