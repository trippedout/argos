#include "BugElement.h"

using namespace ci;
using namespace ci::app;

BugElement::BugElement(ci::gl::Texture* tex, ci::gl::Texture* glowtex) : DisplayObjectContainer()
{
	baseImage = ImageViewRef(new ImageView(tex)); 
	glowImage = ImageViewRef(new ImageView(glowtex)); 

	baseImage->setCenter(ci::Vec3f(0.5f, 0.5f, 0)); 
	baseImage->setAdditiveBlending(true); 
	baseImage->setAlpha(0);

	glowImage->setCenter(ci::Vec3f(0.5f, 0.5f, 0)); 
	glowImage->setAdditiveBlending(true); 
	glowImage->setAlpha(0);

	addChild(baseImage); 
	addChild(glowImage); 
}

void BugElement::setGlowColor(ci::Vec3f color)
{
	glowImage->setColor(color);
}

void BugElement::fancyPulse(float delay)
{
	baseImage->setAlpha(0.0f);
	glowImage->setAlpha(0.0f);

	timeline().apply( baseImage->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).delay(1.0f);
}

void BugElement::fancyGlowPulse(float delay)
{
	baseImage->setAlpha(0.0f);
	glowImage->setAlpha(0.0f);

	timeline().appendTo( glowImage->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).delay(3.0f);

	timeline().appendTo( baseImage->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).delay(3.0f);
}

void BugElement::fancyGlowPulseLoop(float delay)
{
	baseImage->setAlpha(0.0f);
	glowImage->setAlpha(0.0f);

	timeline().apply( baseImage->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );

	timeline().apply( glowImage->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).finishFn(std::bind(&BugElement::fancyGlowPulseLoop2, this));
}

void BugElement::fancyGlowPulseLoop2()
{
	timeline().apply( baseImage->getAlpha(), 1.0f, 0.5f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );

	timeline().apply( glowImage->getAlpha(), 1.0f, 0.5f, EaseOutQuint() );
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).finishFn(std::bind(&BugElement::fancyGlowPulseLoop2, this));
}

void BugElement::selectGlowLoop(float delay)
{
	baseImage->setAlpha(0.0f);
	glowImage->setAlpha(0.0f);

	timeline().apply( baseImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );

	timeline().apply( glowImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(delay + 2.0f);
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).finishFn(std::bind(&BugElement::selectGlowLoop2, this));
}

void BugElement::selectGlowLoop2()
{
	timeline().apply( baseImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );

	timeline().apply( glowImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(2.0f);
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).finishFn(std::bind(&BugElement::selectGlowLoop2, this));
}


void BugElement::fancyGlowPulse3X(float delay)
{	
	baseImage->setAlpha(0.0f);
	glowImage->setAlpha(0.0f);

	timeline().apply( baseImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ); // 1
	timeline().appendTo( baseImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ); // 2
	timeline().appendTo( baseImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( baseImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ); // 3

	timeline().apply( glowImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() ).delay(delay);
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ); // 1
	timeline().appendTo( glowImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ); // 2
	timeline().appendTo( glowImage->getAlpha(), 1.0f, 1.0f, EaseOutQuint() );
	timeline().appendTo( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ); // 3
}


void BugElement::pulse()
{
	baseImage->setAlpha(1.0f);
	glowImage->setAlpha(1.0f);
	timeline().apply( glowImage->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
	timeline().apply( baseImage->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
}


void BugElement::select()
{
	glowImage->setAlpha(1.0f);
	timeline().apply( glowImage->getAlpha(), 0.5f, 1.0f, EaseOutQuint() );
	timeline().apply( baseImage->getAlpha(), 1.0f, 0.4f, EaseOutQuint() );
}

void BugElement::glow()
{
	timeline().apply( glowImage->getAlpha(), 1.0f, 0.3f, EaseOutQuint() );
	timeline().apply( baseImage->getAlpha(), 1.0f, 0.4f, EaseOutQuint() );
}

void BugElement::hide()
{
	timeline().apply( glowImage->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
	timeline().apply( baseImage->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
}

void BugElement::show()
{
	timeline().apply( glowImage->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
	timeline().apply( baseImage->getAlpha(), 1.0f, 0.3f, EaseOutQuint() );
}

BugElement::~BugElement(void)
{
}
