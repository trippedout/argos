#include "HandbookViewMovie.h"


HandbookViewMovie::HandbookViewMovie(DataNodeRef dn, std::string assetID, std::string assetPath, Assets* assets):
	DataViewMovie(dn, assetID, assetPath, assets)
{
	thumbnail->setCenter(ci::Vec3f(0.5f, 0.5f, 0));
	thumbnail->setPosition(ci::Vec3f(1200.0f,330.0f - 100.0f - 60.0f,0.0f)); 
	
	thumbBG->setCenter(ci::Vec3f(0.5f, 0.5f, 0));
	thumbBG->setPosition(ci::Vec3f(1200.0f,330.0f - 100.0f - 60.0f,0.0f)); 
	
}


HandbookViewMovie::~HandbookViewMovie(void)
{
}
