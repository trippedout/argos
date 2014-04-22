#include "LocationLocalViewMap.h"

using namespace ci::app;
using namespace ci::gl;
using namespace ci;

LocationLocalViewMap::LocationLocalViewMap(Assets* assets) : 
	InputView(assets),
	mGridRows(8),
	mImageSize(1024),
	mIsAnimating(false),
	mRotAngle(0.f),
	mRotDistance(0.f)
{
	createPositionsList();
	createUnusedImages();
	createShader();
	createPOIs();
	createCamera();
	createMap();
	createHighlights();
	
	mVignetteAlpha	=	TextureRef( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/vignette_alpha.png" ) ) ) );
	mVignetteBlur	=	TextureRef( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/vignette_blur.png" ) ) ) );

	/*createParams();
	params->addParam("camPos", &mCamPos.value());
	params->addParam("camLookAt", &mLookAt.value());*/
}

LocationLocalViewMap::~LocationLocalViewMap(void)
{
	mUnusedImages->clear();
	delete mUnusedImages;

	mVignetteAlpha->reset();
	mVignetteAlpha.reset();

	mVignetteBlur->reset();
	mVignetteBlur.reset();

	for(int i = 0; i < mImages.size(); ++i)
	{
		mImages[i].reset();
	}

	if(params) params.reset();
}


void LocationLocalViewMap::createPositionsList()
{
	mMapPositions[MapPositions::LOCKER_ROOM]	= std::make_pair( ci::Vec3f(5000, 350, 2400), ci::Vec3f(5000, 0, 3200) );
	mMapPositions[MapPositions::DORMS]			= std::make_pair( ci::Vec3f(4950, 400, 6100), ci::Vec3f(4490, 0, 5400) );
	mMapPositions[MapPositions::COURTYARD]		= std::make_pair( ci::Vec3f(1500, 500, 5150), ci::Vec3f(2200, 0, 4450) );
	mMapPositions[MapPositions::TRAINING_ROOM]	= std::make_pair( ci::Vec3f(780, 450, 1650), ci::Vec3f(1700, 0, 1650) );
	mMapPositions[MapPositions::MEHAFEY]		= std::make_pair( ci::Vec3f(-500, 350, 3245), ci::Vec3f(250, 0, 3245) );
	mMapPositions[MapPositions::TETHER]			= std::make_pair( ci::Vec3f(3315, 450, 2500), ci::Vec3f(3315, 0, 3500) );
	mMapPositions[MapPositions::MEDICAL_BAY]	= std::make_pair( ci::Vec3f(6300, 375, 1450), ci::Vec3f(5800, 0, 2075) );
	mMapPositions[MapPositions::MESS_HALL]		= std::make_pair( ci::Vec3f(4550, 350, -350), ci::Vec3f(4550, 0, 600) );
	mMapPositions[MapPositions::FULL_VIEW]		= std::make_pair( ci::Vec3f(5500, 1200, 1750), ci::Vec3f(3315, 0, 3500) );

	mMapIter = mMapPositions.begin();

	setCameraPosition(mMapPositions[MapPositions::LOCKER_ROOM]);
}

void LocationLocalViewMap::setCameraPosition(std::pair<ci::Vec3f,ci::Vec3f> camPair)
{
	mCamPos = camPair.first;
	mLookAt = camPair.second;

	onAnimateCameraComplete();
}

void LocationLocalViewMap::setMapPosition(const std::string &mapId)
{
	ci::app::console() << "LocationLocalViewMap::setMapPosition() " << mapId << std::endl;

	hideHighlights();

	if(mapId.empty()) 
		setCameraPosition( mMapPositions[MapPositions::TETHER] );
	else
	{
		if(mapId == "local_medical_bay")
		{
			setCameraPosition(mMapPositions[MapPositions::MEDICAL_BAY]);
			setHighlight(MapPositions::MEDICAL_BAY);
		}
		else if(mapId == "local_dorm")
		{
			setCameraPosition(mMapPositions[MapPositions::DORMS]);
			setHighlight(MapPositions::DORMS);
		}		
		else if(mapId == "local_mess_hall") 
		{
			setCameraPosition(mMapPositions[MapPositions::MESS_HALL]);
			setHighlight(MapPositions::MESS_HALL);
		}		
		else if(mapId == "local_mehaffeys_office")
		{
			setCameraPosition(mMapPositions[MapPositions::MEHAFEY]);
			setHighlight(MapPositions::MEHAFEY);
		}	
		else if(mapId == "local_outer_wall")
		{
			setCameraPosition(mMapPositions[MapPositions::COURTYARD]);
			setHighlight(MapPositions::COURTYARD);
		}	
		else if(mapId == "local_locker_room")
		{
			setCameraPosition(mMapPositions[MapPositions::LOCKER_ROOM]);
			setHighlight(MapPositions::LOCKER_ROOM);
		}
		else if(mapId == "local_tether")
		{
			setCameraPosition(mMapPositions[MapPositions::TETHER]);
			setHighlight(MapPositions::TETHER);
		}
	}
}

void LocationLocalViewMap::setHighlight(MapPositions position)
{
	if(mHighlightedImages[position])
		mHighlightedImages[position]->setAlpha(1.f);
}

void LocationLocalViewMap::hideHighlights()
{
	for(HighlightIter iter = mHighlightedImages.begin(); iter != mHighlightedImages.end(); iter++) iter->second->setAlpha(0.f);
}

void LocationLocalViewMap::createShader()
{
	try
	{
		mShader = GlslProg::create( loadAsset("shaders/map.vert"), loadAsset("shaders/map.frag") );
	}
	catch(GlslProgCompileExc error)
	{
		ci::app::console() << error.what();
		mShader = NULL;
	}
}

void LocationLocalViewMap::createPOIs()
{
	//pois
	mPOIs = new std::vector<LocationMapPOIRef>;
	ci::Vec3f rotVec = ci::Vec3f(-180,0,0);

	LocationMapPOIRef lockerRoom = LocationMapPOIRef( new LocationMapPOI(mAssets, "LOCKER ROOM") );
	lockerRoom->setPosition(mMapPositions[MapPositions::LOCKER_ROOM].second);
	lockerRoom->setRotation(rotVec);
	mPOIs->push_back(lockerRoom);

	LocationMapPOIRef dorms = LocationMapPOIRef( new LocationMapPOI(mAssets, "DORMS") );
	dorms->setPosition(mMapPositions[MapPositions::DORMS].second);
	dorms->setRotation(rotVec);
	mPOIs->push_back(dorms);

	LocationMapPOIRef courtyard = LocationMapPOIRef( new LocationMapPOI(mAssets, "COURTYARD") );
	courtyard->setPosition(mMapPositions[MapPositions::COURTYARD].second);
	courtyard->setRotation(rotVec);
	mPOIs->push_back(courtyard);

	LocationMapPOIRef trainingRoom = LocationMapPOIRef( new LocationMapPOI(mAssets, "TRAINING ROOM") );
	trainingRoom->setPosition(mMapPositions[MapPositions::TRAINING_ROOM].second);
	trainingRoom->setRotation(rotVec);
	mPOIs->push_back(trainingRoom);

	LocationMapPOIRef mehafey = LocationMapPOIRef( new LocationMapPOI(mAssets, "MEHAFFEY") );
	mehafey->setPosition(mMapPositions[MapPositions::MEHAFEY].second);
	mehafey->setRotation(rotVec);
	mPOIs->push_back(mehafey);

	LocationMapPOIRef tether = LocationMapPOIRef( new LocationMapPOI(mAssets, "TETHER") );
	tether->setPosition(mMapPositions[MapPositions::TETHER].second);
	tether->setRotation(rotVec);
	mPOIs->push_back(tether);

	LocationMapPOIRef messhall = LocationMapPOIRef( new LocationMapPOI(mAssets, "MESS HALL") );
	messhall->setPosition(mMapPositions[MapPositions::MESS_HALL].second);
	messhall->setRotation(rotVec);
	mPOIs->push_back(messhall);

	LocationMapPOIRef medicalBay = LocationMapPOIRef( new LocationMapPOI(mAssets, "MEDICAL BAY") );
	medicalBay->setPosition(mMapPositions[MapPositions::MEDICAL_BAY].second);
	medicalBay->setRotation(rotVec);
	mPOIs->push_back(medicalBay);
}

void LocationLocalViewMap::createCamera()
{	
	//create FBOs for pingponging in the shader
	gl::Fbo::Format format;
	format.enableMipmapping(true);
	format.setSamples( 4 ); // uncomment this to enable 4x antialiasing
	
	mFbo1 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );
	mFbo2 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );

	//setup camera
	mCam.setPerspective(60.f, getWindowAspectRatio(), 5.f, 10000.f);
	mCamPos = ci::Vec3f(4250.f, 250.f, 4250.f);
	mLookAt = ci::Vec3f(3750.f, 0.f, 3750.f);
	mUp = ci::Vec3f::yAxis();
}

