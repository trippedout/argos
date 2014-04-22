#include "FloorView.h"

using namespace ci;
using namespace ci::app;

FloorView::FloorView( Assets* assets ) : 
	DisplayObject(),
	angle(0),
	meshRotation(ci::Vec3f()),
	meshPosition(ci::Vec3f())
{
	vbo = assets->getVBOByID( "handbook_floor" );
	shader = assets->getShaderByID( "handbook_sphere_blur" );

	size = ci::Vec3f(1920.0f, 1080.0f, 0);

	gl::Fbo::Format format;
	format.setSamples(4);
	fbo = gl::Fbo( size.value().x, size.value().y, format );
}

void FloorView::prerender()
{
	if(alpha->value() <= 0) return; 

	int width = size.value().x;
	int height = size.value().y;

	glPushMatrix();

	//bind FBO, render children. 

		gl::SaveFramebufferBinding bindingSaver;
		fbo.bindFramebuffer();

		float fov = 60.0f; 
		float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
		CameraPersp cam( size.value().x, size.value().y, fov );
		cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, 10000.0f );
		ci::Vec3f camPos = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, camZ );
		ci::Vec3f camTarget = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, 0 );
		ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

		cam.lookAt( camPos, camTarget, camUp);
		gl::setMatrices( cam );
		gl::setViewport( fbo.getBounds() );
		gl::clear(ci::ColorA(0,0,0,0));

		gl::enableAdditiveBlending();

		glDisable(GL_TEXTURE_2D);
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLineWidth( 3.0f );

		GLfloat light_positionWire[]	= { width/2.0f - 100.0f, height/2.0f, -1000.0f, 1.0f };
		GLfloat light_wireColor[]		= { 0.51f, 0.98f, 1.0f, 1.0f };

		GLfloat mat_wireDiffuse[]		= { 0.7f, 0.8f, 0.9f, 0.1f };
		GLfloat mat_wireAmbient[]		= { 0.88f * 0.81f, 0.75f * 0.5f, 1.0f * 0.5f, 1.0f };


		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glLightfv( GL_LIGHT0, GL_POSITION, light_positionWire );

		glPushMatrix();
			glTranslatef(width/2.0f + meshPosition.x, height/2.0f + meshPosition.y, meshPosition.z); 
			glRotatef(meshRotation.x, 1.0f, 0, 0); 
			glRotatef(meshRotation.y, 0, 1.0f, 0); 
			glRotatef(meshRotation.z, 0, 0, 1.0f); 
			glScalef(1.f, -1.f, 1.f); 

			//draw wireframe
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
			glLightfv( GL_LIGHT0, GL_DIFFUSE, light_wireColor );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_wireDiffuse );
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_wireAmbient );
			gl::draw( *vbo );

		glPopMatrix();
	
		// disable everything, return shit to normal
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDisable( GL_LIGHTING );
		glDisable( GL_LIGHT0 );
		glEnable(GL_TEXTURE_2D);
		gl::enableAlphaBlending();

		fbo.unbindFramebuffer();
	//unbind FBO
	glPopMatrix();
}

void FloorView::render()
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

void FloorView::draw()
{

	const ci::gl::Texture fbotex = fbo.getTexture();
	
	gl::enableAdditiveBlending(); 

	fbotex.enableAndBind(); 

	//draw fbo with shader
	shader->bind();
	shader->uniform( "alpha", alpha->value() );
	drawQuad();
	shader->unbind(); 
	fbotex.unbind(); 

	gl::enableAlphaBlending(); 
}

void FloorView::drawQuad()
{
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
}



void FloorView::update(float dt)
{

	angle += dt * 3; 
	if(angle > M_PI * 2) angle -= M_PI*2;

}

FloorView::~FloorView(void)
{

}
