#include "VideoObject.h"

using namespace ci;
using namespace ci::app;

VideoObject::VideoObject(qtime::MovieGl* vid)
{
	video = vid;
	playing = false;
}

VideoObject::VideoObject(qtime::MovieGl* vid, std::vector< float > sc)
{
	video = vid; 
	scenes = sc; 
	scene = 0; 
	playing = false;
}

VideoObject::~VideoObject(void)
{
	ci::app::console() << " ~VideoObject() deleting video pointer " << std::endl;
	if(video){
		video->stop();
		delete video;
	}
}

VideoViewRef VideoObject::create(qtime::MovieGl* vid)
{
	return boost::shared_ptr<VideoObject>( new VideoObject(vid) );
}

void VideoObject::update(float dt)
{
	/*if(video->isDone())
	{
		ci::app::console() << "VideoObject::update()" << " video is done!" << std::endl;
		return;
	}*/

	if(video && playing && !video->isPlaying() && video->checkPlayable() ){
		video->play(); 
	}

	int size = scenes.size();
	if(size == 0) return; 
	
	float time = video->getCurrentTime();

	//probably a more efficient way of doing this...

	scene = 0; 

	for(int i=1; i<size; i++){
		if(scenes[i] > time ){
			scene = i-1; 
			return; 
		}
	}

	scene = size-1; 
}

void VideoObject::render()
{
    try {
        
        const ci::gl::Texture tex = video->getTexture();
        
        if(tex){

            ci::Rectf rect = ci::Rectf( tex.getBounds() );
            float w = rect.x2 - rect.x1; 
            float h = rect.y2 - rect.y1; 
            rect.x1 -= center.x * w; 
            rect.x2 -= center.x * w;
            rect.y1 -= center.y * h; 
            rect.y2 -= center.y * h;

            //ci::app::console() << "VideoObject::render() " << rect << std::endl;
            if(additiveBlending) gl::enableAdditiveBlending(); 

            glPushMatrix(); 
                glTranslatef(position.value().x, position.value().y, position.value().z); 
                glRotatef(rotation.value().x, 1.0f, 0, 0);
                glRotatef(rotation.value().y, 0, 1.0f, 0);
                glRotatef(rotation.value().z, 0, 0, 1.0f);
                glScalef(scale.value().x, scale.value().y, scale.value().z);
                glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
                
                gl::draw(tex, rect); 

            glPopMatrix();

            if(additiveBlending) gl::enableAlphaBlending(); 
        }
    }
    catch(const std::exception &e)
    {
        console() << e.what() << std::endl;
    }

}

void VideoObject::nextScene()
{
	int size = scenes.size();

	if( size == 0 ) {
		
		float t = video->getCurrentTime() + 30.0f;
		if( t > video->getDuration() ) t = video->getDuration();
		video->seekToTime(t); 
	
	}else{
		if( scene+1 <= size-1){
			scene++;
			video->seekToTime(scenes[scene]);
		}else{
			//hack because if you seek to the end it stops and dosent render. 
			video->seekToTime(video->getDuration()*0.99f);
		}
	}
}

void VideoObject::prevScene()
{	
	//TODO: make this not suck

	if( scenes.empty() ) {
		
		float t = video->getCurrentTime() - 30.0f;
		if(t < 0) t = 0;
		video->seekToTime(t); 
	
	}else{
		if( scene-1 >= 0 ){
			scene--;
		}
		video->seekToTime(scenes[scene]);
	}
}

void VideoObject::setInternalAlpha()
{
	this->setAlpha(this->getAlpha()->value());
}

void VideoObject::play()
{
	playing = true; 
	
	if( video && video->checkPlayable() && !video->isPlaying() ){
		ci::app::console()<<"VideoObject::play video playing normally"<<std::endl;
		video->play(); 
	}else{
		ci::app::console()<<"VideoObject::play "<<std::endl;
		if(video){
			ci::app::console()<<"/t video playable:  "<< video->checkPlayable() <<std::endl;
			ci::app::console()<<"/t video is playing:  "<< video->isPlaying() <<std::endl;
		}else{
			ci::app::console()<<"/t video pointer is NULL "<<std::endl;
		}
	}
}

void VideoObject::stop()
{
	playing = false;
	if(video && video->isPlaying()){
		video->stop(); 
	}
}

void VideoObject::reset()
{
	video->seekToStart(); 
	scene = 0; 
}

float VideoObject::getDuration()
{
	return video->getDuration(); 
}

float VideoObject::getTime()
{
	return video->getCurrentTime(); 
}

void VideoObject::setTime(float val)
{
	video->seekToTime(val); 
}

qtime::MovieGl* VideoObject::getVideo()
{
	return video; 
}
