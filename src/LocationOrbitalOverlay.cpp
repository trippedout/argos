#include "LocationOrbitalOverlay.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;

LocationOrbitalOverlay::LocationOrbitalOverlay(Assets* assets) :
	View(assets),
	mSphereVbo( assets->getVBOByID( "loc_sphere" ) ),
	mSphereMesh( assets->getMeshByID( "loc_sphere" ) ),
	mSphereAlpha( ImageViewRef( new ImageView( assets->getTextureByID("loc_planet_alpha") ) ) ),
	mShader( assets->getShaderByID("loc_sphere") ),
	mFbo(NULL),
	sphereRotation(ci::Anim<ci::Vec3f>()),
	sphereOffset(ci::Vec3f(-80,140.f,0.f)),
	sphereScale(.91f),
	drawDistance(650.f),
	//PARTICLE SHIT
	mParticleTex( ImageViewRef( new ImageView( assets->getTextureByID("loc_sphere_particle_tex") ) ) ),
	mParticleShader( assets->getShaderByID("loc_sphere_particle") ),
	mMaxParticleGroups(150),
	mMaxParticles(0),
	mParticleGroups(new std::vector<ParticleGroupRef>(mMaxParticleGroups)),	
	mPositions(NULL),
	mExtraData(NULL),
	mNumParticles(0)
{
	mMaxParticles = mMaxParticleGroups * SphereParticleGroup::mNumPoints;
	mNumParticles = mMaxParticles;
	
	mPositions = new ci::Vec4f[mMaxParticles];
	mExtraData = new ci::Vec4f[mMaxParticles];

	//setup fbo
	Fbo::Format format;
	format.setSamples(16);
	mFbo = new Fbo( 1920, 1080, format );

	//set internal size for prerender
	this->setSize(ci::Vec3f(1920.f, 1080.f, 0.f));

	//setup particle groups
	for(int i = 0; i < mMaxParticleGroups; ++i)
	{
		mParticleGroups->at(i) = ParticleGroupRef( new SphereParticleGroup( getRandomPoints() ) );
	}

}

std::pair<ci::Vec3f*, ci::Vec3f*> LocationOrbitalOverlay::getRandomPoints()
{
	std::vector<ci::Vec3f>* vertices = &mSphereMesh->getVertices();

	unsigned int rand = int(Rand::randFloat() * (vertices->size() - 1));

	ci::Vec3f* vert1 = &vertices->at(rand);
	ci::Vec3f* vert2 = &vertices->at(rand+1);
	
	if( vert1->y > -200 && vert1->y < 400 &&
		vert2->y > -200 && vert2->y < 400)
	{		
		ci::Vec3f distance = *vert1 - *vert2;
		float length = distance.length();

		//check to see if returning zero or reaching too far across sphere
		if( length == 0.f || length > 200.f ) return getRandomPoints();

		return std::make_pair(vert1, vert2);
	}
	else
		return getRandomPoints();
}

LocationOrbitalOverlay::~LocationOrbitalOverlay(void)
{
	mSphereVbo->reset();
	delete mSphereVbo;

	mSphereMesh->clear();
	delete mSphereMesh;

	mSphereAlpha.reset();
	
	mShader->reset();
	mShader.reset();

	mParticleTex.reset();
	
	mParticleShader->reset();
	mParticleShader.reset();

	for(int i = 0; i < mMaxParticleGroups; ++i)
	{
		mParticleGroups->at(i).reset();
	}
	mParticleGroups->clear();
	delete mParticleGroups;

	delete mPositions;
	delete mExtraData;

	delete mFbo;
	delete mSphereVbo;
}

