#include "HandbookBackgroundView.h"

using namespace ci;
using namespace ci::app;

HandbookBackgroundView::HandbookBackgroundView(Assets* assets) :
	InputView(assets),
	object3D(NULL), nextObject3D(NULL), 
	platformOpen(false), platformAdded(false),
	mFloor( HandbookFloorRef( new HandbookFloorView(assets) ) ),
	sphereBG(ImageViewRef( new ImageView(assets->getTextureByID("hb_sphere")) )),
	platformBG( ImageViewRef( new ImageView(assets->getTextureByID("hb_platform")) )),
	sphere(SphereViewRef( new SphereView(assets) )),
	platform(ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("handbook_platform")) )),
	beam(ImageViewRef(new ImageView(assets->getTextureByID("hb_beam")))),
	colorBlur(HandbookColorBlurViewRef(new HandbookColorBlurView(assets)))
{

	bgBox = ImageViewRef( new ImageView(assets->getTextureByID("bgBox")) );
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setAlpha(0);

	createGuide("bgs/lower_hb_ bg_beam.png");
	
	beam->setPosition( ci::Vec3f(880.0f, 60.0f, 0) ); 
	beam->setAdditiveBlending(true);
	beam->setAlpha(0);

	sphere->setPosition( ci::Vec3f(1920.0f - sphere->getSize()->value().x, 0, 0) ); 
	
	sphereBG->setPosition( ci::Vec3f(1920.0f - sphere->getSize()->value().x, 0, 0) );
	sphereBG->setAlpha(0); 

	
	platformBG->setCenter(ci::Vec3f( 0.5f, 0.5f, 0.5f )); 
	platformBG->setPosition( ci::Vec3f( 1400.0f, 860.0f, 0 ) );
	platformBG->setScale(ci::Vec3f( 1.25f, 1.25f, 1.25f ) ); 
	platformBG->setAlpha(0); 

	platform->setAdditiveBlending(true); 
	platform->setScale(ci::Vec3f( -1.25f, 1.25f, 1.25f ) ); 
	platform->setCenter(ci::Vec3f( 0.5f, 0.5f, 0.5f )); 
	platform->setPosition( ci::Vec3f( 1400.0f, 860.0f, 0 ) );
	platform->setSpeed(1.25f);
	
	addChild(mFloor);
	addChild(sphereBG);
	addChild(beam);
	addChild(sphere); 
	addChild(colorBlur); 
	addChild(platformBG);
	addChild(bgBox);
}

HandbookBackgroundView::~HandbookBackgroundView(void)
{
	mFloor.reset();
	sphereBG.reset();
	platformBG.reset();
	sphere.reset();
	platform.reset();
	beam.reset();
	colorBlur.reset();
}

void HandbookBackgroundView::handleInput(INPUT_TYPE type)
{

}

#ifdef _WIN32
void HandbookBackgroundView::handleGamepad( XINPUT_GAMEPAD gamepad )
{
	if( object3D ) object3D->handleGamepad(gamepad); 
}
#endif

void HandbookBackgroundView::onAnimateIn()
{
	sphere->setIdle(true);

	if( object3D ){
		object3D->disposeTempAssets(); 
		removeChild(object3D);
		object3D = NULL;
	}

	platformOpen = false; 
	platform->stopLoop();
	if(platformAdded){
		platformAdded = false;
		removeChild(platform);
	}

	bgBox->setAlpha(1.0f);
	timeline().apply( bgBox->getAlpha(), 0.0f, 0.5f, EaseOutQuint() );

	timeline().apply(colorBlur->getAlpha(), 1.0f, 1.0f, EaseOutQuint());
	sphere->onAnimateIn(); 
	timeline().apply( sphereBG->getAlpha(), 0.9f, 0.5f, EaseOutQuint() );

	
	mFloor->pingVerts();
}

void HandbookBackgroundView::onAnimateOut()
{
	sphere->setIdle(true);

	ci::app::console() << "HandbookBackgroundView::onAnimateOut" << std::endl;

	if( object3D ){
		object3D->disposeTempAssets(); 
		removeChild(object3D);
		object3D = NULL;
	}

	nextObject3D = NULL;
	
	timeline().apply( bgBox->getAlpha(), 1.0f, 0.5f, EaseInQuint() );

	timeline().apply( sphereBG->getAlpha(), 0.0f, 0.5f, EaseInQuint() );
	sphere->onAnimateOut(); 
	
	timeline().apply(colorBlur->getAlpha(), 0.0f, 0.5f, EaseOutQuint());

	onAnimateOutComplete(); 
}

