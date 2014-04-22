#pragma once

#include "InputView.h"
#include "ImageView.h"
#include "VideoObject.h"
#include "EndViewNextEpisodeBtn.h"
#include "ShareView.h"
#include "EndViewSmallButton.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

class EndView : public InputView
{
	typedef boost::shared_ptr<EndViewNextEpisodeBtn> NextEpisodeBtnRef;
	typedef boost::shared_ptr<ShareView> ShareViewRef;
	typedef boost::shared_ptr<EndViewSmallButton> EndViewSmallButtonRef;
protected:
	ImageViewRef		mNextVideoThumb;
	ImageViewRef		bgBox; 
	VideoViewRef		mMainVideo;
	VideoViewRef		mVideoLoop;
	
	NextEpisodeBtnRef	mNextEpisodeBtn;

	ShareViewRef		mShareView;

	EndViewSmallButtonRef	mShareBtn;
	EndViewSmallButtonRef	mHandbookBtn;

	ci::gl::GlslProgRef				mBlurShader;
	ci::Anim<float>				mBlurAmount;
	boost::shared_ptr<ci::gl::Fbo>	mFbo;
	boost::shared_ptr<ci::gl::Fbo>	mVideoFbo, mVideoFbo2;

	void onMovieShrink();

public:
	EndView(Assets* assets, VideoViewRef mainVideo);
	~EndView(void);
		
	virtual void update(float dt);
	virtual void prerender();
	virtual void render();

	virtual void onAnimateIn();
	virtual void onAnimateOut();
	virtual void handleInput(INPUT_TYPE type);
};

