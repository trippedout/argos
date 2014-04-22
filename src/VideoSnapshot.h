#pragma once
#include "Assets.h"
#include "DisplayObject.h"
#include "VideoObject.h"

#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

typedef boost::shared_ptr<class VideoSnapshot> VideoSnapshotRef;

class VideoSnapshot :
	public DisplayObject
{
protected: 
	VideoViewRef video; 
	ci::gl::Fbo	fbo;
	ci::gl::GlslProgRef shader;
	ci::Anim<float>	blur;

public:
	VideoSnapshot(VideoViewRef vid, Assets* assets);
	~VideoSnapshot(void);

	virtual void render(); 
	virtual void draw(); 
	virtual void updateSnapshot(); 

	void setBlur(float val){blur.value() = val;}; 
	ci::Anim<float>* getBlur(){return &blur;}; 
};