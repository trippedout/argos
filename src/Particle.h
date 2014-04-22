#pragma once

#include "DisplayObject.h"

class Particle : public DisplayObject
{
	ci::Vec3f start, vel;
	float size; 
	float alpha; 
	float life; 
	float maxLife;
	float rotation; 
	float a; 
	float aSpeed;
	float blink; 

public:
	Particle(ci::Vec3f p, float s);
	~Particle(void);

	virtual void render(ci::Vec3f* camPos, ci::Vec3f* camTarget); 
	virtual void update(float dt); 
};

