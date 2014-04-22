#pragma once

#include "ImageView.h"
#include "cinder/qtime/QuickTime.h"

typedef boost::shared_ptr<class VideoObject> VideoViewRef;

class VideoObject : public ImageView
{
public:
	enum VideoEvent {
		VIDEO_END, HALFWAY,
	};
	
protected:
    ci::qtime::MovieGl*	video;
	std::vector< float > scenes;
	int scene; 
	bool playing; 
	
public:
	VideoObject(ci::qtime::MovieGl* vid);
	VideoObject(ci::qtime::MovieGl* vid, std::vector< float > sc);

	static VideoViewRef create(ci::qtime::MovieGl* vid);

	virtual void render(); 
	virtual void update(float dt); 

	virtual void play(); 
	virtual void stop(); 
	virtual void reset(); 

	virtual void nextScene(); 
	virtual void prevScene(); 

	//void deleteVideo(); 

	virtual ci::qtime::MovieGl* getVideo(); 
	virtual float getTime();
	virtual float getDuration(); 
	virtual void setTime(float val); 

	virtual void setInternalAlpha();

	virtual ~VideoObject(void);
};