void LocationLocalViewMap::createMap()
{	
	//create all textures necessary and place them in grid
	mMapContainer = DisplayObjectContainerRef( new DisplayObjectContainer );
	ci::gl::Texture::Format format;
	format.enableMipmapping(true);

	for(int y = 0; y < mGridRows; ++y)
	{
		for( int x = 0; x < mGridRows; ++x )
		{
			if(checkFrameForContent(y,x))
			{
				std::string filename = "scenes/location/map/images/";

				int i = (y * mGridRows) + x + 1;
			
				if( i < 10 ) 
					filename.append( "map_0" + std::to_string(i) );
				else
					filename.append( "map_" + std::to_string(i) );

				filename.append(".png");

				ci::app::console() << "new image: " << filename << std::endl;

				ImageViewRef ptr = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( filename ) ), format ) ) );
				ptr->setPosition(ci::Vec3f(x * mImageSize, y * mImageSize, 0));
				
				mImages.push_back(ptr);
				mMapContainer->addChild(ptr);
			}
		}
	}

	//rotate the container 90 degrees so it lays flat and in proper orientation
	mMapContainer->setRotation(ci::Vec3f(90,0,0));
}

void LocationLocalViewMap::createHighlights()
{
	mHighlightContainer = DisplayObjectContainerRef( new DisplayObjectContainer );

	DisplayObjectContainerRef container; 
	ImageViewRef highlight, outline;

	//locker room
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_locker_room.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 4435, 2873, -1 ));
	container->addChild(highlight);

	/*outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	container->addChild( outline );*/

	mHighlightedImages[MapPositions::LOCKER_ROOM] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::LOCKER_ROOM]);
	

	//training room
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_training_room.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 1303, 1449, -1 ));
	container->addChild(highlight);

	//outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	//outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	//container->addChild( outline );

	mHighlightedImages[MapPositions::TRAINING_ROOM] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::TRAINING_ROOM]);


	//tether
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_tether.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 2623, 2783, -1 ));
	container->addChild(highlight);

	//outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	//outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	//container->addChild( outline );

	mHighlightedImages[MapPositions::TETHER] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::TETHER]);


	//mehafey
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_mehafey.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 3, 3050, -1 ));
	container->addChild(highlight);

	//outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	//outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	//container->addChild( outline );

	mHighlightedImages[MapPositions::MEHAFEY] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::MEHAFEY]);


	//medical
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_medical_bay.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 5614, 1698, -1 ));
	container->addChild(highlight);

	//outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	//outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	//container->addChild( outline );

	mHighlightedImages[MapPositions::MEDICAL_BAY] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::MEDICAL_BAY]);


	//dorms
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_dorms.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 3534, 4984, -1 ));
	container->addChild(highlight);

	//outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	//outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	//container->addChild( outline );

	mHighlightedImages[MapPositions::DORMS] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::DORMS]);


	//dorms
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_courtyard.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 1481, 3611, -1 ));
	container->addChild(highlight);

	//outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	//outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	//container->addChild( outline );

	mHighlightedImages[MapPositions::COURTYARD] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::COURTYARD]);


	//mess hall
	container = DisplayObjectContainerRef( new DisplayObjectContainer );
	highlight = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/active_messhall.png" ) ) ) ) );
	highlight->setPosition(ci::Vec3f( 3588, 3, -1 ));
	container->addChild(highlight);

	//outline = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/active/outline_locker_room.png" ) ) ) ) );
	//outline->setPosition(ci::Vec3f( 4435, 2873, -25 ));
	//container->addChild( outline );

	mHighlightedImages[MapPositions::MESS_HALL] = container;
	mHighlightContainer->addChild(mHighlightedImages[MapPositions::MESS_HALL]);

	//rotate container
	mHighlightContainer->setRotation(ci::Vec3f(90,0,0));
}

