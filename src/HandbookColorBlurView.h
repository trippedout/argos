#pragma once
#include "DisplayObjectContainer.h"
#include "Assets.h"
#include "ImageView.h"
#include "cinder/Rand.h"

typedef boost::shared_ptr<class HandbookColorBlurView> HandbookColorBlurViewRef;

class HandbookColorBlurView :
	public DisplayObjectContainer
{

protected:
	struct Particle {
		ImageViewRef image; 
		float angle; 
		float speed; 
		float seed; 
		ci::Vec3f pos; 

		Particle(ImageViewRef image, float angle, float speed, float seed) 
			: image(image), angle(angle), speed(speed), seed(seed) {}
	};

	std::vector<Particle*> particles; 

public:
	HandbookColorBlurView(Assets* assets);
	~HandbookColorBlurView(void);

	virtual void update(float dt); 
};

