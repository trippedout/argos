#include "HandbookViewWaypoint.h"

using namespace ci;
using namespace ci::app;

HandbookViewWaypoint::HandbookViewWaypoint(DataNodeRef dn, Assets* assets):
	DataViewWaypoint(dn, assets)
{
	image->setPosition( ci::Vec3f( 944.0f, -126.0f, 0) );
	buy->setPosition( ci::Vec3f( 944.0f + 236, 400.0f - 20.0f, 0) );
	buy->setCenter( ci::Vec3f(0.5f, 0.5f, 0) ); 
}

HandbookViewWaypoint::~HandbookViewWaypoint(void)
{
}


void HandbookViewWaypoint::onViewClose()
{
	float t = 0.1f;

	for( unsigned int i=0; i<icons.size(); i++){
		timeline().apply(icons[i]->getPosition(), ci::Vec3f(0, (float)i * 100.0f, -100.0f ), 0.3f, EaseOutQuint()).delay((float)i/20.0f);
		timeline().apply(icons[i]->getAlpha(), 0.0f, 0.3f, EaseOutQuint()).delay((float)i/20.0f);
	}

	timeline().apply(image->getPosition(), ci::Vec3f(944.0f, -126.0f -50.0f ,0), 0.3f, EaseOutQuint());
	timeline().apply(image->getAlpha(), 0.0f, 0.5f, EaseOutQuint());

	timeline().apply(title->getPosition(), ci::Vec3f(350.0f,0,-100.0f), 0.3f, EaseOutQuint()).delay(0.1f);
	timeline().apply(title->getAlpha(), 0.0f, 0.3f, EaseOutQuint());

	timeline().apply(body->getPosition(), ci::Vec3f(350.0f,0,-100.0f), 0.3f, EaseOutQuint()).delay(0.15f);
	timeline().apply(body->getAlpha(), 0.0f, 0.3f, EaseOutQuint());

	timeline().apply(buy->getPosition(), ci::Vec3f( 960.0f + 236, 400.0f - 20.0f, -100.0f), 0.3f, EaseOutQuint()).delay(0.2f);
	timeline().apply(buy->getAlpha(), 0.0f, 0.3f, EaseOutQuint());
}

void HandbookViewWaypoint::show(bool snap)
{
	float t = 0.4f;
	float m = 1.0f; 
	if(snap) m=0; 

	for( unsigned int i=0; i<icons.size(); i++){
		icons[i]->setPosition(ci::Vec3f(0, (float)i * 100.0f, 30.0f ));
		timeline().apply(icons[i]->getPosition(), ci::Vec3f(0, (float)i * 100.0f, 0 ), t*m, EaseOutQuint()).delay((float)i/20.0f*m);
		timeline().apply(icons[i]->getAlpha(), 1.0f, t*m, EaseOutQuint()).delay((float)i/20.0f*m);
	}

	image->setAlpha(0);
	image->setPosition(ci::Vec3f(944.0f, -126.0f + 50.0f,0));
	timeline().apply(image->getPosition(), ci::Vec3f(944.0f, -126.0f,0), t, EaseOutQuint());
	timeline().apply(image->getAlpha(), 1.0f, t, EaseOutQuint());

	title->setAlpha(0);
	title->setPosition(ci::Vec3f(100.0f, 125.0f,100.0f));
	timeline().apply(title->getPosition(), ci::Vec3f(100.0f, 125.0f,0), t, EaseOutQuint()).delay(0.1f);
	timeline().apply(title->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.1f);

	body->setAlpha(0);
	body->setPosition(ci::Vec3f(100.0f, 125.0f + 30.0f,100.0f));
	timeline().apply(body->getPosition(), ci::Vec3f(100.0f, 125.0f + 30.0f,0), t, EaseOutQuint()).delay(0.15f);
	timeline().apply(body->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.15f);
	
	buy->setAlpha(0);
	buy->setPosition(ci::Vec3f( 960.0f + 236, 400.0f - 20.0f, 100.0f));
	timeline().apply(buy->getPosition(), ci::Vec3f( 960.0f + 236, 400.0f - 20.0f, 0), t, EaseOutQuint()).delay(0.2f);
	timeline().apply(buy->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.2f);
}

/*
void HandbookViewWaypoint::hide(bool snap)
{
	float t = 0.4f;
	float m = 1.0f; 
	if(snap) m=0; 

	for( unsigned int i=0; i<icons.size(); i++){
		timeline().apply(icons[i]->getPosition(), ci::Vec3f(0, (float)i * 100.0f, 100.0f ), 0.1f*m, EaseOutCubic()).delay((float)i/20.0f*m);
		timeline().apply(icons[i]->getAlpha(), 0.0f, 0.1f*m, EaseOutCubic()).delay((float)i/20.0f*m);
	}

	timeline().apply(image->getPosition(), ci::Vec3f(944.0f, -26.0f,100.0f), 0.2f, EaseOutCubic()).delay(0.2f);
	timeline().apply(image->getAlpha(), 0.0f, 0.2f, EaseOutCubic());

	timeline().apply(title->getPosition(), ci::Vec3f(350.0f,0,100.0f), 0.2f, EaseOutCubic()).delay(0.2f);
	timeline().apply(title->getAlpha(), 0.0f, 0.2f, EaseOutCubic());

	timeline().apply(body->getPosition(), ci::Vec3f(350.0f,0,100.0f), 0.2f, EaseOutCubic()).delay(0.2f);
	timeline().apply(body->getAlpha(), 0.0f, 0.2f, EaseOutCubic());

	timeline().apply(buy->getPosition(), ci::Vec3f( 960.0f, 240.0f, 100.0f), 0.2f, EaseOutCubic()).delay(0.2f);
	timeline().apply(buy->getAlpha(), 0.0f, 0.2f, EaseOutCubic());
}
*/