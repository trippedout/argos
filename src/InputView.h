#pragma once

#include "View.h"
#include "InputTypes.h"
#include "SelectableView.h"
#include "SoundController.h"

#include "ButtonView.h"
#include "ImageView.h"

#include "cinder/params/Params.h"

#ifdef _WIN32
#include <XInput.h>
#endif

#include "SoundController.h"

//using namespace ci::params; 

/*
Class that you should extend all main screens that receive input from a controller.
*/

typedef boost::shared_ptr<class InputView> InputViewRef;

class InputView : public View
{
	typedef boost::signals2::signal<void( InputView*, Settings::ViewID )> ChangeViewSignal;

	typedef SoundController::SoundId SoundId;

public:
	enum InputState { BORDER_RIGHT, BORDER_LEFT, BORDER_DOWN, BORDER_UP };
	typedef	boost::signals2::signal<void (InputView*, InputView::InputState)> InputStateSignal;

	typedef std::vector< ButtonViewRef >::iterator ButtonIterator;

	InputView(Assets* assets);
	virtual ~InputView(void);
	
	virtual void update(float dt);
	virtual void render();
	virtual void draw();

	// override of DisplayObjectContainers implementation to handle
	// adding objects to our selection vector
	void addChild(DisplayObjectRef object);
	
	//keeping this here for now but will eventually be part of selectable view
	virtual void	selectOld();
	ci::Vec2i			getSelectionPos(); 

	virtual void	moveSelection(ci::Vec2i dir);
	virtual void	setSelection(ci::Vec2i selection);

	virtual void	handleInput(INPUT_TYPE type) = 0;
    
#ifdef _WIN32
	virtual void	handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	
	virtual void	disposeTempAssets(); 
	
	virtual void	onAnimateIn(); 
	virtual void	onAnimateOut(); 
	virtual void	onAnimateIn(Settings::ViewID from); 
	virtual void	onAnimateOut(Settings::ViewID to); 

	ChangeViewSignal* getSignal() { return &signal; };
	InputStateSignal* getInputSignal() { return &mInputStateSignal; };
	
	//TODO: find a better solution. 
	// useSelection is kinda really shitty. 
	// its to stop the app from trying
	// to use the navigate selection stuff
	// if theres... nothing to navigate through. 
	bool useSelection; 

protected:
	//vector for right aligning text buttons
	ci::Vec3f*				mRightAlignVec3f;

	//for styling
	ImageViewRef		guide;
	virtual void		createGuide(std::string path);

	ci::params::InterfaceGlRef		params;
	virtual void                    createParams();

	//vector full of all the selectable views
	std::vector< DisplayObjectRef > selectableChildren;

	//currently selected view
	ButtonViewRef			selectedView;
	
	//method for deselecting all views
	virtual void		deselectAll();


	//for signaling a change of section
	ChangeViewSignal signal;

	//for signaling border of selection
	InputStateSignal mInputStateSignal;
	

	//actual grid for selection objects
	std::vector< std::vector< ButtonViewRef > > selectionGrid;

	//current and previous objects and positions
	ButtonViewRef prevSelectedObject;
	ButtonViewRef selectedObject; 
	ci::Vec2i prevSelectionPos;
	ci::Vec2i selectionPos;
	
	virtual void playMovementSound(ci::Vec2i selectionPos);


};