void LocationOrbitalOverlay::update(float dt)
{
	sphereRotation.value().y -= 0.035f;

	ParticleGroupRef particleGroup;


	//update particle groups andcheck if its dead
	for(int i = 0; i < mMaxParticleGroups; ++i)
	{
		//update 
		particleGroup = mParticleGroups->at(i);
		particleGroup->update();

		//check if its dead
		if(particleGroup->getIsDead())
		{
			//erase current
			mParticleGroups->erase(mParticleGroups->begin() + i);
			particleGroup.reset();
			particleGroup = NULL;

			//create new one
			particleGroup = ParticleGroupRef( new SphereParticleGroup( getRandomPoints() ) );
			mParticleGroups->push_back(particleGroup);
		}


		//update points
		for(unsigned int j = 0; j < SphereParticleGroup::mNumPoints; ++j)
		{
			mPositions[(i* SphereParticleGroup::mNumPoints) + j] = particleGroup->getPosition(j);
			mExtraData[(i* SphereParticleGroup::mNumPoints) + j] = ci::Vec4f( particleGroup->getSeed(j), 0.f, 0.f, particleGroup->getAlpha() );
		}

	}
}

void LocationOrbitalOverlay::prerender()
{
	if(alpha->value() <= 0) return; 

	float width = size.value().x;
	float height = size.value().y;
	
//	GLfloat light_positionWire[]	= { width/2.0f - 100.0f, height/2.0f, -1000.f, 1.0f };
//	GLfloat light_wireColor[]		= { 0.51f, 0.98f, 1.0f, .25f };

	glPushMatrix();

	//bind FBO, render children. 

		mFbo->bindFramebuffer();

		float fov = 60.0f; 
		float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
		CameraPersp cam( (int)size.value().x, (int)size.value().y, fov );
		cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, drawDistance );
		ci::Vec3f camPos = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, camZ );
		ci::Vec3f camTarget = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, 0 );
		ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

		cam.lookAt( camPos, camTarget, camUp);
		gl::setMatrices( cam );
		gl::setViewport( mFbo->getBounds() );
		gl::clear(ci::ColorA(0,0,0,0));

		gl::enableAdditiveBlending();

		glDisable(GL_TEXTURE_2D);
		//glEnable( GL_LIGHTING );
		//glEnable( GL_LIGHT0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glLineWidth( 1.05f );
		
		//glLightfv( GL_LIGHT0, GL_POSITION, light_positionWire );
		//glLightfv( GL_LIGHT0, GL_DIFFUSE, light_wireColor );

		glPushMatrix();
			glTranslatef(width/2.0f - sphereOffset.x, height/2.0f - sphereOffset.y, 0); 
			glRotatef(sphereRotation.value().x, 1.0f, 0, 0); 
			glRotatef(sphereRotation.value().y, 0, 1.0f, 0); 
			glRotatef(sphereRotation.value().z, 0, 0, 1.0f); 
			glScalef(sphereScale, sphereScale, sphereScale); 

			//draw wireframe
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
			//glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_wireDiffuse );
			//glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_wireAmbient );
			
			gl::draw( *mSphereVbo );		

			// disable everything, return shit to normal for particles
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glDisable( GL_LIGHTING );
			glDisable( GL_LIGHT0 );
			glEnable(GL_TEXTURE_2D);


			drawParticles();

		glPopMatrix();
	
		

		gl::enableAlphaBlending();

		mFbo->unbindFramebuffer();

	//unbind FBO
	glPopMatrix();
}

void LocationOrbitalOverlay::drawParticles()
{
	glPushMatrix(); 		
		gl::enableAdditiveBlending();
	
		//small particles
		mParticleShader->bind();

		//mParticleTex->getTexture()->bind(0);
		//mParticleShader->uniform("particleTex", 0);
		
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4, GL_FLOAT, 0, mPositions);
		
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, mExtraData);

		mParticleTex->getTexture()->enableAndBind();
		
		glDrawArrays(GL_POINTS, 0, mNumParticles);
		
		mParticleTex->getTexture()->unbind();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		
		mParticleShader->unbind();

	glPopMatrix();
	
}

void LocationOrbitalOverlay::render()
{
	if(alpha->value() <= 0) return; 
	
	glPushMatrix(); 
	{
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
		glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
		
		draw();
	}
	glPopMatrix(); 
}

