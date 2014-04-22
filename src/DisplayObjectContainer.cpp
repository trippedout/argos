#include "DisplayObjectContainer.h"

DisplayObjectContainer::DisplayObjectContainer(void) : DisplayObject()
{

}

DisplayObjectContainer::~DisplayObjectContainer(void)
{
}

void DisplayObjectContainer::update(float dt)
{
	if(children.empty()) return;

	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
		(*it)->update(dt);
}

void DisplayObjectContainer::render()
{
	if(children.empty()) return;

	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x, scale.value().y, scale.value().z);

		for ( ChildIterator it = children.begin(); it != children.end(); it++ ){
			if((*it)->getAlpha()->value() > 0){
				(*it)->render();
			}
		}

	glPopMatrix();
}

void DisplayObjectContainer::draw()
{
	DisplayObject::draw();
}

void DisplayObjectContainer::addChild(DisplayObjectRef object)
{
	//attempt to remove object so we can place back on top 
	if(contains(object)) removeChild(object);

	//add to all children container
	children.push_back( object );
	
	//set the parent for alpha and transformation addition
	object->setParent(this);
}

void DisplayObjectContainer::removeChild(DisplayObjectRef object)
{
	//TODO: test if this actually works? 
	// found it in an example... need to see if that actually does anything. 

	//ci::app::console() << "DOC::removeChild() before: " << children.size();

	children.erase(std::remove(children.begin(), children.end(), object ), children.end());
	
	//ci::app::console() << " after: " << children.size() << " isEmpty? " << children.empty() << std::endl;
}

bool DisplayObjectContainer::contains(DisplayObjectRef object)
{
	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
		if( (*it) == object ) return true; 

	return false; 
}

void DisplayObjectContainer::setPosition(ci::Vec3f val)
{
	position = val; 
	/*
	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
	{
		(*it)->setPosition(val);
	}
	*/
}

void DisplayObjectContainer::setRotation(ci::Vec3f val)
{
	rotation = val;
	/*
	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
	{
		(*it)->setRotation(val);
	}
	*/
}

void DisplayObjectContainer::setScale(ci::Vec3f val)
{
	scale = val; 
	/*
	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
	{
		(*it)->setScale(val);
	}
	*/
}

void DisplayObjectContainer::setSize(ci::Vec3f val)
{
	size = val; 
	/*
	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
	{
		(*it)->setSize(val);
	}
	*/
}

void DisplayObjectContainer::setCenter(ci::Vec3f val)
{
	//TODO: remove this. 
	// you should just set the center paramater of the children. 
	// conceptually, a basic DOContainer shouldn't actually have a centerpoint, 
	// any child class that extends it might have to implement it, but whatefs. 
	center = val; 

	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
	{
		(*it)->setCenter(val);
	}
}

void DisplayObjectContainer::setAlpha(float val)
{
	DisplayObject::setAlpha(val);

	for ( ChildIterator it = children.begin(); it != children.end(); it++ )
	{
		//try to cast child to DisplayObjectContainer and set alpha if non-null
		DisplayObjectContainerRef ptr = boost::dynamic_pointer_cast<DisplayObjectContainer>(*it);
		if(ptr)ptr->setAlpha(val);
	}
}

/*
Hacky workaround for how timeline() in the Tween engine works.
Since we can't pass parameters to our callback functions, and apply() can only
affect a reference to an ci::Anim<T>, we must call setAlpha as an updateFn (since its void())
to make sure that our getAlpha updates from apply() are passed to all the children.
*/
void DisplayObjectContainer::setInternalAlpha()
{
	this->setAlpha(this->getAlpha()->value());
}