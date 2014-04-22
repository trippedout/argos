#include "Particle.h"


Particle::Particle(ci::Vec3f p, float s):DisplayObject()
{
	position = p; 
	size = s; 
	vel = ci::Vec3f( (float)rand()/(float)RAND_MAX* -30.0f - 40.0f, 0,0);
	alpha = 1.0f;
	start = ci::Vec3f(p.x, p.y, p.z); 
	
	blink = (float)rand()/(float)RAND_MAX * (float)M_PI; 

	a = (float)rand()/(float)RAND_MAX * (float)M_PI;
	aSpeed = (float)rand()/(float)RAND_MAX * 0.3f + 0.1f;

	life = 0;
	maxLife = 10.0f + 15.0f * ((float)rand()/(float)RAND_MAX) + size/1000; 
}

void Particle::update(float dt)
{
	life -= dt;

	if(life <= 0){
		alpha = 1.0f;
		life = maxLife;
		position.value().x = start.x;
		position.value().y = start.y;
		position.value().z = start.z;

		vel.y = 0; 
		vel.z = 0; 
	}

	a += dt * aSpeed * 5.0f; 

	blink += dt * 5.0f; 

	vel.y += cos(a) * dt * 10.0f; 
	vel.z += sin(a) * dt * 10.0f; 

	position.value() += vel* dt; 

	alpha = 1.0f * ((life * 2.0f) / maxLife) + sin(blink) * 0.4f;
}

void Particle::render(ci::Vec3f* camPos, ci::Vec3f* camTarget)
{
	float deg = (float)57.2957795;
	float z, l, deltaX, deltaY, deltaZ, angleZ, hyp, angleY; 
	
	l = abs( sqrt( pow(position.value().x - camPos->x, 2) + pow(position.value().y - camPos->y, 2) + pow(position.value().z - camPos->z, 2) ) ); 
	z = 1 - ( l / 1000 );

	deltaX = camTarget->x - camPos->x;
	deltaY = camTarget->y - camPos->y;
	deltaZ = camTarget->z - camPos->z;
	angleZ   = atan2( deltaY,deltaX ); 
	hyp = sqrt( pow( deltaX, 2 ) + pow( deltaY, 2 ) ); 
	angleY   = atan2( hyp, deltaZ );

	glPushMatrix();
	glTranslatef( position.value().x, position.value().y, position.value().z );
	glRotatef( angleZ * deg, 0, 0, 1.0f );
	glRotatef( angleY * deg, 0, 1.0f, 0 );
	glScalef( scale.value().x * size, scale.value().y * size, scale.value().z * size );

	//TODO: less red and green when farther away.
	//TODO: DoF with spritesheets. 

	glColor4f( 1.0f, 1.0f, 1.0f, alpha );

	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f(-.5, -.5);
		glTexCoord2f(1, 0);    glVertex2f( .5, -.5);
		glTexCoord2f(1, 1);    glVertex2f( .5,  .5);
		glTexCoord2f(0, 1);    glVertex2f(-.5,  .5);
	glEnd();
	glPopMatrix();
}

Particle::~Particle(void)
{

}