void LocationOrbitalOverlay::draw()
{

	const ci::gl::Texture fbotex = mFbo->getTexture();
	
	gl::enableAdditiveBlending(); 
	
	mShader->bind();
	
	fbotex.bind(0); 
	mSphereAlpha->getTexture()->bind(1);

	mShader->uniform("mapTexture", 0);
	mShader->uniform("alphaTexture", 1);

	drawQuad();
	
	fbotex.unbind(0);
	mSphereAlpha->getTexture()->unbind(1);

	mShader->unbind();

	gl::enableAlphaBlending(); 
}

void LocationOrbitalOverlay::drawQuad()
{
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
}



// --------------------------------------------------------------------------------------------
// sphere particle group
// --------------------------------------------------------------------------------------------

SphereParticleGroup::SphereParticleGroup(std::pair<ci::Vec3f*, ci::Vec3f*> start_end) :
	mStart(start_end.first), mEnd(start_end.second),
	mParticles( new std::vector<ParticleRef> ),
	mStartingLife(0), mLife(0),
	mLifeAlphaBuffer(150), mAlpha(0.f),
	mIsDead(false),
	mDistance(ci::Vec3f())
{
	mStartingLife = 420 + int(Rand::randFloat() * 500); 
	mLife = mStartingLife;

	//find distance
	mDistance = *mEnd - *mStart;

	//from distance calculate a velocity to move particles along the line
	ci::Vec4f velocity = ci::Vec4f( mDistance.normalized().xyz(), 0.f ) * 0.08f;
	
	for(int i = 0; i < mNumPoints; ++i)
	{
		//get a random point along the line, no more than 65% along the line so it has room to travel
		ci::Vec3f rd = (Rand::randFloat() * (mDistance * .65f)) + *mStart;

		mParticles->push_back( ParticleRef( new SphereParticle( ci::Vec4f(rd.xyz(), 0.f), velocity ) ) );
	}
}

SphereParticleGroup::~SphereParticleGroup(void)
{
	delete mParticles;
}

void SphereParticleGroup::update()
{
	mLife--;

	if( mLife <= 0 ) 
		mIsDead = true;
	else
	{
		//get the fade in period
		float in = mStartingLife - mLifeAlphaBuffer;
		
		//get the fade out period
		float out = mLifeAlphaBuffer;

		if(mLife > in) mAlpha = (mLife - mStartingLife) / (in - mStartingLife);
		else if(mLife < out) mAlpha = mLife / out;
		else mAlpha = 1.f;

		//ci::app::console() << "ParticleGroup::update() life: " << mLife << " alpha: " << mAlpha << " startingLife: " << mStartingLife << " buffer: " << mLifeAlphaBuffer << std::endl;

		for(int i = 0; i < mNumPoints; ++i)
		{
			mParticles->at(i)->update();
		}
	}
}

ci::Vec4f SphereParticleGroup::getPosition(int particle_index)
{
	return mParticles->at(particle_index)->getPosition();
}

float SphereParticleGroup::getSeed(int particle_index)
{
	return mParticles->at(particle_index)->getSeed();
}


// --------------------------------------------------------------------------------------------
// sphere particle
// --------------------------------------------------------------------------------------------


SphereParticle::SphereParticle(ci::Vec4f start, ci::Vec4f velocity) : 
	ParticleBase(start),
	mSeed(0.f)
{
	mVelocity = velocity;
	reset();
}

SphereParticle::~SphereParticle(void)
{

}

void SphereParticle::reset()
{
	mIncrease = Rand::randFloat() * 0.065;
	if(mIncrease < 0.02) mIncrease = 0.02;
	mPosition = mStartingPosition;
	mPosition.w = Rand::randFloat();
	mSeed = Rand::randFloat();
}

void SphereParticle::update()
{
	mPosition += mVelocity;
	mPosition.w += mIncrease;
	mSeed += mIncrease + 0.01;
}