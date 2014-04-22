#pragma once

#include "InputView.h"

#include "ImageView.h"
#include "LocationMapPOI.h"

#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/CinderMath.h"

class LocationLocalViewMap : public InputView
{
private:
	enum MapPositions {
		LOCKER_ROOM, DORMS, COURTYARD, TRAINING_ROOM, MEHAFEY, TETHER, MEDICAL_BAY, MESS_HALL, FULL_VIEW
	};
	
	typedef std::map<MapPositions, std::pair<ci::Vec3f,ci::Vec3f>>::iterator MapIter;
	typedef std::map<MapPositions, DisplayObjectContainerRef>::iterator HighlightIter;

	std::map<MapPositions, std::pair<ci::Vec3f,ci::Vec3f>>	mMapPositions;
	MapIter									mMapIter;

	std::map<MapPositions, DisplayObjectContainerRef>	mHighlightedImages;

protected:	
	static const int	FBO_WIDTH = 1920, FBO_HEIGHT = 1080;
	
	std::vector<DisplayObjectRef>	mImages;
	DisplayObjectContainerRef	mMapContainer;
	DisplayObjectContainerRef	mHighlightContainer;

    ci::gl::TextureRef					mVignetteAlpha;
	ci::gl::TextureRef					mVignetteBlur;

	ci::gl::GlslProgRef					mShader;
		
	//camera
    ci::CameraPersp		mCam;
	ci::Anim<ci::Vec3f>		mCamPos;
	ci::Anim<ci::Vec3f>		mLookAt;
	ci::Vec3f			mUp;
    ci::gl::Fbo				mFbo1, mFbo2;

	//pois
	std::vector<LocationMapPOIRef>*	mPOIs;

	//unused images
	std::vector<ci::Vec2i>*			mUnusedImages;
	void createUnusedImages();
	bool checkFrameForContent(int x, int y);

	//for creation/positioning
	int						mGridRows;
	int						mImageSize;

	//for animation and control
	bool					mIsAnimating;
	float					mRotAngle;
	float					mRotDistance;
	ci::Vec2f					getRotationNormal();
	void					setCameraRotation(float rotation);

	void createPositionsList();
	void createShader();
	void createPOIs();
	void createHighlights();
	void createCamera();
	void createMap();
	
	void renderMaps();
	void renderShaderPasses();

public:
	LocationLocalViewMap(Assets* assets);
	~LocationLocalViewMap(void);

	virtual void update(float dt);
	virtual void render();
	virtual void handleInput(INPUT_TYPE type);
    
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

	void animateCameraTo(ci::Vec3f camPos, ci::Vec3f lookAt);
	void animateCameraTo(std::pair<ci::Vec3f,ci::Vec3f> camPair);
	
	void setMapPosition(const std::string &mapId);
	void setHighlight(MapPositions position);
	void hideHighlights();
	void setCameraPosition(std::pair<ci::Vec3f,ci::Vec3f> camPair);

	void onUpdateRotation();
	void onAnimateCameraComplete();
	
	float getCameraRotation();

	float mapf(float value, float start1, float stop1, float start2, float stop2) 
	{
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	};
};

