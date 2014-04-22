#include "VideoSnapshot.h"

using namespace ci;
using namespace ci::app;

VideoSnapshot::VideoSnapshot(VideoViewRef vid, Assets* assets):DisplayObject()
{
	video = vid; 
	blur = ci::Anim<float>(0.0f); 

	shader = assets->getShaderByID( "video_blur" );
	size = ci::Vec3f(1024, 512, 0);
	gl::Fbo::Format format;
	format.setSamples(16);
	fbo = gl::Fbo( 1024, 512, format );
}

VideoSnapshot::~VideoSnapshot(void)
{
	shader.reset(); 
	shader = NULL; 
}

void VideoSnapshot::render()
{	
	//video->render(); 

	if(alpha->value() <= 0) return; 

	glPushMatrix(); 
		glTranslatef( -32.0f*scale.value().x, 0, 0 ); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);

		draw();
	glPopMatrix(); 

}

void VideoSnapshot::draw()
{
	const ci::gl::Texture fbotex = fbo.getTexture();

	fbotex.enableAndBind(); 

	float a = alpha->value();
	if(parent != NULL){
		//a*= parent->getAlpha()->value();
	}

	if(a <= 0) return; 

	gl::enableAdditiveBlending();

	//draw fbo with shader
	shader->bind();
	shader->uniform( "alpha", a );
	shader->uniform( "blur", blur.value() );
	
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
	
	shader->unbind(); 
	fbotex.unbind(); 

	gl::enableAlphaBlending();
}

void VideoSnapshot::updateSnapshot()
{
	//ci::app::console()<<"VideoSnapshot::updateSnapshot() "<<std::endl;

//draw video
	glPushMatrix();
	//bind FBO, render children. 
		gl::SaveFramebufferBinding bindingSaver;
		fbo.bindFramebuffer();

		float fov = 60.0f; 
		float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * 1;
	
		CameraPersp cam( size.value().x, size.value().y, fov );
		cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, 10000.0f );
	
		ci::Vec3f camPos = ci::Vec3f( 0, 0, camZ );
		ci::Vec3f camTarget = ci::Vec3f( 0, 0, 0 );
		ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

		cam.lookAt( camPos, camTarget, camUp);
		gl::setMatrices( cam );
		gl::setViewport( fbo.getBounds() );

		gl::enableAlphaBlending();
		//gl::enableAdditiveBlending();
		
		gl::clear(ci::ColorA(0,0,0));
		glColor4f(1.0f,1.0f,1.0f,1.0f); 

		glPushMatrix();
			glTranslatef(-size.value().x/2.0f + 32.0f, -size.value().y/2.0f, 0);
			//video->render();

			const ci::gl::Texture tex = video->getVideo()->getTexture();
			if(tex){
				ci::Rectf rect = ci::Rectf( tex.getBounds() );
				gl::draw(tex, rect); 
			}else{
				ci::app::console()<<"\t VIDEO TEXTURE NOT AVAILABLE"<<std::endl;
			}
		glPopMatrix();
		
		fbo.unbindFramebuffer();
	//unbind FBO
	glPopMatrix();
}