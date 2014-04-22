#include "LocationLocalParticleSystem.h"

using namespace ci;
using namespace ci::app;

LocationLocalParticleSystem::LocationLocalParticleSystem(Assets* assets) : 
	View(assets),
	mParticles( new std::vector<Particle> ),
	mBigParticles( new std::vector<BigParticle> ),
	mShader( assets->getShaderByID("small_particles") ),
	mBigShader( assets->getShaderByID("loc_big_particle" ) ),
	mMaxParticles(4500),
	mMaxBigParticles(150),
	mNumParticles(0),
	mNumBigParticles(0)
{
	
	mPositions = ( new ci::Vec4f[mMaxParticles] );
	mBigPositions = ( new ci::Vec4f[mMaxBigParticles] );

	//advance particles
	for(int i = 0; i < 500; ++i)
	{
		this->update();
	}
}


LocationLocalParticleSystem::~LocationLocalParticleSystem(void)
{
	mShader->reset();
	mShader.reset();

	mBigShader->reset();
	mBigShader.reset();

	delete[] mPositions;
	delete[] mBigPositions;

	for( int i = 0; i < mNumParticles; i++ ) 
	{
		mParticles->at(i)->reset();
	}
	mParticles->clear();
	delete mParticles;

	for( int i = 0; i < mNumBigParticles; i++ ) 
	{
		mBigParticles->at(i)->reset();
	}
	mBigParticles->clear();
	delete mBigParticles;
}

void LocationLocalParticleSystem::update()
{
	int i = 0;

	//create small particles
	if( mNumParticles < mMaxParticles )
	{
		for( i = 0; i < 50; ++i)
		{
			ci::Vec4f startingPos = ci::Vec4f(0.f, 0.f, 0.f, 0.f);
			mParticles->push_back(Particle( new LocationSmallParticle(startingPos)));
			mNumParticles++;
		}
	}

	//create big particles
	if( mNumBigParticles < mMaxBigParticles )
	{
		for( i = 0; i < 2; ++i)
		{
			mBigParticles->push_back(BigParticle(new LocationBigParticle()));
			mNumBigParticles++;
		}
	}

	//actually update particles
	for( i = 0; i < mNumParticles; i++ ) 
	{
		mParticles->at(i)->update();
		mPositions[i] = mParticles->at(i)->getPosition();
	}

	for( i = 0; i < mNumBigParticles; i++ ) 
	{
		mBigParticles->at(i)->update();
		mBigPositions[i] = mBigParticles->at(i)->getPosition();
	}
}

void LocationLocalParticleSystem::render()
{
	glPushMatrix(); 		
		gl::enableAdditiveBlending();
	
		//small particles
		mShader->bind();
		mShader->uniform("frame", (float)getElapsedFrames());

		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4, GL_FLOAT, 0, mPositions);

		glDrawArrays(GL_POINTS, 0, mNumParticles);

		glDisableClientState(GL_VERTEX_ARRAY);
		mShader->unbind();


		//big particles
		mBigShader->bind();
	
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4, GL_FLOAT, 0, mBigPositions);

		glDrawArrays(GL_POINTS, 0, mNumBigParticles);

		glDisableClientState(GL_VERTEX_ARRAY);
		mBigShader->unbind();
		
		gl::enableAlphaBlending();
	glPopMatrix();
}




// ---------------------------------------------------------------------------------------------------------
// small particles
// ---------------------------------------------------------------------------------------------------------

LocationSmallParticle::LocationSmallParticle(ci::Vec4f startingPos) : ParticleBase(startingPos)
{
	reset();

	mWidth = getWindowWidth();
	mHalfWidth = mWidth * .5f;
}


LocationSmallParticle::~LocationSmallParticle(void)
{

}

void LocationSmallParticle::reset()
{
	const float r = Rand::randFloat();
	mPosition = mStartingPosition;
	mPosition.x = (float)mHalfWidth + (Rand::randFloat() * 500.f) - 250.0f;
	mPosition.z = 120 + r * 100;
	mPosition.w = Rand::randFloat();

	float xVel = ((mHalfWidth - mPosition.x) * 0.0025f);
	mVelocity = ci::Vec4f(xVel,Rand::randFloat() * .85f,0,0);
}

void LocationSmallParticle::update()
{
	//get new xVel
	float xVel = Rand::randFloat() * .2f - .1f;
	
	mVelocity.x += xVel;
	if( mVelocity.x > 1 ) mVelocity.x = 1;
	if( mVelocity.x < -1 ) mVelocity.x = -1;

	mVelocity.y += Rand::randFloat() * 0.0005f - 0.00025f;
	mPosition += mVelocity;
	
	float distance = (float)mHalfWidth - mPosition.x;
	
	if(distance > 250.f || distance < -250.f)
	{
		if(xVel > .1) mVelocity.x *= -.25;
		mPosition.z -= 2;
	}
			
	//update life
	mPosition.z -= 1;

	//if dead
	if(mPosition.z <= 0) reset();
}

// ---------------------------------------------------------------------------------------------------------
// big particles
// ---------------------------------------------------------------------------------------------------------


LocationBigParticle::LocationBigParticle(void) : 
	ParticleBase(ci::Vec4f())
{
	w = (float)getWindowWidth();
	h = (float)getWindowHeight() * .5f;

	reset();
}


LocationBigParticle::~LocationBigParticle(void)
{

}

void LocationBigParticle::reset()
{
	const float r = Rand::randFloat();

	mPosition = ci::Vec4f(Rand::randFloat() * w, Rand::randFloat() * h - 100, 0.f, 0.f );//Rand::randFloat() * -100 );
	mPosition.w = Rand::randFloat() + 1;
	mPosition.z = 200 + r * 100;

	float xVel = Rand::randFloat() * 0.005f - 0.0025f;
	mVelocity = ci::Vec4f(xVel, 0.01f, 0.f, 0.f);
}

void LocationBigParticle::update()
{
	//get new xVel
	float xVel = Rand::randFloat() * .02f - .01f;
	mVelocity.x += xVel;

	mVelocity.y += 0.0045;
	mPosition += mVelocity;
	
			
	//update life
	mPosition.z -= 1;

	//if dead
	if(mPosition.z <= 0) reset();
}