void HandbookBackgroundView::update(float dt)
{
	InputView::update(dt);

	if( object3D && object3D->canRemove() ){
		
		if(nextObject3D == object3D){
			ci::app::console() << "HandbookBackgroundView::update  next object3d is same as current, animating it back in." << std::endl;
			nextObject3D = NULL;
			object3D->onAnimateIn();
		}else{		
			ci::app::console() << "HandbookBackgroundView::update  removing current object3D" << std::endl;
			object3D->disposeTempAssets(); 
			removeChild(object3D);
			object3D = NULL; 
	
			if(nextObject3D){
				ci::app::console() << "\t adding next object3D" << std::endl;
				object3D = nextObject3D;
				addChild(object3D);
				object3D->onAnimateIn(); 
				nextObject3D = NULL;
			}
		}

	}

	//when the platform is done animating, remove it. 
	if(platformAdded && platform->isFinished()){
		platformAdded = false; 
		removeChild(platform); 
	}
}

void HandbookBackgroundView::showPlatform()
{
	if(!platformOpen){
		platformOpen = true; 
		platform->setTime(0);
		platform->setLoop(4.0f, 12.0f);
		platform->tweenToTime(2.0f, 2.0f);
		platform->play(); 

		platformAdded = true; 
		addChild(platform); 
		
		timeline().removeTarget(platform->getAlpha());

		timeline().apply( platformBG->getAlpha(), 0.75f, 1.0f, EaseOutQuint() ).delay(3.0f);
		timeline().apply( platform->getAlpha(), 1.0f, 0.1f, EaseOutQuint() );

		timeline().apply( beam->getAlpha(), 1.0f, 0.5f, EaseOutQuint() );
		timeline().appendTo(beam->getAlpha(), 0.75f, 0.1f, EaseInQuint());
		timeline().appendTo(beam->getAlpha(), 1.0f, 0.1f, EaseOutQuint());
		timeline().appendTo(beam->getAlpha(), 0.9f, 0.1f, EaseInQuint());
		timeline().appendTo(beam->getAlpha(), 1.0f, 0.1f, EaseOutQuint());
		timeline().appendTo(beam->getAlpha(), 0.9f, 0.1f, EaseInQuint());
		timeline().appendTo(beam->getAlpha(), 1.0f, 0.2f, EaseOutQuint());

		mFloor->pingVerts();
	}
	
	timeline().apply( sphereBG->getAlpha(), 0.0f, 0.5f, EaseInQuint() );
	sphere->onAnimateOut();
}

void HandbookBackgroundView::hidePlatform()
{
	if(platformOpen){
		platformOpen = false; 
		//platform->tweenToTime(16.0f, 4.0f);
		platform->stopLoop(); 

		timeline().removeTarget(platform->getAlpha());

		timeline().apply( platform->getAlpha(), 0.0f, 0.5f, EaseInOutQuint() ).delay(3.0f).finishFn( boost::bind(&HandbookBackgroundView::showSphere, this) );
		timeline().apply( platformBG->getAlpha(), 0.0f, 0.5f, EaseOutQuint() ).delay(3.0f);
		timeline().apply( beam->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).delay(2.5f);
		
		timeline().apply(colorBlur->getAlpha(), 0.25f, 1.0f, EaseOutQuint()).delay(2.5f);
		timeline().appendTo(colorBlur->getAlpha(), 1.0f, 1.0f, EaseOutQuint()).delay(0.5f);
	}
}

void HandbookBackgroundView::showSphere()
{
	sphere->onAnimateIn();
	timeline().apply( sphereBG->getAlpha(), 0.9f, 0.5f, EaseInQuint() );
}

void HandbookBackgroundView::setObject3D(HandbookView3dObjectRef val)
{
	if( object3D ){
		if(val == object3D){
			ci::app::console() << "HandbookBackgroundView::setObject3D  next object3d is same as current, doin nothing." << std::endl;
		}else{
			ci::app::console() << "HandbookBackgroundView::setObject3D  animating out current object3D" << std::endl;
			object3D->onAnimateOut();
			nextObject3D = val;
		}
		
	}else{
		ci::app::console() << "HandbookBackgroundView::setObject3D  adding directly" << std::endl;
		nextObject3D = NULL; 
		object3D = val;
		addChild(object3D);
		object3D->onAnimateIn(); 
	}
}

void HandbookBackgroundView::hideObject3D()
{
	ci::app::console() << "HandbookBackgroundView::hideObject3D" << std::endl;
	if(object3D){
		object3D->hide(); 	
	}
	timeline().apply(colorBlur->getAlpha(), 0.25f, 0.5f, EaseOutQuint());
}

void HandbookBackgroundView::showObject3D()
{
	ci::app::console() << "HandbookBackgroundView::showObject3D" << std::endl;
	if(object3D){
		object3D->show(); 	
		timeline().apply(colorBlur->getAlpha(), 1.0f, 0.5f, EaseOutQuint());
	}
}

void HandbookBackgroundView::setConstellation(int val)
{
	sphere->setConstellation(val); 
}

void HandbookBackgroundView::removeObject3D()
{
	if( object3D ) {
		ci::app::console() << "HandbookBackgroundView::removeObject3D" << std::endl;
		object3D->onAnimateOut();
	}

	nextObject3D = NULL; 
}


void HandbookBackgroundView::prerender()
{
	if( object3D != NULL ) object3D->prerender(); 
	mFloor->prerender();
	sphere->prerender();
}