#include "LocationOrbitalView.h"

using namespace ci;
using namespace ci::app;

LocationOrbitalView::LocationOrbitalView(Assets* assets) : 
	LocationBaseView(assets),
	mPlanet(NULL),
	mGlyphs( GlyphRef( new BackgroundMainGlyphs(assets) ) ),
	mOverlay( OverlayRef( new LocationOrbitalOverlay(assets) ) ),
	mParticles( SmallParticleSystemRef( new BackgroundSmallParticleSystem(assets, 6500) ) ),
	mLight( ImageViewRef( new ImageView( assets->getTextureByID("loc_orbital_light") ) ) ),
	mNoiseTexture( assets->getTextureByID("loc_planet_noise") ),
	mPlanetShader( assets->getShaderByID("loc_planet") )
{
	//guide first!
	//createGuide("guides/location_orbital.png");

	mGlyphs->setPosition(ci::Vec3f((float)getWindowWidth() - 45.f, (float)getWindowHeight()/2.0f,0));
	mGlyphs->setScale(ci::Vec3f(1.12f,1.12f,0.f));

	mParticles->setPosition(ci::Vec3f(840,1520,0));
	mParticles->setRotation(ci::Vec3f(0,0,-90));
	
	mLight->setCenter(ci::Vec3f(1.f, 0.f, 0.f));
	mLight->setPosition(ci::Vec3f(getWindowWidth(), 0.f, 0.f));
	mLight->setAdditiveBlending(true);

	this->setSize(ci::Vec3f(1920,1080,0));
}

LocationOrbitalView::~LocationOrbitalView(void)
{
	mGlyphs.reset();
	mOverlay.reset();
	mParticles.reset();
	mLight.reset();
	
	mNoiseTexture->reset();
	delete mNoiseTexture;

	mPlanetShader->reset();
	mPlanetShader.reset();

	if(mPlanet) mPlanet.reset();
}

void LocationOrbitalView::prerender()
{
	mOverlay->prerender();
}

void LocationOrbitalView::update(float dt)
{
	//InputView::update(dt);
	
	mPlanet->update(dt);
	mGlyphs->update(dt);
	mParticles->update();
	mOverlay->update(dt);
}

void LocationOrbitalView::render()
{
	mPlanet->render(); 

	if(mPlanetShader)
	{
		glPushMatrix(); 
		{
			glTranslatef(position.value().x, position.value().y, position.value().z); 
			glRotatef(rotation.value().x, 1.0f, 0, 0);
			glRotatef(rotation.value().y, 0, 1.0f, 0);
			glRotatef(rotation.value().z, 0, 0, 1.0f);
			glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
			glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
		
		
			mPlanetShader->bind();
		
			mNoiseTexture->bind(0);
			mPlanet->getTexture()->bind(1);

			mPlanetShader->uniform("noiseTex",0);
			mPlanetShader->uniform("planetTex",1);
			mPlanetShader->uniform("frame", (int)getElapsedFrames() );

			gl::enableAdditiveBlending();

			glBegin( GL_QUADS );
				glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
				glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
				glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
				glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
			glEnd();
			
			gl::enableAlphaBlending();;
		
			mNoiseTexture->unbind(0);
			mPlanet->getTexture()->unbind(1);

			mPlanetShader->unbind();
		}
		glPopMatrix(); 
	}

	InputView::render();
}

void LocationOrbitalView::setPlanet(std::string pngPath)
{
	if(mPlanet && contains(mPlanet))
	{
		this->removeChild(mPlanet);
		mPlanet->getTexture()->reset();
		mPlanet.reset();
		mPlanet = NULL;
	}

	mPlanet = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( ((std::string)"scenes/location/").append(pngPath) ) ) ) ) );
	//this->addChild(mPlanet);
	this->addChild(mLight);
	this->addChild(mParticles);
	this->addChild(mGlyphs);
	this->addChild(mOverlay);
}

void LocationOrbitalView::onTransitionStart()
{
	LocationBaseView::onTransitionStart();
	
}

void LocationOrbitalView::onTransitionSwap(bool isNext)
{
	ci::app::console() << "LocationOrbitalView::onTransitionSwap() isNext: "<< isNext << std::endl;

	if(isNext)
	{

	}
	else
	{
		onTransitionEnd();
	}
}

void LocationOrbitalView::onTransitionEnd()
{

	
	LocationBaseView::onTransitionEnd();
}


void LocationOrbitalView::onAnimateIn()
{
	//mVideoLoop->play();
}

void LocationOrbitalView::onAnimateOut()
{
	//mVideoLoop->stop();
}

void LocationOrbitalView::handleInput(INPUT_TYPE type)
{

}

#ifdef _WIN32
void LocationOrbitalView::handleGamepad(XINPUT_GAMEPAD gamepad)
{

}
#endif