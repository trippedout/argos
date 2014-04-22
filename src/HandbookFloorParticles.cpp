#include "HandbookFloorParticles.h"

using namespace ci;
using namespace ci::app;

HandbookFloorParticles::HandbookFloorParticles(Assets* assets, std::vector<ci::Anim<ci::Vec3f>>* floorVerts, int centerIndex) : View(assets),
	mFloorVerts(floorVerts),
	//vert
	mVertShader(assets->getShaderByID("hb_floorVert")),
	mFloatShader(assets->getShaderByID("hb_floatParticle")),
	mMaxVertParticles(450), mNumVertParticles(0), mVertPositions(NULL), mVertParticles(NULL),
	//float
	mMaxFloatParticles(250), mNumFloatParticles(0), mFloatPositions(NULL), mFloatParticles(NULL)
{	
	mCenterParticle = &floorVerts->at(centerIndex).value();

	mVertPositions = new ci::Vec4f[mMaxVertParticles];
	mVertParticles = new std::vector<VertParticle>;

	mFloatPositions = new ci::Vec4f[mMaxFloatParticles];
	mFloatExtras	= new ci::Vec4f[mMaxFloatParticles];
	mFloatParticles = new std::vector<FloatParticle>;

	mVertTexture = assets->getTextureByID("hb_floor_particle");
	
	mNumFloorVerts = floorVerts->size();

	//create vert particles
	for(int i = 0; i < mMaxVertParticles; ++i)
	{
		mVertParticles->push_back( VertParticle( new FloorVerticeParticle( &floorVerts->at( int(Rand::randFloat() * mNumFloorVerts) ).value() ) ) );
		mNumVertParticles++;
	}
}


HandbookFloorParticles::~HandbookFloorParticles(void)
{
	delete mCenterParticle;
	delete mVertPositions;

	for(int i = 0; i < mMaxVertParticles; ++i)
	{
		mVertParticles->at(i).reset();
	}
	mVertParticles->clear();
	delete mVertParticles;
	
	delete mFloatPositions;
	delete mFloatExtras;

	for( int i = 0; i < mFloatParticles->size(); i++ ) 
	{
		mFloatParticles->at(i).reset();
	}
	mFloatParticles->clear();
	delete mFloatParticles;

	mVertTexture->reset();
	delete mVertTexture;

	//shaders
	mVertShader->reset();
	mVertShader.reset();

	mFloatShader->reset();
	mFloatShader.reset();
}

void HandbookFloorParticles::update()
{
	int i;

	//create floor particles
	if( (getElapsedFrames() % 13) == 0 )
	{
		if(mNumFloatParticles < mMaxFloatParticles)
		{
			//create particle
			unsigned int i = mFloatParticles->size();

			mFloatParticles->push_back( FloatParticle( new FloorFloatingParticle( getRandomPoint(), i+1 ) ) );
		}
	}



	//actually update particles
	for( i = 0; i < mNumVertParticles; i++ ) 
	{
		mVertParticles->at(i)->update();
		mVertPositions[i] = mVertParticles->at(i)->getPosition();
	}

	FloatParticle particle;

	for( i = 0; i < mFloatParticles->size(); i++ ) 
	{
		particle = mFloatParticles->at(i);
		particle->update();

		if(particle->getLife() <= 0.f)
		{
			mFloatParticles->erase( mFloatParticles->begin() + i );
			particle.reset();
			particle = NULL;
		}
		else
		{
			mFloatExtras[i] = ci::Vec4f( particle->getLife(), 0.f, 0.f, 0.f );
			mFloatPositions[i] = particle->getPosition();
		}
	}
}

ci::Vec3f HandbookFloorParticles::getRandomPoint()
{
	ci::Vec3f random = mFloorVerts->at( int(Rand::randFloat() * mNumFloorVerts) ).value();

	ci::Vec3f distance = *mCenterParticle - random;
	float length = distance.length();

	if( length < 1200.f )
		return random;
	else
		return getRandomPoint();
}

void HandbookFloorParticles::render()
{
	if(mVertShader)
	{
		glPushMatrix(); 		
			gl::enableAdditiveBlending();
	
			glColor4f(1.f, 1.f, 1.f, 1.f);
			
			mVertTexture->bind(0);

			//small particles
			mVertShader->bind();
			mVertShader->uniform("tex", 0);

			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(4, GL_FLOAT, 0, mVertPositions);

			glDrawArrays(GL_POINTS, 0, mNumVertParticles);

			glDisableClientState(GL_VERTEX_ARRAY);

			mVertShader->unbind();


			//big particles
			mFloatShader->bind();
			mFloatShader->uniform("tex", 0);

			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(4, GL_FLOAT, 0, mFloatPositions);

			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, mFloatExtras);

			glDrawArrays(GL_POINTS, 0, mFloatParticles->size());

			glDisableClientState(GL_VERTEX_ARRAY);
			
			mFloatShader->unbind();

			mVertTexture->unbind(0);
		
			gl::enableAlphaBlending();
		glPopMatrix();
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vertice
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FloorVerticeParticle::FloorVerticeParticle(ci::Vec3f* startingPos) :
	mStartingPosition(startingPos), mPosition(*startingPos), mVelocity(ci::Vec4f()), mLife(0), mIncrease(0.f)
{
	reset();
}

FloorVerticeParticle::~FloorVerticeParticle(void)
{

}

void FloorVerticeParticle::reset()
{
	mPosition.w = Rand::randFloat();
	
	mIncrease = Rand::randFloat() * 0.038f;
	if(mIncrease < 0.02) mIncrease = 0.02f;
}

void FloorVerticeParticle::update()
{
	mPosition.x = mStartingPosition->x;
	mPosition.y = mStartingPosition->y;
	mPosition.z = mStartingPosition->z;
	mPosition.w += mIncrease;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// floating
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FloorFloatingParticle::FloorFloatingParticle(ci::Vec4f startingPos, int index) :
	mStartingPosition(startingPos), mPosition(startingPos), mVelocity(ci::Vec4f()), mStartingLife(0.f),  mLife(0.f), mIncrease(0.f), mIndex(index)
{
	reset();
}

FloorFloatingParticle::~FloorFloatingParticle(void)
{

}

void FloorFloatingParticle::reset()
{
	mStartingLife = 200.f + int(Rand::randFloat() * 100);
	mLife = mStartingLife;

	mPosition.w = Rand::randFloat();
	mVelocity = ci::Vec4f(0.f, 0.f, 0.5f, 0.f);
	
	mIncrease = Rand::randFloat() * 0.035f;
	if(mIncrease < 0.015) mIncrease = 0.015f;

	mVertIncrease = Rand::randFloat() * 0.0055f;
	if(mVertIncrease < 0.0044f) mVertIncrease = 0.0044f;
}

void FloorFloatingParticle::update()
{	
	mLife--;

	mVelocity.z += mVertIncrease;

	//if(mLife < 100)

	mPosition += mVelocity;
	mPosition.w += mIncrease;
}