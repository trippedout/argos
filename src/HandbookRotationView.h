#pragma once
#include "RotationView.h"

class HandbookRotationView :
	public RotationView
{
public:
	HandbookRotationView(std::string assetPath, ci::gl::GlslProgRef glShader);
	~HandbookRotationView(void);

	virtual void update(float dt);

	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 
	
	virtual void setSpeed(float val); 

	void tweenOutComplete(); 

protected: 
	
	ci::Anim<float>* tweenTime; 

	virtual void playIn(); 
	virtual void playOut(); 


	bool looping, tweening; 
	float introTime, endTime, loopTime; 
};

                                                                            