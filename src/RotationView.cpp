#include "RotationView.h"

using namespace ci;
using namespace ci::app;

RotationView::RotationView():DisplayObject()
{
	frameRate = 45.0f;
	frameUpdate = 0; 

	duration = 8.0f;
	animOutComplete = false; 
	hidden = false; 
	video = NULL;
}

RotationView::RotationView(std::string path, ci::gl::GlslProgRef glShader):DisplayObject()
{
	//TODO: this is soooo hacks. 
	assetPath = path + "_Loop.mp4"; 

	FBOReady = false; 
	videoReady = false;
	threadLoading = false; 
	deleteAfterLoad = false; 

	frameRate = 45.0f;
	frameUpdate = 0; 

	duration = 8.0f;
	angle = 0; 

	shader = glShader;
	
	delay = 0; 
	time = 0; 
	speed = 1.0f;
	
	setAlpha(0);

	animOutComplete = false; 
	hidden = false; 
	
	video = NULL;
}

void RotationView::onAnimateIn()
{
	ci::app::console() << "RotationView::onAnimateIn()" << std::endl;

	hidden = false; 

	if(video && FBOReady){
		timeline().apply(getAlpha(), 1.0f, 0.25f, EaseOutCubic());
	}else{
		delay = 0.5f; 
	}
}

void RotationView::onAnimateOut()
{
	
}

void RotationView::threadLoaded()
{
	ci::ThreadSetup threadSetup;

    threadLoading = false; 
 	videoReady = true; 
	//ci::app::console()<< "RotationView::threadLoaded / asset: " << assetPath << std::endl;
	
	//video = qtime::MovieGlRef( new qtime::MovieGl( loadAsset(assetPath) ) );
	video = qtime::MovieSurfaceRef( new qtime::MovieSurface( loadAsset(assetPath) ) );
	video->play();
	video->stop();

	//video->getDuration(); 

	delay = 0; 
	time = 0; 

	setAlpha(0);

	if(deleteAfterLoad){
		disposeTempAssets();
		deleteAfterLoad = false; 
	}else{
		if(!hidden)timeline().apply(getAlpha(), 1.0f, 0.5f, EaseOutCubic()).delay(0.5f);
	}
}

void RotationView::hide()
{
	hidden = true; 
	timeline().apply(getAlpha(), 0.0f, 0.25f, EaseOutCubic());
}

void RotationView::show()
{
	hidden = false; 
	timeline().apply(getAlpha(), 1.0f, 0.25f, EaseOutCubic());
}

void RotationView::disposeTempAssets()
{
	if(video){
		//ci::app::console() << "RotationView::disposeTempAssets" <<std::endl;
		video->stop(); 
		video.reset();
		video = NULL;

		videoReady = false;
	}else if (threadLoading){
		deleteAfterLoad = true; 
	}

	timeline().apply(getAlpha(), 0.0f, 0.5f, EaseOutCubic());
}

void RotationView::render()
{
	if(alpha->value() <= 0) return; 

	glPushMatrix(); 

		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
		glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
		draw();

	glPopMatrix(); 
}

void RotationView::prerender()
{
	if( !video || !video->checkPlayable() || alpha->value() == 0 )return; 

	//ci::gl::Texture tex = video->getTexture();
	ci::gl::Texture tex = video->getSurface();

	if(!FBOReady){
		ci::Rectf bounds = ci::Rectf( tex.getBounds() );
		size = ci::Vec3f(bounds.x2 - bounds.x1, bounds.y2 - bounds.y1, 0);

		FBOReady = true; 
		gl::Fbo::Format format;
		fbo = gl::Fbo( (int)bounds.x2 - (int)bounds.x1, (int)bounds.y2 - (int)bounds.y1, format );
	}

	glPushMatrix();
	
	//bind FBO, render children.
	gl::SaveFramebufferBinding bindingSaver;
	fbo.bindFramebuffer();

	float fov = 60.0f; 
	float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925);
	
	CameraPersp cam( (int)size.value().x, (int)size.value().y, fov );
	cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, 10000.0f );
	ci::Vec3f camPos = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, camZ );
	ci::Vec3f camTarget = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, 0 );
	ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

	cam.lookAt( camPos, camTarget, camUp);
	gl::setMatrices( cam );
	gl::setViewport( fbo.getBounds() );
	gl::enableAlphaBlending();
	gl::clear(ci::ColorA(0,0,0,0));

	if(tex){
		ci::Rectf rect = ci::Rectf( tex.getBounds() );
		gl::draw(tex, rect); 
	}
	fbo.unbindFramebuffer();
	//unbind FBO
	glPopMatrix();
}

void RotationView::draw()
{
	if( !FBOReady || !video || !video->checkPlayable() || alpha->value() == 0 ) return; 

	const ci::gl::Texture tex = fbo.getTexture();
	
	gl::enableAdditiveBlending(); 

	tex.enableAndBind(); 
	
	shader->bind();
	shader->uniform( "angle", angle );
	shader->uniform( "speed", speed );
	shader->uniform( "alpha", alpha->value() );

	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
	
	shader->unbind(); 
	
	tex.unbind(); 

	gl::enableAlphaBlending(); 
}

void RotationView::update(float dt)
{
	if(delay > 0) delay-=dt; 

	if(delay <=0 && !videoReady && !threadLoading && !video){
		threadLoading = true; 
		loaderThread = new std::thread( std::bind(&RotationView::threadLoaded, this));
	}

	if(!video || !video->checkPlayable() || alpha->value() == 0) return; 

	angle += dt * 5.0f; 
	if(angle > (float)M_PI * 2.0f) angle -= (float)M_PI*2.0f;

	time += dt * speed; 
	if(time < 0) time += duration; 
	else if (time > duration)  time -= duration; 

	//only update at frameRate (default 30) FPS. 
	frameUpdate += dt; 
	if(frameUpdate > 1.0f/frameRate){
		frameUpdate = 0;
		video->seekToTime(time);
	}
}

void RotationView::setSpeed(float val)
{
	speed = val;
}

RotationView::~RotationView(void)
{

}
