#pragma once
#include "ImageView.h"
#include "cinder/gl/GlslProg.h"

class ShaderImageView :
	public ImageView
{
public:
	ShaderImageView(ci::gl::Texture* tex, ci::gl::GlslProgRef glShader);
	virtual ~ShaderImageView(void);

	virtual void setShader(ci::gl::GlslProgRef glShader); 
	virtual ci::gl::GlslProgRef getShader();
	virtual void draw(); 

protected: 
	ci::gl::GlslProgRef shader;
	virtual void shaderParams(); 
};

