#pragma once
#include "View.h"
#include "Assets.h"

#include "TextViewMultiline.h"
#include "DataViewTitle.h"
#include "DataNode.h"

#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

class DataInnerViewSimple :
	public View
{
protected:
	ImageViewRef icon; 
	boost::shared_ptr<DataViewTitle> title; 
	boost::shared_ptr<TextViewMultiline> body;

	ImageViewRef mBarTop, mBarBottom;

    ci::gl::Fbo	fbo;
	ci::gl::GlslProgRef shader;
	ci::Anim<float>	blur;

public:
	DataInnerViewSimple(boost::shared_ptr<DataNode> data,  Assets* assets);
	~DataInnerViewSimple(void);

	virtual void show(bool snap); 
	virtual void hide(bool snap); 

	virtual void prerender();
	virtual void render(); 
	virtual void draw();

	ci::Anim<float>* getBlur(){return &blur;}; 
};

