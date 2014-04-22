#include "ImageView.h"

using namespace ci;

ImageView::ImageView() : DisplayObject()
{
	color = ci::Vec3f(1.0f,1.0f,1.0f); 
	additiveBlending = false; 
	mProject3D = false;
	texture = NULL; 
}

ImageView::ImageView(ci::gl::Texture* tex) : DisplayObject()
{
	color = ci::Vec3f(1.0f,1.0f,1.0f); 
	additiveBlending = false; 
	mProject3D = false;
	setTexture(tex);
}

ImageView::ImageView(ci::gl::Texture* tex, ci::Vec3f pos, ci::Vec3f cent) : DisplayObject()
{
	position = pos;
	center = cent; 
	color = ci::Vec3f(1.0f,1.0f,1.0f); 
	additiveBlending = false; 
	mProject3D = false;
	setTexture(tex);
}

ImageView::ImageView(ci::gl::Texture* tex, ci::Rectf rect) : DisplayObject()
{
	color = ci::Vec3f(1.0f,1.0f,1.0f); 
	additiveBlending = false; 
	mProject3D = false;
	setTexture(tex);
	setRenderRect(rect); 
}

ImageView::~ImageView(void)
{
	//if(texture) deleteTexture();
}

/*
TODO - refactor all this shit (size/color/etc) out and into DisplayObject.cpp
*/
void ImageView::render()
{
	if(!texture || alpha->value() <= 0) return; 


	float r,g,b,a; 
	r = color.value().x; 
	g = color.value().y; 
	b = color.value().z;

	//set alpha based on parent
	if(parent == NULL){
		a = alpha->value();
	}else{
		a = alpha->value() * parent->getAlpha()->value();
	}

	if(additiveBlending) gl::enableAdditiveBlending(); 

	if(mProject3D){
		glColor4f(r*0.9f,g*0.9f,b,a*0.1f); 
		glPushMatrix(); 
			glTranslatef(position.value().x, position.value().y, position.value().z + 16.0f); 
			glRotatef(rotation.value().x, 1.0f, 0, 0);
			glRotatef(rotation.value().y, 0, 1.0f, 0);
			glRotatef(rotation.value().z, 0, 0, 1.0f);
			glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
			draw();
		glPopMatrix(); 

		glColor4f(r*0.95f,g*0.95f,b,a*0.2f); 
		glPushMatrix(); 
			glTranslatef(position.value().x, position.value().y, position.value().z + 8.0f); 
			glRotatef(rotation.value().x, 1.0f, 0, 0);
			glRotatef(rotation.value().y, 0, 1.0f, 0);
			glRotatef(rotation.value().z, 0, 0, 1.0f);
			glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
			draw();
		glPopMatrix(); 
	}

	glColor4f(r,g,b,a); 
	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
		draw();
	glPopMatrix(); 

	if(additiveBlending) gl::enableAlphaBlending(); 
}

void ImageView::draw()
{
	texture->enableAndBind(); 
	
	drawQuad();
	
	texture->unbind(); 
	
}

void ImageView::drawQuad()
{
	ci::Rectf bounds = ci::Rectf( texture->getBounds() );
	float w = bounds.x2 - bounds.x1; 
	float h = bounds.y2 - bounds.y1; 

	glBegin( GL_QUADS );
		glTexCoord2f(renderRect.x1/w, renderRect.y1/h);											glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f((renderRect.x1 + renderRect.x2)/w, renderRect.y1/h);						glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f((renderRect.x1 + renderRect.x2)/w, (renderRect.y1 + renderRect.y2)/h);		glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(renderRect.x1/w, (renderRect.y1 + renderRect.y2)/h);						glVertex2f( 0-center.x, 1-center.y);
	glEnd();
}

ci::Rectf* ImageView::getRenderRect()
{
	return &renderRect;
}

void ImageView::setRenderRect(ci::Rectf val)
{
	renderRect = val; 
	size =  ci::Vec3f(renderRect.x2, renderRect.y2, 0);
}

void ImageView::setColor(ci::Vec3f val)
{
	color = val; 
}

ci::Anim<ci::Vec3f>* ImageView::getColor()
{
	return &color; 
}

ci::gl::Texture* ImageView::getTexture()
{
	return texture; 
}

void ImageView::setAdditiveBlending(bool val)
{
	additiveBlending = val; 
}

void ImageView::setTexture(ci::gl::Texture* tex){
	texture = tex; 
	ci::Rectf bounds = ci::Rectf( texture->getBounds() );
	size = ci::Vec3f(bounds.x2 - bounds.x1, bounds.y2 - bounds.y1, 0);
	renderRect = ci::Rectf(0,0,size.value().x, size.value().y); 
}

void ImageView::deleteTexture()
{
	if(texture){
		GLuint ids[1];
		ids[0] = texture->getId(); 
		glDeleteTextures(1, ids); 

		delete texture;
		texture = NULL;
	}
}