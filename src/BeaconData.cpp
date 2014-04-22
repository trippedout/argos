#include "BeaconData.h"

using namespace std;

BeaconData::BeaconData(std::string ass, std::string mess, std::string user) :
	assetID(ass),
	message(mess),
	userName(user),
	isNew(true)
{
	
}

BeaconData::~BeaconData(void)
{

}