void LocationLocalViewMap::createUnusedImages()
{
	mUnusedImages = new std::vector<ci::Vec2i>;

	mUnusedImages->push_back(ci::Vec2i(0,0));
	mUnusedImages->push_back(ci::Vec2i(0,1));
	mUnusedImages->push_back(ci::Vec2i(0,2));
	mUnusedImages->push_back(ci::Vec2i(0,5));
	mUnusedImages->push_back(ci::Vec2i(0,6));
	mUnusedImages->push_back(ci::Vec2i(0,7));
	mUnusedImages->push_back(ci::Vec2i(1,0));
	mUnusedImages->push_back(ci::Vec2i(1,6));
	mUnusedImages->push_back(ci::Vec2i(1,7));
	mUnusedImages->push_back(ci::Vec2i(2,7));
	mUnusedImages->push_back(ci::Vec2i(3,7));
	mUnusedImages->push_back(ci::Vec2i(4,0));
	mUnusedImages->push_back(ci::Vec2i(4,7));
	mUnusedImages->push_back(ci::Vec2i(5,0));
	mUnusedImages->push_back(ci::Vec2i(5,7));
	mUnusedImages->push_back(ci::Vec2i(6,0));
	mUnusedImages->push_back(ci::Vec2i(6,3));
	mUnusedImages->push_back(ci::Vec2i(6,7));
	mUnusedImages->push_back(ci::Vec2i(7,0));
	mUnusedImages->push_back(ci::Vec2i(7,3));
	mUnusedImages->push_back(ci::Vec2i(7,4));
	mUnusedImages->push_back(ci::Vec2i(7,5));
	mUnusedImages->push_back(ci::Vec2i(7,6));
	mUnusedImages->push_back(ci::Vec2i(7,7));
}

