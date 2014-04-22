#include "DisplayObject.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace std;

DisplayObject::DisplayObject(void) :
	position( ci::Vec3f(0.f,0.f,0.f) ),
	rotation( ci::Vec3f(0.f,0.f,0.f) ),
	scale( ci::Vec3f(1.0f,1.0f,1.0f) ),
	alpha( new ci::Anim<float>(1.0f) ),
	parent(NULL)
{
	
}

DisplayObject::~DisplayObject(void)
{

}

void DisplayObject::update(float dt)
{
	//override
}

// --------------------------------------------------------------------------------
// render is solely responsible for the initial positioning of the whole displayobject. 
// if your overall position/rotation/scale change at all, override this function
// other children positioning or matrix work is meant to be done in the draw function
// --------------------------------------------------------------------------------
void DisplayObject::render()
{
	if(alpha->value() <= 0) return; 

	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x, scale.value().y, scale.value().z);
		
		/*
		this is brought into any object that extends DisplayObject and NOT DisplayObjectContainer
		that wants to deal with overriding render() itself:

		if(parent == NULL)
			glColor4f(color.value().x, color.value().y, color.value().z, alpha->value());
		else
			glColor4f(color.value().x, color.value().y, color.value().z, alpha->value() * parent->getAlpha()->value());

		check ImageView.cpp for implementation details
		*/

		draw();

	glPopMatrix();
	
}

void DisplayObject::draw()
{
	//override me!
}

ci::Anim<ci::Vec3f>* DisplayObject::getPosition()
{
	return &position;
}

ci::Anim<ci::Vec3f>* DisplayObject::getRotation()
{
	return &rotation; 
}

ci::Anim<ci::Vec3f>* DisplayObject::getScale()
{
	return &scale; 
}

void DisplayObject::setPosition(ci::Vec3f val)
{
	position = val; 
}

void DisplayObject::setRotation(ci::Vec3f val)
{
	rotation = val;
}

void DisplayObject::setScale(ci::Vec3f val)
{
	scale = val; 
}

ci::Anim<ci::Vec3f>* DisplayObject::getSize()
{
	return &size; 
}

void DisplayObject::setSize(ci::Vec3f val)
{
	size = val; 
}

ci::Vec3f* DisplayObject::getCenter()
{
	return &center; 
}

void DisplayObject::setCenter(ci::Vec3f val)
{
	center = val; 
}

void DisplayObject::setAlpha(float val)
{
	/*if(parent != NULL) 
	{
		ci::app::console() << "DisplayObject::setAlpha() parent exists!" << std::endl;
		alpha->value() = parent->getAlpha()->value();
	}
	else*/
	alpha->value() = val;
}

ci::Anim<float>* DisplayObject::getAlpha()
{
	return alpha;
}

void DisplayObject::setParent(DisplayObject* parent)
{
	this->parent = parent;
}
	
DisplayObject* DisplayObject::getParent()
{
	return this->parent;
}
