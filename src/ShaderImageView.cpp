#include "ShaderImageView.h"

using namespace ci;

ShaderImageView::ShaderImageView(ci::gl::Texture* tex, ci::gl::GlslProgRef glShader):ImageView(tex)
{
	setShader(glShader);
}

void ShaderImageView::shaderParams()
{
	//override and put in shader variables here
	//ex shader->uniform( "angle", 0.3f );
}

void ShaderImageView::draw()
{
	texture->enableAndBind(); 
	shader->bind();
	shaderParams();

	if(additiveBlending) gl::enableAdditiveBlending(); 

	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
	
	shader->unbind();
	texture->unbind(); 
	
	if(additiveBlending) gl::enableAlphaBlending(); 
}

void ShaderImageView::setShader(ci::gl::GlslProgRef glShader)
{
	shader = glShader;
}

ci::gl::GlslProgRef ShaderImageView::getShader()
{
	return shader;
} 

ShaderImageView::~ShaderImageView(void)
{
	shader.reset();
	shader = NULL; 
}