bool LocationLocalViewMap::checkFrameForContent(int x, int y)
{
	bool hasContent = true;
	ci::Vec2i test = ci::Vec2i(x,y);

	for(unsigned int i = 0; i < mUnusedImages->size(); ++i)
	{
		if( test == mUnusedImages->at(i))
		{
			hasContent = false;
			break;
		}
	}

	return hasContent;
}

void LocationLocalViewMap::animateCameraTo(ci::Vec3f camPos, ci::Vec3f lookAt)
{
	mIsAnimating = true;

	SoundController::getInstance().playSound(SoundController::LOC_CAMERA_MOVE);

	timeline().apply(&mCamPos,camPos, 2.25f, EaseInOutCubic());
	timeline().apply(&mLookAt,lookAt, 2.25f, EaseInOutCubic())
		.updateFn(boost::bind(&LocationLocalViewMap::onUpdateRotation, this))
		.finishFn(boost::bind(&LocationLocalViewMap::onAnimateCameraComplete,this));
}

void LocationLocalViewMap::onUpdateRotation()
{
	ci::Vec2f norm = getRotationNormal();
	mRotAngle = atan2(norm.y, norm.x);
}

void LocationLocalViewMap::onAnimateCameraComplete()
{
	ci::Vec2f norm = getRotationNormal();
	mRotAngle = atan2(norm.y, norm.x);
	mRotDistance = (mLookAt.value().xz() - mCamPos.value().xz()).length();
	mIsAnimating = false;
}

void LocationLocalViewMap::animateCameraTo(std::pair<ci::Vec3f,ci::Vec3f> camPair)
{
	this->animateCameraTo(camPair.first, camPair.second);
}

void LocationLocalViewMap::update(float dt)
{
	//update rotation of POIs against camera for a fake look-at
	for( unsigned int i = 0; i < mPOIs->size(); ++i )
		mPOIs->at(i)->getRotation()->value().y = toDegrees(mRotAngle) - 90.f;
}

void LocationLocalViewMap::render()
{	
	//draw actual camera persp
	glPushMatrix();
	{
		//bind buffer
		mFbo1.bindFramebuffer();
		
			gl::enableAlphaBlending();
			gl::clear( ci::ColorA( 0, 0, 0, 0 ) ); 

			//look at cam
			mCam.lookAt( mCamPos, mLookAt, mUp );
			gl::setMatrices(mCam);
			
			//render maps
			const float depth = 150.f;
			const float levels = 5.f;
			float alpha = 0.f;

			gl::translate(0,-depth,0);			
			
			for(int i = 0; i < levels-1; ++i)
			{
				alpha = (i*.1f) + .45f;
				gl::translate(0,depth/levels,0);
				mMapContainer->setAlpha(alpha);
				mMapContainer->render();
			}

			alpha = .95f;
			gl::translate(0,depth/levels,0);
			mMapContainer->setAlpha(alpha);
			mMapContainer->render();

			//new matrix since highlights and text are both raised
			glPushMatrix();
			{
				//render highlights
				mHighlightContainer->render();
			
				//text is additive
				gl::enableAdditiveBlending();
				gl::translate(0,50,0);
		
				//render pois
				for(unsigned int i = 0; i < mPOIs->size(); ++i)
				{
					mPOIs->at(i)->render();
				}
			}
			glPopMatrix();
			
			//for testing
			//gl::drawColorCube(mLookAt, ci::Vec3f(25,25,25));
			
			gl::enableAlphaBlending();
			
		//unbind fbo
		mFbo1.unbindFramebuffer();
	}
	glPopMatrix();
	
	renderShaderPasses();

	//params->draw();
}

