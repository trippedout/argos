#pragma once

#include <boost/shared_ptr.hpp>

//using namespace std; 

typedef boost::shared_ptr<class BeaconData> BeaconDataRef;

class BeaconData
{
protected: 

public:
	bool isNew;
	std::string assetID, message, userName; 
	BeaconData(std::string ass, std::string mess, std::string user);
	~BeaconData(void);
};

