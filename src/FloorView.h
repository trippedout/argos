#pragma once
#include "DisplayObject.h"
#include "ImageView.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "Assets.h"

typedef boost::shared_ptr<class FloorView> FloorViewRef;

class FloorView : public DisplayObject
{
protected: 
	
    ci::gl::Fbo	fbo;
	ci::gl::VboMesh* vbo; 
	ci::gl::GlslProgRef shader;
	
	ci::Vec3f meshRotation, meshPosition; 
	
	float angle; 

public:
	FloorView(Assets* assets);
	~FloorView(void);

	void drawQuad(); 
	virtual void prerender(); 
	virtual void render(); 
	virtual void draw(); 
	virtual void update(float dt);

	ci::Vec3f* getMeshRotation() { return &meshRotation; };
	ci::Vec3f* getMeshPosition() { return &meshPosition; };

};

                                                                            