void LocationLocalViewMap::renderShaderPasses()
{
	//fbo pingpong for some box-blur
	if(mShader)mShader->bind();
	glPushMatrix();
	{
		mFbo2.bindFramebuffer();

			gl::enableAlphaBlending();
			gl::clear( ci::ColorA( 0, 0, 0, 0 ) ); 
			
			ci::gl::Texture texture = mFbo1.getTexture(0);
			texture.setFlipped(true);

			texture.bind(0);
			mVignetteAlpha->bind(1);
			mVignetteBlur->bind(2);
		
			//draw fbo1 with shader first pass
			if(mShader)
			{
				mShader->uniform("direction",0);
				mShader->uniform("mapTexture",0);
				mShader->uniform("vignetteAlpha",1);
				mShader->uniform("vignetteBlur",2);
			}
			gl::draw( texture, ci::Rectf( 0.f, 0.f, (float)FBO_WIDTH, (float)FBO_HEIGHT ) );
		
			
			mVignetteAlpha->unbind();
			mVignetteBlur->unbind();
			texture.unbind();

		mFbo2.unbindFramebuffer();

		//start second pass of shader and render to screen
		texture = mFbo2.getTexture(0);
		texture.setFlipped(true);

		texture.bind(0);
		mVignetteAlpha->bind(1);
		mVignetteBlur->bind(2);
		
		if(mShader)
		{
			mShader->uniform("direction",1);
			mShader->uniform("mapTexture",0);
			mShader->uniform("vignetteAlpha",1);
			mShader->uniform("vignetteBlur",2);
		}
		
		//move whole thing down 200 so that lookat is centered along bottom 
		gl::translate(0,200,0);
		gl::draw( texture, ci::Rectf( 0.f, 0.f, (float)FBO_WIDTH, (float)FBO_HEIGHT ) );
		
		mVignetteAlpha->unbind();
		mVignetteBlur->unbind();
		texture.unbind();
	}
	glPopMatrix();
	if(mShader)mShader->unbind();
}


void LocationLocalViewMap::handleInput(INPUT_TYPE type)
{
	switch(type)
	{
	case NEXT:
		mMapIter++;
		if(mMapIter == mMapPositions.end()) mMapIter = mMapPositions.begin();

		animateCameraTo(mMapIter->second);
		break;

	case PREVIOUS:
		if(mMapIter == mMapPositions.begin()) mMapIter = mMapPositions.end();
		mMapIter--;

		animateCameraTo(mMapIter->second);
		break;

	case SELECT:
		//createShader();
		break;
            
        default:
            break;
	}
    
}

#ifdef _WIN32
void LocationLocalViewMap::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(!mIsAnimating)
	{
		float xVal = 0.f, yVal = 0.f;
		const float xRange = 2.f, yRange = 20.f, min = 12000.f;

		if( gamepad.sThumbRX > min )
			xVal = mapf(gamepad.sThumbRX, min, MAXSHORT, 0.f, xRange);
		else if( gamepad.sThumbRX < -min )
			xVal = mapf(gamepad.sThumbRX, -min, -MAXSHORT, 0.f, -xRange);

		if( gamepad.sThumbRY > min )
			yVal = mapf(gamepad.sThumbRY, min, MAXSHORT, 0.f, yRange);
		else if( gamepad.sThumbRY < -min )
			yVal = mapf(gamepad.sThumbRY, -min, -MAXSHORT, 0.f, -yRange);
	
		//adjust height
		mCamPos.value().y += yVal;

		if(mCamPos.value().y > 1200) 
			mCamPos.value().y = 1200;
		else if(mCamPos.value().y < 250)
			mCamPos.value().y = 250;

		//adjust rotation 
		setCameraRotation(toRadians(xVal));
	}
}
#endif

ci::Vec2f LocationLocalViewMap::getRotationNormal()
{
	return (mCamPos.value().xz() - mLookAt.value().xz()).normalized();
}

void LocationLocalViewMap::setCameraRotation(float rotation)
{
	mRotAngle += rotation;
	
	mCamPos.value().x = (mRotDistance * cos(mRotAngle)) + mLookAt.value().x;
	mCamPos.value().z = (mRotDistance * sin(mRotAngle)) + mLookAt.value().z;
}

float LocationLocalViewMap::getCameraRotation()
{
	return mRotAngle;
}