#include "DataViewAudio.h"

using namespace ci::app;
using namespace ci;

DataViewAudio::DataViewAudio(boost::shared_ptr<DataNode> dn, std::string assetID, std::string assetP,  Assets* assets) : InputView(assets)
{
	assetPath = assetP;
	dataNode = dn;
	thumbnail = ImageViewRef( new ImageView(assets->getTextureByID(assetID) ) );
	thumbnail->setScale(ci::Vec3f( 1.5f, 1.5f, 1.5f )); 
	thumbnail->setCenter(ci::Vec3f( 0.5f, 0.5f, 0.5f )); 
	thumbnail->setPosition(ci::Vec3f( 1200.0f-20, 230.0f - 60 - 70, 0.0f)); 
	thumbnail->setAdditiveBlending(true);
	thumbnail->setAlpha(0.7f);
	//thumbnail->setColor(ci::Vec3f(1,0,0));

	video = NULL;
	
	playing = false;

	addChild(thumbnail); 
}

DataViewAudio::~DataViewAudio(void)
{
	disposeTempAssets();
}

void DataViewAudio::disposeTempAssets()
{
	playing = false;

	if(video){
		ci::app::console() << "DataViewAudio deleteing video!" << std::endl;
		
		removeChild(video);
		video->stop();
		video.reset();
		video = NULL;

		addChild(thumbnail);
	}
}

void DataViewAudio::update(float dt)
{
	InputView::update(dt);

	if( video && playing ){

		float t = video->getVideo()->getCurrentTime();
		float d = video->getVideo()->getDuration();

		if( t > d*0.99f){
			exitVideo();
		}
	}
}

void DataViewAudio::exitVideo()
{
	disposeTempAssets();
}

void DataViewAudio::onAnimateOut()
{
	exitVideo(); 
}

void DataViewAudio::handleInput(INPUT_TYPE type)
{

	switch(type)
	{
	case NEXT:
		if(video)video->nextScene();
		break;

	case PREVIOUS:
		if(video)video->prevScene();
		break;

	case BACK:
		//close video if its open. 
		exitVideo();
		break;

	case SELECT:
		//close video if its open. 
		if(!playing){
			playing = true; 

			if(!video){
				video = VideoViewRef( new VideoObject( new qtime::MovieGl( loadAsset(assetPath) ) ) );

				video->setAlpha(0.7f);
				removeChild(thumbnail);
				addChild(video); 

				video->setAdditiveBlending(true);
				video->reset();
				video->play();
				video->setCenter( *thumbnail->getCenter() ); 
				video->setScale( thumbnail->getScale()->value() ); 
				video->setPosition( thumbnail->getPosition()->value() );
			}

		}else{
			if(video->getVideo()->isPlaying()){
				video->stop();
			}else{
				video->play();
			}
		}
		break;

	default:
            break;
	}
}