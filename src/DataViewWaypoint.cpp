#include "DataViewWaypoint.h"

using namespace ci;
using namespace ci::app;

DataViewWaypoint::DataViewWaypoint(boost::shared_ptr<DataNode> dn, Assets* assets) : DataViewContentBase(assets)
{
	std::vector<DataNodeWaypointItem*>* items = dn->getWaypointItems(); 

	//mGuide->setPosition(ci::Vec3f(100.0f, 125.0f,0));
	holderSnapY = 0; 

	image = boost::shared_ptr<DataImage>(new DataImage(assets->getTextureByID( "data_waypoint_item" ), assets)); 
	image->setPosition(ci::Vec3f(350.0f,0,0));
	image->setImageAdditiveBlending(false); 
	image->showBG(false); 

	holder = DisplayObjectContainerRef( new DisplayObjectContainer() ); 
	holder->setPosition(ci::Vec3f(0,125.0f,0));

	title = boost::shared_ptr<TextViewMultiline>( new TextViewMultiline(assets, "this is title", ci::Vec2i(300,400), assets->getFontByID("data_waypoint_head"), ci::ColorA(1.0f,1.0f,1.0f,1.0f) ) ); 
	title->setPosition(ci::Vec3f(100.0f, 125.0f,0));
	title->project3D(true); 
	addChild(title); 

	//TextViewMultiline(Assets* assets, std::string text, ci::Vec2i maskSize, ci::Font* font, ci::ColorA color)
	body = boost::shared_ptr<TextViewMultiline>( new TextViewMultiline(assets, "this is body", ci::Vec2i(300,400), assets->getFontByID("data_waypoint_body"), ci::ColorA(1.0f,1.0f,1.0f,1.0f) ) ); 
	body->setPosition(ci::Vec3f(100.0f, 125.0f + 30.0f,0));
	body->project3D(true); 
	addChild(body); 

	int size = items->size(); 
	for( int i=0; i<size; i++){
		DataViewListIcon::IconType type = DataViewListIcon::IconType::FIGURE;

		if(items->at(i)->type == "helmet"){
			type = DataViewListIcon::IconType::HELMET;

		}else if(items->at(i)->type == "clothes"){
			type = DataViewListIcon::IconType::CLOTHES;

		}else if(items->at(i)->type == "book"){
			type = DataViewListIcon::IconType::BOOK;

		}else if(items->at(i)->type == "figure"){
			type = DataViewListIcon::IconType::FIGURE;
		}

		IconRef icon = IconRef( new DataViewListIcon(assets, type ) );
		icons.push_back( icon );
		
		//(std::string title, std::string subtitle, ci::gl::Texture* image;
		infoList[icon] = new WaypointInfo( items->at(i)->title, items->at(i)->body, assets->getTextureByID(items->at(i)->assetID) ); 
	}
	
	buy = ImageViewRef( new ImageView(assets->getTextureByID("data_waypoint_buy") ) ); 
	buy->setAdditiveBlending(true);
	buy->setCenter(ci::Vec3f(0.5f,0.5f,0.5f));
	buy->setPosition(ci::Vec3f( 960.0f, 240.0f, 0));
	buy->setAlpha(0.75f); 
	this->addChild(buy);

	int num_of_col = 1;
	int num_of_row = icons.size();
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	for( int i=0; i<num_of_row; i++){
		selectionGrid[0][i] = icons[i];
		icons[i]->setPosition(ci::Vec3f(0, (float)i * 100.0f, 0 ));
	}

	if(num_of_row > 4){
		arrow = ImageViewRef( new ImageView(assets->getTextureByID("data_item_arrow") ) ); 
		arrow->setAdditiveBlending(true);
		arrow->setCenter(ci::Vec3f(0.5f,0.5f,0.5f));
		arrow->setPosition(ci::Vec3f( 32.0f, 540.0f, 0));
		this->addChild(arrow);
	}

	addChild(image);
	addChild(holder);
	
	useSelection = true; 
	moveSelection(ci::Vec2i(0, 0));
}

void DataViewWaypoint::onViewOpen()
{
	show(false);
}

void DataViewWaypoint::onViewClose()
{
	for( unsigned int i=0; i<icons.size(); i++){
		timeline().apply(icons[i]->getPosition(), ci::Vec3f(0, (float)i * 100.0f, -300.0f ), 0.4f, EaseInQuint()).delay((float)i/20.0f);
		timeline().apply(icons[i]->getAlpha(), 0.0f, 0.4f, EaseInQuint()).delay((float)i/20.0f);
	}

	timeline().apply(image->getPosition(), ci::Vec3f(350.0f,0,-300.0f), 0.4f, EaseInQuint());
	timeline().apply(image->getAlpha(), 0.0f, 0.5f, EaseInQuint());

	timeline().apply(title->getPosition(), ci::Vec3f(350.0f,0,-300.0f), 0.4f, EaseInQuint()).delay(0.1f);
	timeline().apply(title->getAlpha(), 0.0f, 0.2f, EaseInQuint());

	timeline().apply(body->getPosition(), ci::Vec3f(350.0f,0,-300.0f), 0.4f, EaseInQuint()).delay(0.15f);
	timeline().apply(body->getAlpha(), 0.0f, 0.2f, EaseInQuint());

	timeline().apply(buy->getPosition(), ci::Vec3f( 960.0f, 240.0f, -300.0f), 0.4f, EaseInQuint()).delay(0.2f);
	timeline().apply(buy->getAlpha(), 0.0f, 0.2f, EaseInQuint());
}

