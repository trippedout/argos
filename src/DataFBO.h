#pragma once

#include "InputView.h"

#include "VideoObject.h"
#include "DataViewCard.h"
#include "DataController.h"

#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "cinder/gl/GlslProg.h"

class DataFBO : public InputView
{
	typedef boost::shared_ptr<DataViewCard> DataViewCardRef;

protected: 
	static const int FBO_WIDTH = 1920, FBO_HEIGHT = 1080;
    ci::gl::Fbo	fbo;
	ci::gl::GlslProgRef shader;
	VideoViewRef video; 

	std::vector<ImageViewRef> ticks; 
	DisplayObjectContainerRef tickHolder;

	DisplayObjectContainerRef holder;
	std::vector<DataViewCardRef> cards; 
	std::vector<DataViewCardRef>* currentCards; 

	int currentCard;
	ci::Vec3f currentPos, nextPos, prevPos, currentRot, nextRot, prevRot; 

	DataController* dataController; 

	void updatePositions(bool snap);
	//CardShPtr		mDataCard;
	float animationDelay; 

public:
	DataFBO(Assets* assets, VideoViewRef vid);
	~DataFBO(void);

	void next(); 
	void prev();

	virtual void update(float dt);
	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	virtual void render();
	virtual void renderBasic();
	virtual void renderShader();
	virtual void prerender(); 

	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 

	bool showLeft(); 
	bool showRight(); 

	virtual void onContentInputBorder(InputView* view, InputView::InputState state);
};

