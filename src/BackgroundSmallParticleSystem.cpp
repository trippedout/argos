#include "BackgroundSmallParticleSystem.h"

using namespace ci;
using namespace ci::app;

BackgroundSmallParticleSystem::BackgroundSmallParticleSystem(Assets* assets, int numParticles) : 
	View(assets),
	mMaxParticles(numParticles),
	mNumParticles(0),
	mPhase(0.f)
{
	//build shader on creation
	mShader = assets->getShaderByID("small_particles");

	mPositions			=	new ci::Vec4f[mMaxParticles];	

	//advance particles
	for(int i = 0; i < 500; ++i)
	{
		this->update();
	}
}


BackgroundSmallParticleSystem::~BackgroundSmallParticleSystem(void)
{
	mShader.reset();

	delete[] mPositions;

	for( int i = 0; i < mNumParticles; i++ ) 
	{
		mParticles[i].reset();
	}

	mParticles.clear();
}

void BackgroundSmallParticleSystem::update()
{
	int i = 0;

	if( mNumParticles < mMaxParticles )
	{
		for( i = 0; i < 50; ++i)
		{
			ci::Vec4f startingPos = ci::Vec4f(0.f, (float)getWindowHeight(), 0.f, 0.f);
			mParticles.push_back(SmallParticle( new BackgroundSmallParticle(startingPos)));
			mNumParticles++;
		}
	}

	for( i = 0; i < mNumParticles; i++ ) 
	{
		mParticles[i]->update();
		mPositions[i] = mParticles[i]->getPosition();
	}
}

void BackgroundSmallParticleSystem::render()
{
	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x, scale.value().y, scale.value().z);
		
		gl::enableAdditiveBlending();
	
		mShader->bind();
	
		//pass in uniforms
		mShader->uniform("frame", (float)getElapsedFrames());

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