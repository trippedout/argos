#include "ThreadedLoadVideoSurface.h"

using namespace ci;
using namespace ci::app;
using namespace ci::qtime;

ThreadedLoadVideoSurface::ThreadedLoadVideoSurface(std::string assetPath):DisplayObjectContainer(),
	assetPath(assetPath),
	threadLoading(false), 
	deleteAfterLoad(false),
	autoPlay(true),
	video(NULL)
{
}

ThreadedLoadVideoSurface::~ThreadedLoadVideoSurface(void)
{
	ci::app::console() << "~ThreadedLoadVideoSurface" << std::endl;
	deleteVideo();
}

void ThreadedLoadVideoSurface::loadVideo()
{
	if(video || threadLoading) return; 
	ci::app::console() << "ThreadedLoadVideoSurface::loadVideo()" << std::endl;

	deleteAfterLoad = false; 
    threadLoading = true; 
	//loaderThread = new std::thread( std::bind(&ThreadedLoadVideoSurface::threadLoaded, this));

	threadLoaded();
}

void ThreadedLoadVideoSurface::threadLoaded()
{
	//ci::ThreadSetup threadSetup;

    threadLoading = false; 
	
	video = VideoObject::create( new qtime::MovieGl( loadAsset(assetPath) ) );

	if(deleteAfterLoad){
		ci::app::console() << "ThreadedLoadVideoSurface::threadLoaded() load finished, deleteing" << std::endl;
		deleteVideo();
		deleteAfterLoad = false; 
	}else{
		ci::app::console() << "ThreadedLoadVideoSurface::threadLoaded() loaded, playing" << std::endl;
		addChild(video);
		video->reset();
		video->play();
		
		if(!autoPlay){
			stop();
		}
	}
}

void ThreadedLoadVideoSurface::deleteVideo()
{
	if(video){
		ci::app::console() << " ~ThreadedLoadVideoSurface() deleting video pointer " << std::endl;
		
		removeChild(video);
		video->stop();
		video.reset(); 
		video = NULL;
	}else if(threadLoading){
		deleteAfterLoad = true; 
	}
}

void ThreadedLoadVideoSurface::update(float dt)
{
	DisplayObjectContainer::update(dt);
}

void ThreadedLoadVideoSurface::render()
{
	DisplayObjectContainer::render();
}

void ThreadedLoadVideoSurface::nextScene()
{
	if( !video || threadLoading ) return; 

	video->nextScene();
}

void ThreadedLoadVideoSurface::prevScene()
{	
	if(!video || threadLoading) return; 

	video->prevScene();
}

void ThreadedLoadVideoSurface::play()
{
	if(!video || threadLoading) return; 
	video->play(); 
	playing = true; 
	ci::app::console()<<"ThreadedLoadVideoSurface::play "<<std::endl;
}

void ThreadedLoadVideoSurface::stop()
{
	if(!video || threadLoading) return; 
	video->stop(); 
	playing = false;
	ci::app::console()<<"ThreadedLoadVideoSurface::stop "<<std::endl;
}

void ThreadedLoadVideoSurface::reset()
{
	//stop();
	if(video || threadLoading) video->reset(); 
}

float ThreadedLoadVideoSurface::getDuration()
{
	if(!video || threadLoading) return -1.0f; 
	else return video->getDuration(); 
}

float ThreadedLoadVideoSurface::getTime()
{
	if(!video || threadLoading) return -1.0f; 
	else return video->getTime(); 
}

void ThreadedLoadVideoSurface::setTime(float val)
{
	if(video || threadLoading) video->setTime(val); 
}
