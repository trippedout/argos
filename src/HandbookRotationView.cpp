#include "HandbookRotationView.h"

using namespace ci;
using namespace ci::app;

HandbookRotationView::HandbookRotationView(std::string path, ci::gl::GlslProgRef glShader):RotationView()
{
	assetPath = path + ".mp4"; 

	tweenTime = new ci::Anim<float>(0);

	FBOReady = false; 
	videoReady = false;
	threadLoading = false; 
	deleteAfterLoad = false; 
	looping = false; 
	animOutComplete = false; 
	hidden = false;
	tweening = false;
	
	angle = 0; 
	introTime = 4.0f;
	loopTime = 8.0f;
	endTime = 4.0f;

	duration = 16.0f;

	shader = glShader;
	
	delay = 0; 
	time = 0; 
	speed = 2.0f;
	
	setAlpha(0);
}

void HandbookRotationView::onAnimateIn()
{
	ci::app::console()<< "HandbookRotationView::onAnimateIn" << std::endl; 
	hidden = false; 

	if(video && FBOReady){
		timeline().apply(getAlpha(), 1.0f, 0.25f, EaseOutCubic());
	}else{
		delay = 2.25f; 
	}

	playIn();
}

void HandbookRotationView::onAnimateOut()
{
	ci::app::console()<< "HandbookRotationView::onAnimateOut" << std::endl; 
	if(hidden){
		//already hidden... no need to animate it out. 
		ci::app::console()<< "\t object already hidden, skipping animateOut" << std::endl; 
		hidden = false; 
		looping = false; 
		time = introTime + loopTime;
		animOutComplete = true; 
	}else{
		playOut();
	}

}
 
void HandbookRotationView::playIn()
{
	tweening = false;
	animOutComplete = false; 
	looping = false; 
	time = 0;
}

void HandbookRotationView::playOut()
{
	looping = false; 
	
	if(time < introTime + loopTime){
		//time = introTime + loopTime;
		tweening = true;
		tweenTime->value() = time;
		timeline().apply(tweenTime, introTime + loopTime, ( (introTime + loopTime) - time ) * 0.25f ).finishFn(std::bind(&HandbookRotationView::tweenOutComplete, this));

	}else{
		// do nothing, its already animating out. 
	}
}

void HandbookRotationView::tweenOutComplete()
{
	looping = false;
	tweening = false;
	time = introTime + loopTime;
}


void HandbookRotationView::update(float dt)
{
	if(delay > 0) delay-=dt; 

	if(delay <=0 && !videoReady && !threadLoading && !video){
		threadLoading = true; 
		loaderThread = new std::thread( std::bind(&HandbookRotationView::threadLoaded, this));
	}

	if( !video || !video->checkPlayable() || alpha->value() == 0) return; 

	angle += dt * 5.0f; 
	if(angle > (float)M_PI * 2.0f) angle -= (float)M_PI*2.0f;

	if(looping){
		time += dt * speed; 
		if ( time < introTime ) time += loopTime; 
		if ( time > introTime + loopTime )  time -= loopTime; 

	}else{
		if(tweening){
			time = tweenTime->value();
		}else{
			time += dt * speed;
		 
			if ( time > introTime && time < introTime + loopTime ) {
				//intro animation has finished.
				looping = true; 
			}
		}

		if ( time >= duration ){
			//end animation has finished... should probably do something here. 
			ci::app::console()<< "HandbookRotationView::update animation out complete" << std::endl; 
			time = duration; 
			animOutComplete = true;
			hidden = false; 
			setAlpha(0);
		}
	}
	
	//only update at frameRate (default 30) FPS. 
	frameUpdate += dt; 
	if(frameUpdate > 1.0f/frameRate){
		frameUpdate = 0;
		video->seekToTime(time);
	}
}

void HandbookRotationView::setSpeed(float val)
{
	//stop speed changes when looping. 

	if(looping) speed = val; 
	else speed = 2.0f; 
}

HandbookRotationView::~HandbookRotationView(void)
{

}
