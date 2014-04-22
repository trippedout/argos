#pragma once
#include "ShaderImageView.h"

// this class is a simple extension of shaderimageview 
// it passes an 'angle' of 0-2PI to the shader 
// really handy 

class AngleShaderImageView :
	public ShaderImageView
{
public:
	AngleShaderImageView(ci::gl::Texture* tex, ci::gl::GlslProgRef glShader);
	~AngleShaderImageView(void);

	virtual void shaderParams();
	virtual void update(float dt); 
	virtual void setSpeed(float val); 

protected: 
	float angle; 
	float speed;
};

