#pragma once
#include "DataViewDocument.h"
class HandbookViewDocument :
	public DataViewDocument
{
public:
	HandbookViewDocument(DataNodeRef dn, std::string assetID, std::string assetPath, Assets* assets);
	~HandbookViewDocument(void);
};

