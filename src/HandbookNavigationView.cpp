#include "HandbookNavigationView.h"

using namespace ci;
using namespace ci::app;

HandbookNavigationView::HandbookNavigationView(HandbookBackgroundViewRef background, Assets* assets) : 
	InputView(assets),
	mTabsView( HandbookTabsRef( new HandbookTabs(assets) ) ),
	mItemsList( HandbookListRef( new HandbookList(assets, assets->getData()->getHBItems()) ) ),
	mCharactersList( HandbookListRef( new HandbookList(assets, assets->getData()->getHBCharacters()) ) ),
	mLocationsList( HandbookListRef( new HandbookLocationList(assets) ) ),
	mOniList( HandbookListRef( new HandbookList(assets, assets->getData()->getHBOniFiles()) ) ),
	mCurrentList(NULL),
	mBackground(background)
{

	//tabs
	mTabsView->getTabChangeSignal()->connect(boost::bind(&HandbookNavigationView::onTabChange, this, boost::arg<1>::arg()));
	this->addChild(mTabsView);

	//create all lists
	ci::Vec3f listPos = ci::Vec3f(0.f, 70.f, 0.f);

	//items
	mItemsList->setPosition(listPos);
	mTabListMap[HandbookTabs::Tab::ITEMS] = mItemsList;
	this->addChild(mItemsList);
	
	//items
	mCharactersList->setPosition(listPos);
	mTabListMap[HandbookTabs::Tab::CHARACTERS] = mCharactersList;
	this->addChild(mCharactersList);
	
	//items
	mLocationsList->setPosition(listPos);
	mTabListMap[HandbookTabs::Tab::LOCATIONS] = mLocationsList;
	this->addChild(mLocationsList);
	
	//items
	mOniList->setPosition(listPos);
	mTabListMap[HandbookTabs::Tab::ONI_FILES] = mOniList;
	this->addChild(mOniList);
	
	onTabChange(HandbookTabs::Tab::ITEMS);
}

void HandbookNavigationView::showFirstTab()
{
	mTabsView->showFirstTab(); 
	//onTabChange(HandbookTabs::Tab::ITEMS);
}

void HandbookNavigationView::showOniTab()
{
	//set proper tab
	if(mCurrentList != mTabListMap[HandbookTabs::Tab::ONI_FILES] ) mTabsView->showOniTab();
	
	//then make list choose proper item
	
	//first close all open lists
	mCurrentList->closeOpenLists();

	//select top item, then move selection to list item
	mCurrentList->setSelection(ci::Vec2i(0,0));
	mCurrentList->handleInput(INPUT_TYPE::SELECT);
	mCurrentList->setSelection(ci::Vec2i(0,1));
}

HandbookNavigationView::~HandbookNavigationView(void)
{
	mTabsView.reset();
	mItemsList.reset();
	mCurrentList.reset();
	mCharactersList.reset();
	mLocationsList.reset();
	mOniList.reset();
}

void HandbookNavigationView::onTabChange(HandbookTabs::Tab tab)
{
	//alpha out current list
	if(mTabListMap[tab] == mCurrentList) return; 

	ci::app::console() << "onTabChange()" << std::endl;

	ci::Vec3f listPos = ci::Vec3f(0.f, 70.f, 0.f);
	ci::Vec3f offset = ci::Vec3f(0.f, 0, 100.f);

	if(mCurrentList){
		mCurrentList->onAnimateOut();
	}

	mCurrentList = mTabListMap[tab];
	mCurrentList->getSubItemClickedSignal()->connect(boost::bind(&HandbookNavigationView::onSubItemClicked, this, boost::arg<1>::arg()));
	mCurrentList->getLocationItemClickedSignal()->connect(boost::bind(&HandbookNavigationView::onLocationItemClicked, this, boost::arg<1>::arg()));
	mCurrentList->getSubItemSelectedSignal()->connect(boost::bind(&HandbookNavigationView::onSubItemSelected, this, boost::arg<1>::arg()));
	
	mCurrentList->setPosition(listPos + offset);
	timeline().apply(mCurrentList->getPosition(), listPos, 0.4f, EaseOutQuint()); 
	mCurrentList->onAnimateIn();
}

void HandbookNavigationView::onSubItemSelected(int val)
{
	mBackground->setConstellation(val); 
}

void HandbookNavigationView::onSubItemClicked(DataNodeRef dataNode)
{
	mDataSelectedSignal(dataNode);
}

void HandbookNavigationView::onLocationItemClicked(LocationNodeRef locationNode)
{
	mLocationSelectedSignal(locationNode);
}

void HandbookNavigationView::render()
{
	InputView::render();
}

void HandbookNavigationView::handleInput(INPUT_TYPE type)
{
	if(mTabsView) mTabsView->handleInput(type);
	if(mCurrentList) mCurrentList->handleInput(type);
}

void HandbookNavigationView::onAnimateOut()
{
	mCurrentList->onAnimateOut();
	mTabsView->onAnimateOut();
}

void HandbookNavigationView::onAnimateIn()
{
	mTabsView->onAnimateIn();
	mCurrentList->onAnimateIn();

	ci::Vec3f listPos = ci::Vec3f(0.f, 70.f, 0.f);
	ci::Vec3f offset = ci::Vec3f(0.f, 0, 100.f);
	
	mTabsView->setPosition(offset);
	mCurrentList->setPosition(listPos + offset);

	timeline().apply(mTabsView->getPosition(), ci::Vec3f(), 0.3f, EaseOutQuint());
	timeline().apply(mCurrentList->getPosition(), listPos, 0.4f, EaseOutQuint()); 

	
	mCurrentList->getSubItemClickedSignal()->connect(boost::bind(&HandbookNavigationView::onSubItemClicked, this, boost::arg<1>::arg()));
	mCurrentList->getLocationItemClickedSignal()->connect(boost::bind(&HandbookNavigationView::onLocationItemClicked, this, boost::arg<1>::arg()));
	mCurrentList->getSubItemSelectedSignal()->connect(boost::bind(&HandbookNavigationView::onSubItemSelected, this, boost::arg<1>::arg()));
}