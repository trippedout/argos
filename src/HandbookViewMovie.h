#pragma once
#include "DataViewMovie.h"
class HandbookViewMovie :
	public DataViewMovie
{
public:
	HandbookViewMovie(DataNodeRef dn, std::string assetID, std::string assetPath, Assets* assets);
	~HandbookViewMovie(void);
};

