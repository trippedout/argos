#include "BackgroundMainParticles.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;

BackgroundMainParticles::BackgroundMainParticles(Assets* assets) : View(assets)
{	
	//build shader on creation
	mShader = assets->getShaderByID("main_particles");
	
	//load texture
	mParticleTexture = assets->getTextureByID("main_bg_particle_tex");
	
	//set nums
	mMaxParticles	=	1250;
	mNumParticles	=	0;
	mPhase			=	0.0f;
	mPositions		=	new ci::Vec4f[mMaxParticles];
	mPerlin			=	Perlin();

	mTranslate		=	ci::Vec3f(960,0,0);
	mRotate			=	ci::Vec3f(0,-55,-6);

	//advance particles
	for(int i = 0; i < 500; ++i)
	{
		this->update(0.f);
	}
}

BackgroundMainParticles::~BackgroundMainParticles(void)
{
	mShader->reset();

	delete mParticleTexture;
	delete[] mPositions;

	for( int i = 0; i < mNumParticles; i++ ) 
	{
		mParticles[i].reset();
	}

	mParticles.clear();
}

void BackgroundMainParticles::update(float dt)
{
	int i = 0;

	//create particles
	if( mNumParticles < mMaxParticles )
	{
		float y, ry; 

		for( i = 0; i < 5; i++ )
		{	
			ry = (float)rand()/(float)RAND_MAX;
			y = getWindowHeight() / 2 + ry * 50 - 25;

			ci::Vec4f startingPos = ci::Vec4f(2100,y,300);
			mParticles.push_back(DOFParticle( new BackgroundDOFParticle(startingPos)) );
			mNumParticles++;
		}
	}

	for( i = 0; i < mNumParticles; i++ ) 
	{
		mParticles[i]->update();
		mPositions[i] = mParticles[i]->getPosition();
		i++;
	}
}

void BackgroundMainParticles::render()
{	
	gl::enableAdditiveBlending();
	mShader->bind();

	//pass in uniforms
	//mShader.uniform("frame", (int)getElapsedFrames());
	
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, 0, mPositions);

	mParticleTexture->enableAndBind();

	glDrawArrays(GL_POINTS, 0, mNumParticles);

	mParticleTexture->unbind();

	glDisableClientState(GL_VERTEX_ARRAY);
	mShader->unbind();
	gl::enableAlphaBlending();

	//mParams->draw();
}
