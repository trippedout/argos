#include "HandbookTabs.h"

using namespace ci;
using namespace ci::app;

HandbookTabs::HandbookTabs(Assets* assets) : 
	InputView(assets),
	mItemsTab(HandbookTabViewRef( new HandbookTabView(assets, "ITEMS") ) ),
	mCharactersTab(HandbookTabViewRef( new HandbookTabView(assets, "CHARACTERS") ) ),
	mLocationsTab(HandbookTabViewRef( new HandbookTabView(assets, "LOCATIONS") ) ),
	mOniTab(HandbookTabViewRef( new HandbookTabView(assets, "ONI FILES") ) ),
	mTabBorder( ImageViewRef(new ImageView(assets->getTextureByID("hb_tab1") ) ) ),
	mTabSelect1( ImageViewRef(new ImageView(assets->getTextureByID("hb_tabSelect1") ) ) ),
	mTabSelect2( ImageViewRef(new ImageView(assets->getTextureByID("hb_tabSelect1") ) ) ),
	mTabSelect3( ImageViewRef(new ImageView(assets->getTextureByID("hb_tabSelect1") ) ) )
{


	//createGuide("guides/handbook_tabs.png");
	ci::Vec3f tabScale = ci::Vec3f(1.0f, 1.0f, 0.f);
	float offsetX = -2.0f;
	float offsetY = 12.0f;
	
	mItemsTab->setScale(tabScale);
	mItemsTab->setPosition(ci::Vec3f(15.0f + offsetX, offsetY,0));
	mContentMap[mItemsTab] = ITEMS;
	this->addChild(mItemsTab);

	mCharactersTab->setScale(tabScale);
	mCharactersTab->setPosition(ci::Vec3f(128.0f + offsetX, offsetY,0));
	mContentMap[mCharactersTab] = CHARACTERS;
	this->addChild(mCharactersTab);
	
	mLocationsTab->setScale(tabScale);
	mLocationsTab->setPosition(ci::Vec3f(299.0f + offsetX, offsetY,0));
	mContentMap[mLocationsTab] = LOCATIONS;
	this->addChild(mLocationsTab);
	
	mOniTab->setScale(tabScale);
	mOniTab->setPosition(ci::Vec3f(460.0f + offsetX, offsetY,0));
	mContentMap[mOniTab] = ONI_FILES;
	this->addChild(mOniTab);

	int num_of_col = 4;
	int num_of_row = 1;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	// hmm better way? maybe hard coding is the safest... not sure. 
	selectionGrid[0][0] = mItemsTab; 
	selectionGrid[1][0] = mCharactersTab; 
	selectionGrid[2][0] = mLocationsTab; 
	selectionGrid[3][0] = mOniTab; 
	
	mTabSelect1->setColor( ci::Vec3f(0, 0.5f, 0.99f) );
	mTabSelect2->setColor( ci::Vec3f(0, 0.5f, 0.99f) );
	mTabSelect3->setColor( ci::Vec3f(0, 0.5f, 0.99f) );
	
	mTabBorder->setAdditiveBlending(true);
	mTabSelect1->setAdditiveBlending(true);
	mTabSelect2->setAdditiveBlending(true);
	mTabSelect3->setAdditiveBlending(true);

	mTabSelect1->setAlpha(0);
	mTabSelect2->setAlpha(0);
	mTabSelect3->setAlpha(0);

	this->addChild(mTabBorder); 
	this->addChild(mTabSelect1); 
	this->addChild(mTabSelect2); 
	this->addChild(mTabSelect3); 

	mTabBorderTexMap[mItemsTab] = assets->getTextureByID("hb_tab1"); 
	mTabSelectTexMap[mItemsTab] = assets->getTextureByID("hb_tabSelect1"); 
	
	mTabBorderTexMap[mCharactersTab] = assets->getTextureByID("hb_tab2"); 
	mTabSelectTexMap[mCharactersTab] = assets->getTextureByID("hb_tabSelect2"); 
	
	mTabBorderTexMap[mLocationsTab] = assets->getTextureByID("hb_tab3"); 
	mTabSelectTexMap[mLocationsTab] = assets->getTextureByID("hb_tabSelect3"); 
	
	mTabBorderTexMap[mOniTab] = assets->getTextureByID("hb_tab4"); 
	mTabSelectTexMap[mOniTab] = assets->getTextureByID("hb_tabSelect4"); 


	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));
}

void HandbookTabs::showFirstTab()
{
	setSelection(ci::Vec2i(0,0));
}

void HandbookTabs::showOniTab()
{
	setSelection(ci::Vec2i(3,0));
}

HandbookTabs::~HandbookTabs(void)
{
	mItemsTab.reset();
	mCharactersTab.reset();
	mLocationsTab.reset();
	mOniTab.reset();
}

