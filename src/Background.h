#pragma once
#include "View.h"
#include "Settings.h"
#include "Particle.h"

class Background : public View
{
protected:
	ci::gl::Texture* particleTexture; 
	std::list<Particle> particles;

public:
	Background(Assets* ass);

	virtual void render(ci::Vec3f* camPos, ci::Vec3f* camTarget); 
	virtual void update(float dt);
	void changeView(Settings::ViewID); 

	~Background(void);
};