void DataViewWaypoint::show(bool snap)
{
	float t =0.4f;
	float m = 1.0f; 
	if(snap) m=0.01f; 

	for( unsigned int i=0; i<icons.size(); i++){
		icons[i]->setPosition(ci::Vec3f(0, (float)i * 100.0f, 30.0f ));
		timeline().apply(icons[i]->getPosition(), ci::Vec3f(0, (float)i * 100.0f, 0 ), t*m, EaseOutQuint()).delay((float)i/20.0f*m);
		timeline().apply(icons[i]->getAlpha(), 1.0f, t*m, EaseOutQuint()).delay((float)i/20.0f*m);
	}

	image->setAlpha(0);
	image->setPosition(ci::Vec3f(350.0f,0,-30.0f));
	timeline().apply(image->getPosition(), ci::Vec3f(350.0f,0,0), t, EaseOutQuint());
	timeline().apply(image->getAlpha(), 1.0f, t, EaseOutQuint());

	title->setAlpha(0);
	title->setPosition(ci::Vec3f(100.0f, 125.0f,30.0f));
	timeline().apply(title->getPosition(), ci::Vec3f(100.0f, 125.0f,0), t, EaseOutQuint()).delay(0.1f);
	timeline().apply(title->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.1f);

	body->setAlpha(0);
	body->setPosition(ci::Vec3f(100.0f, 125.0f + 30.0f,30.0f));
	timeline().apply(body->getPosition(), ci::Vec3f(100.0f, 125.0f + 30.0f,0), t, EaseOutQuint()).delay(0.15f);
	timeline().apply(body->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.15f);
	
	buy->setAlpha(0);
	buy->setPosition(ci::Vec3f( 960.0f, 240.0f, 30.0f));
	timeline().apply(buy->getPosition(), ci::Vec3f( 960.0f, 240.0f, 0), t, EaseOutQuint()).delay(0.2f);
	timeline().apply(buy->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.2f);

	//
}

void DataViewWaypoint::hide(bool snap)
{
	float m = 1.0f; 
	if(snap) m=0; 

	for( unsigned int i=0; i<icons.size(); i++){
		timeline().apply(icons[i]->getPosition(), ci::Vec3f(0, (float)i * 100.0f, -30.0f ), 0.1f*m, EaseInQuint()).delay((float)i/20.0f*m);
		timeline().apply(icons[i]->getAlpha(), 0.0f, 0.1f*m, EaseInQuint()).delay((float)i/20.0f*m);
	}

	timeline().apply(image->getPosition(), ci::Vec3f(350.0f,0,30.0f), 0.2f, EaseInQuint()).delay(0.2f);
	timeline().apply(image->getAlpha(), 0.0f, 0.2f, EaseInQuint());

	timeline().apply(title->getPosition(), ci::Vec3f(350.0f,0,30.0f), 0.2f, EaseInQuint()).delay(0.2f);
	timeline().apply(title->getAlpha(), 0.0f, 0.2f, EaseInQuint());

	timeline().apply(body->getPosition(), ci::Vec3f(350.0f,0,30.0f), 0.2f, EaseInQuint()).delay(0.2f);
	timeline().apply(body->getAlpha(), 0.0f, 0.2f, EaseInQuint());

	timeline().apply(buy->getPosition(), ci::Vec3f( 960.0f, 240.0f, 30.0f), 0.2f, EaseInQuint()).delay(0.2f);
	timeline().apply(buy->getAlpha(), 0.0f, 0.2f, EaseInQuint());
}

void DataViewWaypoint::moveSelection(ci::Vec2i dir)
{
	ButtonViewRef prevContent = selectedObject;

	InputView::moveSelection(dir);

	if(selectedObject){

		if(prevContent != selectedObject){
			image->changeImage( infoList[selectedObject]->image ); 
			title->setText(infoList[selectedObject]->title); 
			body->setText(infoList[selectedObject]->subtitle); 
			body->setPosition( ci::Vec3f(100.0f, 125.0f + title->getSize()->value().y + 20.0f,0) ); 
		}

		bool move = false; 
		int i;
		int size = icons.size();
		float itemY; 

		float topY; 
		float selectedY = selectedObject->getPosition()->value().y;
		
		if( selectedY + holderSnapY < 0 ){
			//item is above selection
			topY = selectedY;
			move = true; 
		}else if( selectedY + holderSnapY >= 300.0f ){
			topY = selectedY - 300.0f; 
			move = true; 
		}else{
			//inside the box somewhere...
			for( i=0; i<size; i++){
				itemY = icons[i]->getPosition()->value().y;
				if( itemY + holderSnapY == 0 ) topY = itemY; 
			}
			//ci::app::console() << "DataViewWaypoint::moveSelection topY " << topY <<std::endl;
		}

		holderSnapY = 0 - topY;
		timeline().apply(holder->getPosition(), ci::Vec3f(0, 125.0f - topY, 0), 0.3f, EaseOutQuint());

		for( i=0; i<size; i++){
			if( holder->contains(icons[i]) ){
				holder->removeChild(icons[i]);
				icons[i]->selectView(false);
			}
		}
		
		for( i=0; i<size; i++){
			itemY = icons[i]->getPosition()->value().y;
			
			if( itemY >= topY && itemY <= topY + 300.0f ){
				holder->addChild(icons[i]);
			}
		}

		selectedObject->selectView(true);
	}
}

void DataViewWaypoint::handleInput(INPUT_TYPE type)
{

	switch(type)
	{
	case DPAD_DOWN:
	case DOWN:
		moveSelection(ci::Vec2i(0,1));
		break;
	case DPAD_UP:
	case UP:
		moveSelection(ci::Vec2i(0,-1));
		break;
        default:
            break;
	}

}

DataViewWaypoint::~DataViewWaypoint(void)
{
}
