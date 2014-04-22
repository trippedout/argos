#include "EndView.h"

using namespace ci::gl;
using namespace ci::app;
using namespace ci::qtime;

EndView::EndView(Assets* assets, VideoViewRef mainVideo) : 
	InputView(assets),
	mMainVideo(mainVideo),
	mNextVideoThumb( ImageViewRef( new ImageView( assets->getTextureByID("end_next_episode_thumb") ) ) ),
	mVideoLoop( NULL ),
	mNextEpisodeBtn( NextEpisodeBtnRef( new EndViewNextEpisodeBtn(assets) ) ),
	mShareView( ShareViewRef( new ShareView(assets) ) ) ,
	mShareBtn( EndViewSmallButtonRef( new EndViewSmallButton( assets, "SHARE" ) ) ),
	mHandbookBtn( EndViewSmallButtonRef( new EndViewSmallButton( assets, "HANDBOOK" ) ) ),
	bgBox(ImageViewRef( new ImageView(assets->getTextureByID("bgBox")))),
	mBlurShader(assets->getShaderByID("endscreen_blur")),
	mBlurAmount(0.f)
{

	mNextVideoThumb->setPosition(ci::Vec3f(960.f,0.f,0.f));
	this->addChild(mNextVideoThumb);

	mNextEpisodeBtn->setPosition(ci::Vec3f(960.f,200.f,0.f));
	this->addChild(mNextEpisodeBtn);
	
	
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setAlpha(0.f);
	this->addChild( bgBox );

	//shareview
	mShareView->setPosition(ci::Vec3f(385.f, 375.f, 0.f));
	this->addChild( mShareView );

	mShareBtn->setPosition(ci::Vec3f(192.f, 538.f, 0.f));
	this->addChild(mShareBtn);

	mHandbookBtn->setPosition(ci::Vec3f(387.f, 538.f, 0.f));
	this->addChild(mHandbookBtn);
	
	//set selection
	int num_of_col = 2;
	int num_of_row = 2;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	selectionGrid[0][0] = mNextEpisodeBtn; 
	selectionGrid[1][0] = mNextEpisodeBtn; 
	selectionGrid[0][1] = mShareBtn; 
	selectionGrid[1][1] = mHandbookBtn; 
	
	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));

	//fbo stuffs
	Fbo::Format format;
	format.setSamples(16);
	mFbo = boost::shared_ptr<Fbo>(new Fbo(1920,1080,format));
	mVideoFbo = boost::shared_ptr<Fbo>(new Fbo(1920, 1080, format));
	mVideoFbo2 = boost::shared_ptr<Fbo>(new Fbo(1920, 1080, format));
}

EndView::~EndView(void)
{
	mMainVideo.reset();
	mVideoLoop.reset();
	mNextVideoThumb.reset();
}

void EndView::onAnimateIn()
{
	//mVideoLoop->setTime(0.f);

	timeline().apply(mMainVideo->getScale(),ci::Vec3f(1.f, .835f, 1.f), 1.f)
		.finishFn(boost::bind(&EndView::onMovieShrink, this));
}

void EndView::onMovieShrink()
{	
	if(!mVideoLoop){
		mVideoLoop = VideoViewRef( new VideoObject(new MovieGl( loadAsset("scenes/endscreen/endmenu.mp4") ) ) );
		mVideoLoop->setPosition(ci::Vec3f(0.f,450.f,0.f));
		mVideoLoop->setAdditiveBlending(true);
		mVideoLoop->getVideo()->setLoop(true);
		this->addChild(mVideoLoop);
		mVideoLoop->play();
	}
	
	onAnimateInComplete();
}

void EndView::onAnimateOut()
{
	if(mVideoLoop){
		removeChild(mVideoLoop);
		mVideoLoop->stop();
		mVideoLoop.reset();
		mVideoLoop = NULL;
	}

	mMainVideo->setScale(ci::Vec3f(2.f,2.f,1.f));

	onAnimateOutComplete();
}

void EndView::update(float dt)
{
	InputView::update(dt);
}

void EndView::prerender()
{
	//render video loop first
	//mVideoFbo->bindFramebuffer();
	//{
	//	gl::clear(ci::ColorA(0,0,0,0.f));
	//	
	//	mVideoLoop->render();
	//}
	//mVideoFbo->unbindFramebuffer();
	//
	//mVideoFbo2->bindFramebuffer();
	//{
	//	mBlurShader->bind();
	//	{
	//		glColor4f(1.f,1.f,1.f,1.f);
	//	
	//		mBlurShader->uniform("blur", mBlurAmount);
	//		mBlurShader->uniform("alpha", 1.f);
	//	
	//		glColor4f(1.f,1.f,1.f,1.f);

	//		gl::draw(mVideoFbo->getTexture(), ci::Rectf(0.f,0.f,1920.f,1080.f));
	//	}
	//	mBlurShader->unbind();
	//}
	//mVideoFbo2->unbindFramebuffer();

	////render the rest
	//mFbo->bindFramebuffer();
	//{
	//	gl::clear(ci::ColorA(0,0,0,0.f));
	//
	//	mNextVideoThumb->render();
	//	mNextEpisodeBtn->render();

	//	mShareBtn->render();
	//	mHandbookBtn->render();

	//	mMainVideo->render();
	//}
	//mFbo->unbindFramebuffer();
}

void EndView::render()
{		
	/*mBlurShader->bind();
	{
		glColor4f(1.f,1.f,1.f,1.f);
		
		mBlurShader->uniform("blur", mBlurAmount);
		mBlurShader->uniform("alpha", 1.f);
		
		glColor4f(1.f,1.f,1.f,1.f);
		
		ci::gl::Texture* texture = &mFbo->getTexture();
		texture->setFlipped(true);

		gl::draw(*texture, ci::Rectf(0.f,0.f,1920.f,1080.f));

	}
	mBlurShader->unbind();
	
	ci::gl::Texture* texture = &mVideoFbo2->getTexture();

	gl::enableAdditiveBlending();
	gl::draw(*texture, ci::Rectf(0.f,0.f,1920.f,1080.f));
	gl::enableAlphaBlending();*/
	
	if(mVideoLoop) mVideoLoop->render();
	
	mNextVideoThumb->render();
	mNextEpisodeBtn->render();

	mShareBtn->render();
	mHandbookBtn->render();

	mMainVideo->render();

	bgBox->render();
	mShareView->render();
}

void EndView::handleInput(INPUT_TYPE type)
{
	InputView::handleInput(type);

	switch(type)
	{
	case BACK:
		if(mShareView->getIsShowing()){
			mShareView->onAnimateOut();
			timeline().apply(bgBox->getAlpha(),0.0f, 0.3f);
		}
		else{
			mAssets->getData()->setEpisodeComplete(false);
			mMainVideo->stop();
			mMainVideo->reset(); 
			signal(this, Settings::ViewID::START);
		}
		break;
		
	case SELECT:
		if(!mShareView->getIsShowing())
		{
			if(selectedObject == mShareBtn)
			{
				mShareView->onAnimateIn();
				timeline().apply(bgBox->getAlpha(),0.85f, 0.3f);
			}
			else if(selectedObject == mHandbookBtn)
			{
				mAssets->getData()->setEpisodeComplete(false);
				mMainVideo->stop();
				mMainVideo->reset(); 
				signal(this, Settings::ViewID::HANDBOOK);
			}
		}
		break;
        default:
            break;
	}

	if(mShareView->getIsShowing()) mShareView->handleInput(type);
}
