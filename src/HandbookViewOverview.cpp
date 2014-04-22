#include "HandbookViewOverview.h"

using namespace ci;
using namespace ci::app;

HandbookViewOverview::HandbookViewOverview(DataNodeRef dn, Assets* assets):
	DataViewOverview(dn, assets)
{
	mImage->setPosition(ci::Vec3f( 944.0f, -26.0f - 60.0f, 0 ));
}

HandbookViewOverview::~HandbookViewOverview(void)
{
}

void HandbookViewOverview::onViewOpen()
{
	float t = 0.4f;

	mImage->setAlpha(0);
	mImage->setPosition(ci::Vec3f(944.0f, -26.0f + 50.0f - 60.0f,0));
	timeline().apply(mImage->getPosition(), ci::Vec3f(944.0f, -26.0f - 60.0f,0), t*2.0f, EaseOutQuint()).delay(0.1f);
	timeline().apply(mImage->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.1f);
	
	mText->setAlpha(0);
	mText->setPosition(ci::Vec3f(0,85,100.0f));
	timeline().apply(mText->getPosition(), ci::Vec3f(0,85,0), t, EaseOutQuint()).delay(0.05f);
	timeline().apply(mText->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.05f);
}

void HandbookViewOverview::onViewClose()
{
	float t= 0.1f;

	timeline().apply(mImage->getPosition(), ci::Vec3f(944.0f, -26.0f-50.0f - 60.0f,0), t, EaseOutQuint()).delay(0.1f);
	timeline().apply(mImage->getAlpha(), 0.0f, t, EaseOutQuint()).delay(0.1f);
	
	timeline().apply(mText->getPosition(), ci::Vec3f(0,85,-100.0f), t, EaseOutQuint()).delay(0.05f);
	timeline().apply(mText->getAlpha(), 0.0f, t, EaseOutQuint()).delay(0.05f);
}

void HandbookViewOverview::show(bool snap)
{
	float t= 0.2f;

	mImage->setAlpha(0);
	mImage->setPosition(ci::Vec3f(944.0f, -26.0f+50.0f - 60.0f, 0));
	timeline().apply(mImage->getPosition(), ci::Vec3f(944.0f, -26.0f - 60.0f, 0), t*2.0f, EaseOutQuint()).delay(0.1f);
	timeline().apply(mImage->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.1f);
	
	mText->setAlpha(0);
	mText->setPosition(ci::Vec3f(0,85,30.0f));
	timeline().apply(mText->getPosition(), ci::Vec3f(0,85,0), t, EaseOutQuint()).delay(0.05f);
	timeline().apply(mText->getAlpha(), 1.0f, t, EaseOutQuint()).delay(0.05f);
}
