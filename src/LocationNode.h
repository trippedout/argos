#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

//using namespace std;

typedef boost::shared_ptr<class LocationNode> LocationNodeRef;

class LocationNode
{
public:
	struct LocationInfo 
	{
		std::string head;
		std::string subhead;
		std::string body; 
		std::string assetID; 

		LocationInfo(std::string assetID, std::string head, std::string subhead, std::string body) 
			: assetID(assetID), head(head), subhead(subhead), body(body) {}
	};

protected:
	std::string locationID;
	std::vector<std::string> viewedLocationIDs;

	LocationInfo* orbital;
	LocationInfo* aerial;
	LocationInfo* local;

public:

	LocationNode(LocationInfo* orbital, LocationInfo* aerial, LocationInfo* local);
	LocationNode(LocationInfo* orbital, LocationInfo* aerial);
	LocationNode(LocationInfo* orbital);
	~LocationNode(void);

	bool isLocked(); 
	void setLocked(bool val); 

	LocationInfo* getOrbital() { return orbital; }; 
	LocationInfo* getAerial() { return aerial; }; 
	LocationInfo* getLocal() { return local; }; 

	std::string getCurrentLocationID();
	void setCurrentLocationID(std::string val); 

};

