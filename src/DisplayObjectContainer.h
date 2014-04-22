#pragma once

#include "Displayobject.h"

typedef boost::shared_ptr<class DisplayObjectContainer> DisplayObjectContainerRef;

class DisplayObjectContainer : public DisplayObject
{
protected:
	typedef std::vector< DisplayObjectRef >::iterator ChildIterator;
	std::vector< DisplayObjectRef > children;

public:
	
	DisplayObjectContainer(void);
	virtual ~DisplayObjectContainer(void);
 
	virtual void update(float dt);
	virtual void render();
	virtual void draw();

	// responsible for adding/removing children to the child vector; also virtual so that 
	// inputView can override with its selectable items
	virtual void addChild(DisplayObjectRef object); 
	virtual void removeChild(DisplayObjectRef object);
	bool contains(DisplayObjectRef object);
	
	virtual void setPosition( ci::Vec3f val ); 
	virtual void setRotation( ci::Vec3f val ); 
	virtual void setScale( ci::Vec3f val );  

	virtual void setCenter( ci::Vec3f val ); 
	virtual void setSize( ci::Vec3f val ); 
	virtual void setAlpha(float val);
	virtual void setInternalAlpha();

	//TODO: addChildAt(index), getChildAt(index), etc..

};

