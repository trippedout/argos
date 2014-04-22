#pragma once
#include "ImageView.h"
#include <string>
#include <vector>
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"

//using namespace ci;
//using namespace ci::app;
//using namespace ci::gl;

typedef boost::shared_ptr<class PNGSequence> PNGSequenceRef;

class PNGSequence :
	public ImageView
{
protected:
	std::vector<ci::gl::Texture*> textures; 
	float time; 
	float duration;
	float framerate;
	float frameTime; 
	int frame; 
	int totalFrames; 

	bool looping, playing; 

public:
	PNGSequence(std::string path, int num, float frameRate);
	~PNGSequence(void);
	
	virtual void play();
	virtual void stop(); 
	virtual void setLoop(bool val){looping = val;}; 

	virtual void update(float dt); 

	virtual void setTime(float val){ time = val; if(time > duration) time = duration; if(time < 0) time = 0; }; 
};

