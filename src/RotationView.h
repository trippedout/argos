#pragma once
#include "DisplayObject.h"
#include "ImageView.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include <thread>

class RotationView :
	public DisplayObject
{
public:
	RotationView();
	RotationView(std::string assetPath, ci::gl::GlslProgRef glShader);
	~RotationView(void);

	virtual void disposeTempAssets(); 

	virtual void prerender(); 
	virtual void render(); 
	virtual void draw(); 
	virtual void update(float dt);
	virtual void setSpeed(float val); 
	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 

	virtual void show();
	virtual void hide();

	bool canRemove(){ return animOutComplete; }; 

protected: 

	std::thread* loaderThread;

    ci::gl::Fbo	fbo;
	ci::qtime::MovieSurfaceRef video;
	ci::gl::GlslProgRef shader;

	bool FBOReady, videoReady, threadLoading, deleteAfterLoad, animOutComplete, hidden; 
	float speed, time, duration, angle, delay, frameRate, frameUpdate;
	std::string assetPath;
	
	virtual void threadLoaded(); 
};

                                                                            