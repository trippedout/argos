#include "ScrubVideoObject.h"

using namespace ci::app;

ScrubVideoObject::ScrubVideoObject(ci::qtime::MovieGl* vid):
	VideoObject(vid),
	loop(false),
	playing(false),
	tweening(false),
	finished(false),
	time(0),
	loopStart(0),
	loopEnd(0),
	speed(1.0f),
	tweenTime( new ci::Anim<float>(0) ),
	firstTime(true),
	frameRate(30),
	frameUpdate(0)
{
}

ScrubVideoObject::~ScrubVideoObject(void)
{
	//VideoObject::~VideoObject();
}

void ScrubVideoObject::render()
{
	if(alpha->value() == 0) return; 
	VideoObject::render();
}

void ScrubVideoObject::update(float dt)
{
	if(alpha->value() == 0) return; 

	if(firstTime){
		firstTime = false;
		video->play(); 
		video->stop(); 
	}

	if(tweening){
		time = tweenTime->value();
	}else{
		if(playing) time += dt * speed; 
		
		if(loop){
			if( time > loopEnd ) time -= (loopEnd - loopStart);
			if( time < loopStart ) time += (loopEnd - loopStart);
		}
	}

	if(speed > 0.f)
	{
		if(time >= getDuration()) {
			time = getDuration();
			finished = true;
		}else{
			finished = false; 
		}
	}
	else
	{
		if(time <= 0.f) {
			time = 0.f;
			finished = true;
		}else{
			finished = false; 
		}
	}
	
	//only update at frameRate (default 30) FPS. 
	frameUpdate += dt; 
	if(frameUpdate > 1.0f/frameRate){
		frameUpdate -= 1.0f/frameRate;
		video->seekToTime(time);
	}
}


void ScrubVideoObject::tweenToTime(float val, float t)
{
	tweening = true;
	tweenTime->value() = time;
	timeline().apply(tweenTime, val, t ).finishFn(std::bind(&ScrubVideoObject::tweenComplete, this));
}

void ScrubVideoObject::tweenComplete()
{
	tweening = false; 
}

void ScrubVideoObject::setLoop(float start, float end)
{
	loop = true; 
	loopStart = start; 
	loopEnd = end; 
}

void ScrubVideoObject::stopLoop()
{
	loop = false;
}

void ScrubVideoObject::play()
{
	playing = true; 
}

void ScrubVideoObject::stop()
{
	playing = false;
}

void ScrubVideoObject::reset()
{
	time = 0; 
}

float ScrubVideoObject::getTime()
{
	return time; 
}

void ScrubVideoObject::setTime(float val)
{
	time = val;  
	if(time >= getDuration()) {
		time = getDuration();
		finished = true;
	}else{
		finished = false; 
	}
}

void ScrubVideoObject::nextScene()
{

}

void ScrubVideoObject::prevScene()
{	

}