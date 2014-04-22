#include "cinder/app/AppNative.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"

#include "cinder/Surface.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"

#include "InputController.h"
#include "ViewController.h"
#include "Assets.h"
#include "CXBOXController.h"

#include "SoundController.h"

//using namespace ci;
//using namespace ci::app;
//using namespace std;

class ARGOS_PrototypeApp : public cinder::app::AppNative {
    ci::gl::Texture infoTex;
	
	Assets*				assets;
	InputController*	inputController;
	ViewController*		viewController;
	
	int width, height;
	double lastFrameTime;
	float dt;
    
public:
	void setup();
	void prepareSettings( Settings *settings );
    
	void keyDown( ci::app::KeyEvent event );
	void update();
	void draw();
};

void ARGOS_PrototypeApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1920, 1080 );
	settings->setFrameRate( 60.0f );
	settings->setFullScreen( true );
}

void ARGOS_PrototypeApp::setup()
{
	width = getWindowWidth();
	height = getWindowHeight();
	
	dt = 0;
	lastFrameTime = getElapsedSeconds();
	
	assets = new Assets;
    
	// initialize assets in SoundController but calling getInstance()
	SoundController::getInstance();
    
	inputController		=	new InputController;
	inputController->setup();
    
	viewController = new ViewController( assets, inputController );
}

void ARGOS_PrototypeApp::keyDown( ci::app::KeyEvent event )
{
	if( event.getChar() == ci::app::KeyEvent::KEY_ESCAPE  ){
		
		assets->disposeTextures();
		assets->disposeVideos();
        
		GLuint textures[1];
		if(infoTex) textures[0] = infoTex.getId();
		glDeleteTextures(1, textures);
        
		quit();
	}
    
	inputController->getKeyboardInput()->handleKeyboard(&event);
}

void ARGOS_PrototypeApp::update()
{
	double currentTime = getElapsedSeconds();
	dt = (float)currentTime - (float)lastFrameTime;
	lastFrameTime = currentTime;
    
	inputController->update();
	viewController->update(dt);
}

void ARGOS_PrototypeApp::draw()
{
    ci::gl::clear( ci::Color( 0, 0, 0 ) );
	ci::gl::enableAlphaBlending();
    
	viewController->render(width, height);
    
	// draw debug info
	ci::TextLayout infoText;
	infoText.clear( ci::ColorA( 0.2f, 0.2f, 0.2f, 0.5f ) );
	infoText.setColor( ci::Color::white() );
	infoText.addLine( "FPS: " + ci::toString(getAverageFps()) );
	
	//render input controls to the infotext
	inputController->render(&infoText);
	
	infoText.setBorder( 4, 2 );
	infoTex = ci::gl::Texture( infoText.render( true ) );
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	ci::gl::draw( infoTex, ci::Rectf( infoTex.getBounds() )   );
}

CINDER_APP_NATIVE( ARGOS_PrototypeApp, ci::app::RendererGl )
