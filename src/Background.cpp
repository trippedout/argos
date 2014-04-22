#include "Background.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Background::Background(Assets* ass): View(ass)
{
	particleTexture = ass->getTextureByID("particle"); 

	int numParticles = 1000;
	float w=2000, h=2000, d=2000;
	float x, y, z, rs, rx, ry, rz; 

	for(int i=0; i<numParticles - 20; i++){
		rs = (float)rand()/(float)RAND_MAX;
		rx = (float)rand()/(float)RAND_MAX;
		ry = (float)rand()/(float)RAND_MAX;
		rz = (float)rand()/(float)RAND_MAX;

		x = 1920;
		y = 500 + rx * 100 - 50; 
		z = rz * -500 + 250;

		particles.push_back( Particle( ci::Vec3f( x, y, z ), 2 + 25 * rs ) ); 
	}
}

void Background::render(ci::Vec3f* camPos, ci::Vec3f* camTarget)
{
	gl::enableAdditiveBlending(); 
	particleTexture->enableAndBind(); 
	
	for( list<Particle>::iterator it = particles.begin(); it != particles.end(); ++it ) {
		it->render(camPos, camTarget); 
	}

	particleTexture->unbind(); 
	gl::enableAlphaBlending();
}

void Background::update(float dt)
{
	for( list<Particle>::iterator pointIter = particles.begin(); pointIter != particles.end(); ++pointIter ) {
		pointIter->update(dt); 
	}
}

void Background::changeView(Settings::ViewID)
{
	//TODO: do stuff based on what view is bout to show up. 
}

Background::~Background(void)
{

}
