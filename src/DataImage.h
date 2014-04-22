#pragma once
#include "DisplayObjectContainer.h"
#include "AngleShaderImageView.h"
#include "Assets.h"
#include "DataNode.h"
#include "ImageView.h"

class DataImage :
	public DisplayObjectContainer
{
protected: 
	boost::shared_ptr<AngleShaderImageView> dust; 
	boost::shared_ptr<AngleShaderImageView> image; 
	ImageViewRef bg; 

public:

	//DataType type; 
	DataImage(ci::gl::Texture* tex, Assets* assets);
	DataImage(ci::gl::Texture* tex, ci::gl::GlslProgRef imageShader, Assets* assets);

	~DataImage(void);

	void changeImage(ci::gl::Texture* tex); 
	void setImageAdditiveBlending(bool val); 
	void showBG(bool val);

};
