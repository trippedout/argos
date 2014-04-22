#include "LocationNode.h"

using namespace std;

LocationNode::LocationNode(LocationInfo* orb, LocationInfo* aer, LocationInfo* loc)
{
	orbital = orb; 
	aerial = aer; 
	local = loc; 
	//location ids can be orbital, aerial, local, or local_someshityouwant
	locationID = "orbital";
}

LocationNode::LocationNode(LocationInfo* orb, LocationInfo* aer)
{
	orbital = orb; 
	aerial = aer; 
	local = NULL;
	//location ids can be orbital, aerial, local, or local_someshityouwant
	locationID = "orbital";
}

LocationNode::LocationNode(LocationInfo* orb)
{
	orbital = orb; 
	aerial = NULL;
	local = NULL;
	//location ids can be orbital, aerial, local, or local_someshityouwant
	locationID = "orbital";
}

std::string LocationNode::getCurrentLocationID()
{
	return locationID;
}

void LocationNode::setCurrentLocationID(std::string val)
{
	locationID = val; 
}

bool LocationNode::isLocked()
{
	bool locked = true; 
	
	int size = viewedLocationIDs.size(); 
		
	for(int i=0; i<size; i++){
		if(viewedLocationIDs[i] == locationID){
			// current ID has already been viewed. 
			// shit has already been unlocked. 
			locked = false; 
		}
	}

	return locked;
}

void LocationNode::setLocked(bool val)
{
	viewedLocationIDs.clear(); 

	if(!val){
		// its unlocked, add the current locationID to the list of shit thats been seen 
		//   orbital, aerial, local
		//	 local_medical_bay, local_dorm, local_mess_hall, local_tether
		//   local_mehaffeys_office, local_outer_wall, local_locker_room

		if(orbital) viewedLocationIDs.push_back("orbital");
		if(aerial) viewedLocationIDs.push_back("aerial");
		if(local){
			viewedLocationIDs.push_back("local");
			viewedLocationIDs.push_back("local_medical_bay");
			viewedLocationIDs.push_back("local_dorm");
			viewedLocationIDs.push_back("local_mess_hall");
			viewedLocationIDs.push_back("local_tether");
			viewedLocationIDs.push_back("local_mehaffeys_office");
			viewedLocationIDs.push_back("local_outer_wall");
			viewedLocationIDs.push_back("local_locker_room");
		}
	}
}

LocationNode::~LocationNode(void)
{
}