void HandbookTabs::setSelection(ci::Vec2i selection)
{
	InputView::setSelection(selection);

	HandbookTabViewRef tabView = boost::dynamic_pointer_cast<HandbookTabView>(selectedObject);
	
	mTabChangeSignal( mContentMap[tabView] );
		
	float t = 0.3f;
	
	mTabBorder->setTexture(mTabBorderTexMap[tabView]); 
	mTabSelect1->setTexture(mTabSelectTexMap[tabView]); 
	mTabSelect2->setTexture(mTabSelectTexMap[tabView]); 
	mTabSelect3->setTexture(mTabSelectTexMap[tabView]); 
	
	mTabSelect1->setPosition(ci::Vec3f()); 
	mTabSelect2->setPosition(ci::Vec3f()); 
	mTabSelect3->setPosition(ci::Vec3f()); 

	mTabSelect1->setAlpha(0);
	mTabSelect2->setAlpha(0);
	mTabSelect3->setAlpha(0);

	if( tabView->getLocked()){
		mTabSelect1->setColor( ci::Vec3f(0.5f, 0.5f, 0.5f) );
		mTabSelect2->setColor( ci::Vec3f(0.5f, 0.5f, 0.5f) );
		mTabSelect3->setColor( ci::Vec3f(0.5f, 0.5f, 0.5f) );
	}else{
		mTabSelect1->setColor( ci::Vec3f(0, 0.5f, 0.99f) );
		mTabSelect2->setColor( ci::Vec3f(0, 0.5f, 0.99f) );
		mTabSelect3->setColor( ci::Vec3f(0, 0.5f, 0.99f) );
	}

	timeline().apply( mTabSelect1->getPosition(), ci::Vec3f(1,1,0), t, EaseOutQuint() );
	timeline().apply( mTabSelect2->getPosition(), ci::Vec3f(-3,-3,0), t, EaseOutQuint() );
	timeline().apply( mTabSelect3->getPosition(), ci::Vec3f(5,5,0), t, EaseOutQuint() );

	timeline().apply( mTabSelect1->getAlpha(), 0.25f, t, EaseOutQuint() );
	timeline().apply( mTabSelect2->getAlpha(), 0.25f, t, EaseOutQuint() );
	timeline().apply( mTabSelect3->getAlpha(), 0.25f, t, EaseOutQuint() );

}

void HandbookTabs::onAnimateIn()
{
	DataController::UnlockedInfo info = mAssets->getData()->getUnlocked(); 

	mItemsTab->setLocked( info.items == 0 ); 
	mCharactersTab->setLocked( info.characters == 0 ); 
	mLocationsTab->setLocked( info.locations == 0 ); 
	mOniTab->setLocked( info.onis == 0 ); 


	float t = 0.3f;
	timeline().apply( mItemsTab->getAlpha(), 1.0f, t, EaseOutQuint() );
	timeline().apply( mCharactersTab->getAlpha(), 1.0f, t, EaseOutQuint() );
	timeline().apply( mLocationsTab->getAlpha(), 1.0f, t, EaseOutQuint() );
	timeline().apply( mOniTab->getAlpha(), 1.0f, t, EaseOutQuint() );

	timeline().apply( mTabBorder->getAlpha(), 1.0f, t, EaseOutQuint() );
	timeline().apply( mTabSelect1->getAlpha(), 0.25f, t, EaseOutQuint() );
	timeline().apply( mTabSelect2->getAlpha(), 0.25f, t, EaseOutQuint() );
	timeline().apply( mTabSelect3->getAlpha(), 0.25f, t, EaseOutQuint() );
}

void HandbookTabs::onAnimateOut()
{
	float t = 0.3f;
	timeline().apply( mItemsTab->getAlpha(), 0.0f, t, EaseOutQuint() );
	timeline().apply( mCharactersTab->getAlpha(), 0.0f, t, EaseOutQuint() );
	timeline().apply( mLocationsTab->getAlpha(), 0.0f, t, EaseOutQuint() );
	timeline().apply( mOniTab->getAlpha(), 0.0f, t, EaseOutQuint() );

	timeline().apply( mTabBorder->getAlpha(), 0.0f, t, EaseOutQuint() );
	timeline().apply( mTabSelect1->getAlpha(), 0.0f, t, EaseOutQuint() );
	timeline().apply( mTabSelect2->getAlpha(), 0.0f, t, EaseOutQuint() );
	timeline().apply( mTabSelect3->getAlpha(), 0.0f, t, EaseOutQuint() );
}



void HandbookTabs::handleInput(INPUT_TYPE type)
{
	InputView::handleInput(type);
}