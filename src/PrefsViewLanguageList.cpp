#include "PrefsViewLanguageList.h"

using namespace ci;
using namespace ci::app;

PrefsViewLanguageList::PrefsViewLanguageList(Assets* assets) : InputView(assets)
{	
	mCurrentLanguageBg		=	ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/prefs/language_list_faded_bg.png" ) ) ) ) );
	this->addChild(mCurrentLanguageBg);

	mCurrentLanguage		=	TextViewRef( new TextView("ENGLISH", assets->getFontByID(Assets::FileID::DEFAULT_FONT), ci::ColorA(1,1,1,1) ) );
	mCurrentLanguage->setPosition(ci::Vec3f(150,21,0));
	mCurrentLanguage->setCenter(ci::Vec3f(.5,.5,0));
	mCurrentLanguage->setFontSize(15);
	this->addChild(mCurrentLanguage);

	//set up languages
	std::string languages[mNumLanguages] = {
		"ENGLISH",
		"GERMAN",
		"ITALIAN",
		"KOREAN",
		"PORTUGUESE",
		"FRENCH",
		"SPANISH",
		"JAPANESE",
		"THAI",
		"ARABIC",
		"CANTONESE",
		"RUSSIAN",
		"GREEK"
	};

	//create language buttons and add to selection grid
	ButtonViewRef btn = NULL;
	
	selectionGrid.resize( 1 , std::vector<ButtonViewRef>( mNumLanguages , NULL ) );

	for( int i = 0; i < mNumLanguages; i++ )
	{
		btn = ButtonViewRef( new ARGOSButtonView(assets, languages[i]) );
		btn->setPosition(ci::Vec3f(300.0f, i*42.0f, 0.0f));
		boost::dynamic_pointer_cast<ARGOSButtonView>(btn)->getTextView()->setFontSize(15);
		this->addChild(btn);

		selectionGrid[0][i] = btn;
		mLanguages.push_back( btn );
	}

	this->deselectAll();
	this->useSelection = true;
	moveSelection(ci::Vec2i(0,0));
	
	//start
	hide();
}

PrefsViewLanguageList::~PrefsViewLanguageList(void)
{

}

void PrefsViewLanguageList::show()
{
	timeline().apply(mCurrentLanguageBg->getAlpha(), 1.0f, 0.3f);
	timeline().apply(mCurrentLanguage->getAlpha(), 1.0f, 0.3f);
	setButtonAlphas(1.0f);

	this->showing = true;
}

void PrefsViewLanguageList::hide()
{
	timeline().apply(mCurrentLanguageBg->getAlpha(), 0.0f, 0.3f);
	timeline().apply(mCurrentLanguage->getAlpha(), 0.0f, 0.3f);
	setButtonAlphas(0.0f);

	this->showing = false;
}

void PrefsViewLanguageList::setLanguage()
{
	SoundController::getInstance().playSound(SoundController::SELECT);

	boost::shared_ptr<ARGOSButtonView> ptr = boost::dynamic_pointer_cast<ARGOSButtonView>(selectedObject);
	mCurrentLanguage->setText( ptr->getTextView()->getText() );

	ci::app::console() << "PrefsViewLanguageList::setLanguage()" << ptr->getTextView()->getText() << std::endl;
}

void PrefsViewLanguageList::setActive(bool active)
{
	this->useSelection = active;

	if(active)
		this->setAlpha(1.0f);
	else
		this->setAlpha(0.5f);
}

void PrefsViewLanguageList::setButtonAlphas(float alpha)
{
	for(int i = 0; i < mNumLanguages; i++)
	{
		timeline().apply(mLanguages[i]->getAlpha(), alpha, 0.3f).delay(i*0.05f);
	}
}

bool PrefsViewLanguageList::isShowing()
{
	return this->showing;
}

void PrefsViewLanguageList::handleInput(INPUT_TYPE type)
{

}