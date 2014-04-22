#pragma once
#include "VideoObject.h"

typedef boost::shared_ptr<class ScrubVideoObject> ScrubVideoRef;

class ScrubVideoObject :
	public VideoObject
{
protected:
	ci::Anim<float>* tweenTime; 
	float speed, time, loopStart, loopEnd, frameUpdate, frameRate; 
	bool loop, playing, tweening, finished; 
	bool firstTime; 

public:
	ScrubVideoObject(ci::qtime::MovieGl* vid);
	~ScrubVideoObject(void);

	virtual void render(); 
	virtual void update(float dt); 

	virtual void tweenComplete(); 

	virtual void tweenToTime(float val, float t); 
	virtual void setLoop(float start, float end); 
	virtual void stopLoop(); 

	virtual void play(); 
	virtual void stop(); 
	virtual void reset(); 

	virtual void nextScene(); 
	virtual void prevScene(); 
	virtual float getTime();
	virtual void setTime(float val); 

	float getFrameRate(){return frameRate;}; 
	void setFrameRate(float val){frameRate = val;};

	void setSpeed(float val){speed = val;}; 
	float getSpeed(){return speed;}; 

	bool isFinished(){return finished;}; 
};

