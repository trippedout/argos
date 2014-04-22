#include "SphereConstellation.h"

using namespace ci;
using namespace ci::app;

SphereConstellation::SphereConstellation(ci::gl::VboMesh* mesh, ci::TriMesh* outline, ci::Vec3f rotation) 
	: mMesh(mesh), mOutline(outline), mRotation(rotation),
	mInnerAlpha(0.f), mSolidALpha(0.f),
	mLightBeam(NULL)
{
	std::vector<ci::Vec3f*>* points = new std::vector<ci::Vec3f*>;

	for(unsigned int i = 0; i < mOutline->getNumVertices() / 2; ++i)
	{
		points->push_back( &mOutline->getVertices().at(i) );
	}

	mLightBeam = LightBeamRef( new LightBeam(points, ci::Vec4f(1.f,1.f,1.f,1.f), 3.f) );
}


SphereConstellation::~SphereConstellation(void)
{
	delete mMesh;
	delete mOutline;
}

void SphereConstellation::animateIn(ci::Anim<float>* sphereAlpha)
{
	mSphereAlpha = sphereAlpha;

	mLightBeam->animateIn(1.f, 1.35f);
	
	float t = 2.0f;

	timeline().apply( &mSolidALpha, 1.0f, 0.3f).delay(t*0.75f + 0.4f);
	timeline().appendTo( &mSolidALpha, 0.9f, 0.1f);
	timeline().appendTo( &mSolidALpha, 1.0f, 0.1f);
	timeline().appendTo( &mSolidALpha, 0.7f, 0.3f);

	timeline().apply( &mInnerAlpha, 0.75f, 0.8f).delay(t*0.75f + 0.1f);
}

void SphereConstellation::animateOut()
{
	mLightBeam->animateOut(.5f);

	timeline().apply( &mSolidALpha, 0.0f, 0.1f);
	timeline().apply( &mInnerAlpha, 0.0f, 0.2f);
}

void SphereConstellation::update(float dt)
{
	mLightBeam->update(dt);
	mLightBeam->setAlpha(mSphereAlpha->value());
}

void SphereConstellation::render()
{
	////inner mesh
	glLineWidth( 1.0f );
	if(mInnerAlpha.value() > 0){
		glColor4f(0.2f* mInnerAlpha.value(), 0.9f * mInnerAlpha.value(), 1.0f * mInnerAlpha.value(), 0.5f * mInnerAlpha.value() * mSphereAlpha->value());  
		gl::draw( *mMesh );
	}

	//outline
	mLightBeam->render();

	//solid 
	if(mSolidALpha.value() > 0){
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glColor4f(0.25f * mSolidALpha.value(), 0.7f * mSolidALpha.value(), 1.0f * mSolidALpha.value(), 0.5f * mSolidALpha.value() * mSphereAlpha->value());  
		gl::draw( *mMesh );
	}
}