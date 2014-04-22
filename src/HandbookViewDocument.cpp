#include "HandbookViewDocument.h"


HandbookViewDocument::HandbookViewDocument(DataNodeRef dn, std::string assetID, std::string assetPath, Assets* assets):
	DataViewDocument(dn, assetID, assetPath, assets)
{
	thumbnail->setCenter(ci::Vec3f(0.5f, 0.5f, 0)); 
	thumbnail->setPosition(ci::Vec3f( 1200.0f, 330.0f - 100.0f - 60.0f, 0.0f )); 
	
	bg->setCenter(ci::Vec3f(0.5f, 0.5f, 0)); 
	bg->setPosition(ci::Vec3f( 1200.0f, 330.0f - 100.0f, 0.0f )); 
}


HandbookViewDocument::~HandbookViewDocument(void)
{
}
