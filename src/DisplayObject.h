#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<class DisplayObject> DisplayObjectRef;

class DisplayObject
{
protected:
	ci::Anim<ci::Vec3f> position, rotation, scale, size;
	ci::Vec3f center;
	ci::Anim<float>* alpha;
	DisplayObject* parent;
	ci::ColorA* color;

public:
	DisplayObject(void);
	virtual ~DisplayObject(void);

	virtual void update(float dt);
	virtual void render(); 
	virtual void draw();

	ci::Anim<ci::Vec3f>* getPosition();
	ci::Anim<ci::Vec3f>* getRotation();
	ci::Anim<ci::Vec3f>* getScale();

	virtual void setPosition( ci::Vec3f val );
	virtual void setRotation( ci::Vec3f val );
	virtual void setScale( ci::Vec3f val );

	virtual void setCenter( ci::Vec3f val );
	ci::Vec3f* getCenter();

	virtual void setSize( ci::Vec3f val );
	ci::Anim<ci::Vec3f>* getSize();

	virtual void setAlpha(float val);
	ci::Anim<float>* getAlpha();

	virtual void setParent(DisplayObject* parent);
	DisplayObject* getParent();

};

