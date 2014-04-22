#include "HostView.h"

using namespace ci;
using namespace ci::app;
using namespace std;

HostView::HostView(Assets* assets) : InputView(assets)
{
	//select back
	mAddBack = ImageViewRef( new ImageView( assets->getTextureByID("add_back") ) );
	mAddBack->setPosition(ci::Vec3f(180,950,0));
	this->addChild(mAddBack);

	mAnimMap["title_in"] = ci::Vec3f(162,509,0);
	mAnimMap["title_top"] = ci::Vec3f(178,149,0);

	mTitle = TextViewRef(new TextView("HOST/JOIN PARTY", mAssets->getFontByID(Assets::FileID::DEFAULT_FONT), ci::ColorA(1,1,1,1)));
	mTitle->setPosition(mAnimMap["title_top"]);//mAnimMap["title_in"]);
	this->addChild(mTitle);

	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "EGORAPTOR", "host_icon", true)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "JUSTINURFACE", "host_icon2", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "HOLYLASAGNA", "host_icon3", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "CHESTER", "host_icon4", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "LORDARANIII", "host_icon5", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "MARUMARU", "host_icon6", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "MISTERMYSTERY", "host_icon7", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "DANTHEMANFROMJAPAN", "host_icon8", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "JODICI", "host_icon", false)) ); 
	buttons.push_back( HostButtonViewRef(new HostButtonView(assets, "GREGORY", "host_icon9", false)) ); 


	useSelection = true; 

	int num_of_col = 2;
	int num_of_row = 5;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	int x=0,y=0; 
	for ( HostButtonIter it = buttons.begin(); it != buttons.end(); it++ )
	{
		selectionGrid[x][y] = (*it);

		(*it)->setPosition(ci::Vec3f(400.0f + 580.0f*x, 375.0f + 80.0f*y,0)); 
		addChild((*it));

		y++; 
		if(y >= num_of_row) {
			y=0; 
			x++;
		}
	}

	this->deselectAll();
	moveSelection(ci::Vec2i(0,0));

	//guide = new ImageView( new ci::gl::Texture( loadImage( loadAsset( "guides/host.png"))));
	//createParams();
	//params->addParam("title", &mTitle->getPosition()->value() );
}

HostView::~HostView(void)
{

}

void HostView::onAnimateIn()
{
	mTitle->setPosition(mAnimMap["title_in"]);
	timeline().apply(mTitle->getPosition(), mAnimMap["title_in"], mAnimMap["title_top"], 0.5f, EaseOutCubic());
	
	mAddBack->setAlpha(0.f);
	timeline().apply(mAddBack->getAlpha(), 1.0f, 0.5f, EaseOutCubic());

	int size = children.size();
	for(int i = 0; i < size; i++ )
	{
		DisplayObjectRef ptr = children[i];

		if(ptr != mTitle && ptr != mAddBack)
		{
			DisplayObjectContainerRef doc = boost::dynamic_pointer_cast<DisplayObjectContainer>(children[i]);
			doc->setAlpha(0.0f);
			
			Tween<float>::Options options = timeline().apply(doc->getAlpha(), 1.0f, 0.5f)
					.delay(0.2f + i*0.05f)
					.updateFn(std::bind(&DisplayObjectContainer::setInternalAlpha, doc));

			if(i == children.size() - 1) options.finishFn(std::bind(&View::onAnimateInComplete, this));			
		}			
	}
}

void HostView::onAnimateOut()
{
	View::onAnimateOut();
}

void HostView::handleInput(INPUT_TYPE type)
{
	//automatically handle movement
	InputView::handleInput(type);

	//check if selected is a hostButtonView
	HostButtonViewRef btn = boost::dynamic_pointer_cast<HostButtonView>(selectedObject);

	switch(type)
	{
	case SELECT:
		SoundController::getInstance().playSound(SoundController::SELECT);
		selectedObject->activate();
		break;

	case INVITE:
		if(btn)
			if(!btn->getIsInvited())
				selectedObject->activate();
		break;

	case UNINVITE:
		if(btn)
			if(btn->getIsInvited())
				selectedObject->activate();
		break;

	case BACK:
		signal(this, Settings::ViewID::BACK);
		break;

	case PRESSED_X:
	case HANDBOOK:
		signal(this, Settings::ViewID::HANDBOOK);
		break;
	}
}


void HostView::playMovementSound(ci::Vec2i selection)
{
	//all movement plays same sound, no need to find difference
	SoundController::getInstance().playSound(SoundController::MOVE_VERTICAL);
}