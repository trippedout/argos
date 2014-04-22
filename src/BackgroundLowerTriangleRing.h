#pragma once

#include "View.h"

#include "cinder/ObjLoader.h"
#include "cinder/gl/Vbo.h"

class BackgroundLowerTriangleRing : public View
{
protected:
	ci::TriMesh			mMesh;
	ci::gl::VboMesh			mVbo;
	ci::gl::Texture			mTexture;
public:
	BackgroundLowerTriangleRing(Assets* assets);
	~BackgroundLowerTriangleRing(void);

	virtual void render();
	virtual void draw();
};

