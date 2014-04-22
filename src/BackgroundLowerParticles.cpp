#include "BackgroundLowerParticles.h"

using namespace ci;
using namespace ci::app;

BackgroundLowerParticles::BackgroundLowerParticles(Assets* assets) :
	View(assets),
	mParticles( new std::vector<ParticleRef> ),
	mShader( assets->getShaderByID("loc_big_particle" ) ),
	mMaxParticles(350),
	mNumParticles(0)
{
	mPositions = new ci::Vec4f[mMaxParticles];
}


BackgroundLowerParticles::~BackgroundLowerParticles(void)
{
	mShader->reset();
	mShader.reset();

	delete[] mPositions;
	
	for( int i = 0; i < mNumParticles; i++ ) 
	{
		mParticles->at(i)->reset();
	}
	mParticles->clear();
	delete mParticles;
}

void BackgroundLowerParticles::update()
{
	int i = 0;

	//create small particles
	if( mNumParticles < mMaxParticles )
	{
		for( i = 0; i < 2; ++i)
		{
			mParticles->push_back(ParticleRef(new BackgroundLowerParticle()));
			mNumParticles++;
		}
	}

	//actually update particles
	for( i = 0; i < mNumParticles; i++ ) 
	{
		mParticles->at(i)->update();
		mPositions[i] = mParticles->at(i)->getPosition();
	}
}

void BackgroundLowerParticles::render()
{
	glPushMatrix(); 		
		gl::enableAdditiveBlending();
	
		//big particles
		mShader->bind();
	
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4, GL_FLOAT, 0, mPositions);

		glDrawArrays(GL_POINTS, 0, mNumParticles);

		glDisableClientState(GL_VERTEX_ARRAY);
		
		mShader->unbind();
		
		gl::enableAlphaBlending();
	glPopMatrix();
}

// ---------------------------------------------------------------------------------------------------------
// big particles
// ---------------------------------------------------------------------------------------------------------

BackgroundLowerParticle::BackgroundLowerParticle(void) : 
	ParticleBase(ci::Vec4f())
{
	w = (float)getWindowWidth();
	h = (float)getWindowHeight();

	reset();
}


BackgroundLowerParticle::~BackgroundLowerParticle(void)
{

}

void BackgroundLowerParticle::reset()
{
	const float r = Rand::randFloat();

	mPosition = ci::Vec4f(Rand::randFloat() * w, Rand::randFloat() * h - 100, 0.f, 0.f );//Rand::randFloat() * -100 );
	mPosition.w = Rand::randFloat() + 1;
	mPosition.z = 200 + r * 100;

	float xVel = Rand::randFloat() * 0.005f - 0.0025f;
	mVelocity = ci::Vec4f(xVel, -0.01f, 0.f, 0.f);
}

void BackgroundLowerParticle::update()
{
	//get new xVel
	float xVel = Rand::randFloat() * .02f - .01f;
	mVelocity.x += xVel;

	mVelocity.y -= 0.0045;
	mPosition += mVelocity;	
			
	//update life
	mPosition.z -= 1;

	//if dead
	if(mPosition.z <= 0) reset();
}