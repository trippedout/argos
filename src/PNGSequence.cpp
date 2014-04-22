#include "PNGSequence.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

PNGSequence::PNGSequence(std::string path, int num, float frameRate):ImageView(),
	totalFrames(num),
	time(0),
	frame(0),
	framerate(framerate),
	frameTime(0),
	duration(num * 1.0f/framerate),
	looping(false), 
	playing(false)
{	
	ci::app::console()<< "PNGSequence loading images" <<std::endl;

	std::string n; 
	for(int i=0; i<totalFrames; i++){
		n = ""; 
		if(i<10) n+= "00";
		else if(i<100) n+= "0";
		n += cinder::toString(i);
		std::string p = path + "00" + n + ".png";
		ci::app::console()<< "\t"<< p <<std::endl;
		textures.push_back( new ci::gl::Texture( loadImage(loadAsset( p ))) );
	}

	setTexture(textures[frame]);
}

PNGSequence::~PNGSequence(void)
{
}

void PNGSequence::update(float dt)
{
	if(playing){
		time += dt;
		if(time > duration){
			if(looping){
				time-=duration;
				frameTime = time;
				frame=0;
			}else{
				time = duration; 
				frameTime = 0; 
			}
		}

		frameTime+=dt; 
		if(frameTime > 1.0f/framerate){
			frameTime -= 1.0f/framerate;
			frame++;
			setTexture(textures[frame]);
		}
	}
}

void PNGSequence::play()
{
	playing = true; 
}

void PNGSequence::stop()
{
	playing = false; 
}

