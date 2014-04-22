#pragma once

#include "DisplayObject.h"
#include "cinder/gl/Texture.h"

typedef boost::shared_ptr<class ImageView> ImageViewRef;

class ImageView: public DisplayObject
{
protected:
	ci::Anim<ci::Vec3f> color;
	ci::Rectf renderRect; 
	ci::gl::Texture* texture;
	bool additiveBlending; 
	bool mProject3D;
	

public:
	ImageView(); 
	ImageView(ci::gl::Texture* tex); 
	ImageView(ci::gl::Texture* tex, ci::Vec3f pos, ci::Vec3f cent);
	ImageView(ci::gl::Texture* tex, ci::Rectf rect); 

	virtual ~ImageView(void);

	virtual ci::gl::Texture* getTexture();	
	virtual void setTexture(ci::gl::Texture* tex); 
	
	virtual ci::Anim<ci::Vec3f>* getColor();
	virtual void setColor(ci::Vec3f val); 

	virtual ci::Rectf* getRenderRect();
	virtual void setRenderRect(ci::Rectf val);

	virtual void render(); 
	virtual void draw();
	virtual void setAdditiveBlending(bool val); 

	virtual void deleteTexture();
	
	virtual void drawQuad(); 

	void project3D(bool val){mProject3D = val;}; 
};
