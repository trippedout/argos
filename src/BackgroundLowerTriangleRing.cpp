#include "BackgroundLowerTriangleRing.h"

using namespace ci;
using namespace ci::app;

BackgroundLowerTriangleRing::BackgroundLowerTriangleRing(Assets* assets) : View(assets)
{
	ObjLoader loader( loadAsset("obj/bg_triangle_strip_circle.obj") );
	loader.load( &mMesh );
	mVbo = ci::gl::VboMesh( mMesh );

	ci::app::console() << "BackgroundLowerTriangleRing created! vertices: " << mVbo.getNumVertices() << std::endl;
}

BackgroundLowerTriangleRing::~BackgroundLowerTriangleRing(void)
{

}

void BackgroundLowerTriangleRing::render()
{
	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);

		glColor4f(1.f, 1.f, 1.f, alpha->value());
		draw();
		
	glPopMatrix(); 
}

void BackgroundLowerTriangleRing::draw()
{
	glBegin( GL_TRIANGLES );
	/*texture->enableAndBind(); 
	if(additiveBlending) gl::enableAdditiveBlending(); 

	ci::Rectf bounds = ci::Rectf( texture->getBounds() );
	float w = bounds.x2 - bounds.x1; 
	float h = bounds.y2 - bounds.y1; 

	glBegin( GL_QUADS );
		glTexCoord2f(renderRect.x1/w, renderRect.y1/h);											glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f((renderRect.x1 + renderRect.x2)/w, renderRect.y1/h);						glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f((renderRect.x1 + renderRect.x2)/w, (renderRect.y1 + renderRect.y2)/h);		glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(renderRect.x1/w, (renderRect.y1 + renderRect.y2)/h);						glVertex2f( 0-center.x, 1-center.y);
	glEnd();
	
	texture->unbind(); 
	
	if(additiveBlending) gl::enableAlphaBlending();*/ 
}