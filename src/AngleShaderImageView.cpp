#include "AngleShaderImageView.h"


AngleShaderImageView::AngleShaderImageView(ci::gl::Texture* tex, ci::gl::GlslProgRef glShader):ShaderImageView(tex, glShader)
{
	angle = 0; 
	speed = 1.0f;
}

void AngleShaderImageView::shaderParams()
{
	float a = alpha->value();
	if(parent != NULL) a *= parent->getAlpha()->value();

	shader->uniform( "alpha", a );
	shader->uniform( "angle", angle );
}

void AngleShaderImageView::update(float dt)
{
	angle += dt * speed; 
	if(angle > M_PI * 2) angle -= M_PI*2;
}

void AngleShaderImageView::setSpeed(float val)
{
	speed = val;
}

AngleShaderImageView::~AngleShaderImageView(void)
{
	shader.reset();
	shader = NULL; 
}
