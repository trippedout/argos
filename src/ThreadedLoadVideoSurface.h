#pragma once

#include "DisplayObjectContainer.h"
#include "VideoObject.h"
#include <thread>

typedef boost::shared_ptr<class ThreadedLoadVideoSurface> ThreadedLoadVideoSurfaceRef;

class ThreadedLoadVideoSurface : public DisplayObjectContainer
{
protected:
	VideoViewRef video;
	bool playing, threadLoading, deleteAfterLoad, additiveBlending, autoPlay;
	std::string assetPath; 
	//std::thread* loaderThread;

	virtual void threadLoaded();

public:
	ThreadedLoadVideoSurface(std::string assetPath);
	virtual ~ThreadedLoadVideoSurface(void);

	virtual void render(); 
	virtual void update(float dt); 

	virtual void play(); 
	virtual void stop(); 
	virtual void reset(); 

	virtual void nextScene(); 
	virtual void prevScene(); 

	virtual float getTime();
	virtual float getDuration(); 
	virtual void setTime(float val); 

	virtual void loadVideo(); 
	virtual void deleteVideo();

	virtual bool isPlaying(){return playing;};
	virtual void setAdditiveBlending(bool val){additiveBlending = val;}; 
	virtual void setAutoPlay(bool val){autoPlay = val;}; 
};
