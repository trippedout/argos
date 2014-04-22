#include "HandbookColorBlurView.h"

using namespace ci;

HandbookColorBlurView::HandbookColorBlurView(Assets* assets)
{
	//std::vector<Particle> particles; 
	

	//Particle(ImageViewRef image, float angle, float speed, float seed)
	for(int i=0; i<10; i++){	
		Particle* particle; 
		particle = new Particle(ImageViewRef(new ImageView(assets->getTextureByID("hb_colorBlur"))), Rand::randFloat()*(float)M_PI*2.0f, 0.5f + Rand::randFloat(), 0.5f + Rand::randFloat()*0.75f);
		particle->image->setCenter(ci::Vec3f(0.45f,0.5f,0)); 
		particle->image->setAdditiveBlending(true); 
		particle->image->setScale( ci::Vec3f(particle->seed, particle->seed, 0) ); 
		addChild(particle->image);
		particles.push_back(particle); 
	}

	particles[0]->pos = ci::Vec3f(1100,300,0); 
	particles[1]->pos = ci::Vec3f(1200,330,0); 
	particles[2]->pos = ci::Vec3f(1500,600,0); 
	particles[3]->pos = ci::Vec3f(1550,430,0); 
	particles[4]->pos = ci::Vec3f(1600,480,0); 
	particles[5]->pos = ci::Vec3f(1350,624,0); 
	particles[6]->pos = ci::Vec3f(1570,234,0); 
	particles[7]->pos = ci::Vec3f(1300,490,0);
	particles[8]->pos = ci::Vec3f(1640,200,0); 
	particles[9]->pos = ci::Vec3f(1500,264,0); 
}

void HandbookColorBlurView::update(float dt)
{
	for(unsigned int i=0; i<particles.size(); i++){
		particles[i]->angle += dt * particles[i]->speed;
		if(particles[i]->angle > (float)M_PI*2.0f)particles[i]->angle -= (float)M_PI*2.0f;

		float rot = particles[i]->image->getRotation()->value().y;
		rot += dt * particles[i]->speed * 0.1f;
		if(rot > 360.0f) rot -= 360.0f;

		particles[i]->image->setAlpha(0.25f * particles[i]->seed + sin(particles[i]->angle) * 0.25f );
		particles[i]->image->setRotation( ci::Vec3f(0,0, rot) );
		particles[i]->image->setPosition( particles[i]->pos + ci::Vec3f( cos(particles[i]->angle) * 50.0f, sin(particles[i]->angle) * 100.0f, 0 ) );
	}
}

HandbookColorBlurView::~HandbookColorBlurView(void)
{
	for(int i=0; i<10; i++)
	{
		delete particles[i];
	}
}
