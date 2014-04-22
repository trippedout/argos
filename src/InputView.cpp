#include "InputView.h"

using namespace ci;
using namespace ci::app;
using namespace ci::params;

InputView::InputView(Assets* assets) : 
	View(assets),
	prevSelectionPos(ci::Vec2i(-1,-1)),
	selectionPos(ci::Vec2i(-1,-1)),
	prevSelectedObject(NULL),
	selectedObject(NULL),
	useSelection(false),
	params(NULL),
	guide(NULL),
	mRightAlignVec3f(new ci::Vec3f(1.0f,0.5f,0))
{
	//set your guide first so it renders first!
	//createGuide("guides/guide.png");

	/////////
	//in all subclasses - do all your main shit here! AFTER guide and BEFORE params!
	/////////

	// remember to set params last!
	/*
	createParams();
	params->addParam("x", &title->getPosition()->value().x );
	params->addParam("y", &title->getPosition()->value().y );
	*/
}

InputView::~InputView(void)
{
	if(guide) guide.reset();
	if(params) params.reset();
}

void InputView::createGuide(std::string path)
{
	guide = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset(path) ) ) ) );
	this->addChild(guide);
}

void InputView::createParams()
{
	params = InterfaceGl::create( getWindow(), "Styling Params", toPixels( ci::Vec2i( 300, 120 ) ) );
}

void InputView::update(float dt)
{
	View::update(dt);

	//override and call me as super InputView::update(dt)
}

void InputView::disposeTempAssets()
{

}

void InputView::onAnimateIn()
{
	View::onAnimateIn();
}

void InputView::onAnimateOut()
{
	View::onAnimateOut();
}

void InputView::onAnimateIn(Settings::ViewID from)
{
	this->onAnimateIn();
}

void InputView::onAnimateOut(Settings::ViewID to)
{
	this->onAnimateOut();
}

//InputView::render() will automatically handle the 
//rendering of params if they exist

//override and call me as super InputView::render(dt)
void InputView::render()
{
	View::render();
	
	if(params) params->draw();
}

//override and call me as super InputView::draw(dt)
void InputView::draw()
{
	View::draw();
}

void InputView::deselectAll()
{
	for (ChildIterator it = selectableChildren.begin(); it != selectableChildren.end(); it++ )
	{
		boost::dynamic_pointer_cast<SelectableView>(*it)->selectView(false);
	}
}

void InputView::handleInput(INPUT_TYPE type)
{
	switch(type)
	{
	case DPAD_UP:
	case UP:
		this->moveSelection(ci::Vec2i(0,-1));
		break;

	case DPAD_DOWN:
	case DOWN:
		this->moveSelection(ci::Vec2i(0,1));
		break;

	case DPAD_LEFT:
	case LEFT:
		this->moveSelection(ci::Vec2i(-1,0));
		break;

	case DPAD_RIGHT:
	case RIGHT:
		this->moveSelection(ci::Vec2i(1,0));
		break;
            
        default:
            break;
	}
}

#ifdef _WIN32
void InputView::handleGamepad(XINPUT_GAMEPAD gamePad)
{

}
#endif

void InputView::selectOld()
{

}

void InputView::moveSelection(ci::Vec2i dir)
{
	if(!useSelection) return; 

	ci::Vec2i p = selectionPos + dir; 
	
	int sizeX = (int) selectionGrid.size();
	int sizeY = (int) selectionGrid[0].size();

	if( p.x < 0 ){
		p.x = 0; 
		//ci::app::console() << "InputView::moveSelection BORDER_LEFT" << std::endl;
		mInputStateSignal(this, InputState::BORDER_LEFT); 

	} else if( p.x >= sizeX ){
		p.x = selectionGrid.size() - 1;
		//ci::app::console() << "InputView::moveSelection BORDER_RIGHT" << std::endl;
		mInputStateSignal(this, InputState::BORDER_RIGHT); 
	}

	if( p.y < 0 ) {
		p.y = 0;
		//ci::app::console() << "InputView::moveSelection BORDER_UP" << std::endl;
		mInputStateSignal(this, InputState::BORDER_UP); 

	} else if( p.y >= sizeY ){
		p.y = sizeY - 1;
		//ci::app::console() << "InputView::moveSelection BORDER_DOWN" << std::endl;
		mInputStateSignal(this, InputState::BORDER_DOWN); 

	}

	if(selectionPos != p ) setSelection(p);
}

void InputView::setSelection(ci::Vec2i selection)
{
	if(selection == ci::Vec2i(-1,-1))
	{
		selectedObject = NULL;
		selectionPos = ci::Vec2i(-1,-1);
		this->deselectAll();
		return;
	}

	//even tho we're checking in moveSelection, some classes can call this method
	//directly so we need to make sure its not the same as currently selected
	if(selectionPos == selection ) return;


	//then make sure that its actually on the grid
	if(selectionGrid[selection.x][selection.y])
	{
		//set previous
		prevSelectedObject = selectedObject;
		prevSelectionPos = selectionPos;

		//set current
		selectedObject = selectionGrid[selection.x][selection.y];
		selectionPos = selection;
		
		//play sound if different than prev
		if(prevSelectedObject != NULL) playMovementSound(selection);

		//deselect all but selectedObject. 
		for (ChildIterator it = selectableChildren.begin(); it != selectableChildren.end(); it++ )
		{
			boost::shared_ptr<SelectableView> ptr = boost::dynamic_pointer_cast<SelectableView>(*it);

			if(ptr)
			{
				if( ptr != selectedObject ) 
					ptr->selectView(false);
				else 
					ptr->selectView(true);
			}
		}

	}
}

/*
Default movement sound implementation. Override if you want to handle sound yourself, like in Host A Party where 
left, right, up, and down all sound the same.
*/
void InputView::playMovementSound(ci::Vec2i selection)
{
	ci::Vec2i diff = selection - prevSelectionPos;
	
	if(diff.x == 0)
		SoundController::getInstance().playSound(SoundController::MOVE_VERTICAL);
	else if(diff.y == 0)
		SoundController::getInstance().playSound(SoundController::MOVE_HORIZONTAL);
}

ci::Vec2i InputView::getSelectionPos()
{
	return selectionPos; 
}

void InputView::addChild(DisplayObjectRef object)
{
	DisplayObjectContainer::addChild(object);

	//check to see if child is of selectable type, to add to our selection list
	boost::shared_ptr<SelectableView> ptr(boost::dynamic_pointer_cast<SelectableView>(object));
	if(ptr) selectableChildren.push_back(object);
}

