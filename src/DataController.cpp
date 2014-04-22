#include "DataController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

DataController::DataController(JsonTree tree) :
	mEngageComplete(false),
	mEpisodeComplete(false),
	mGamePulsed(false),
	currentScene(0),
	handbookLocation(NULL),
	pauseScene(NULL)
{
	/*
	for( JsonTree::ConstIter cIt = tree.begin(); cIt != tree.end(); ++cIt )
	{
		//std::string data = (*cIt)["data_nodes"].getValue();
		ci::app::console() << "theres a thing" << std::endl;
	}
	*/
	addGhettoData();
}

void DataController::unlockAllContent()
{
	//mEpisodeComplete = true;
	mEngageComplete = true;
	mGamePulsed = true; 

	for(unsigned int i=0; i<data.size(); i++){
		data[i]->setLocked(false);
		if(data[i]->isOni()) data[i]->setOniLocked(false);
	}

	for(unsigned int i=0; i<locations.size(); i++){
		locations[i]->setLocked(false);
	}

	for(unsigned int i=0; i<scenes.size(); i++){
		std::vector<BeaconDataRef>* beacons = scenes[i]->getBeacons();
		for(unsigned int j=0; j<beacons->size(); j++){
			beacons->at(j)->isNew = false; 
		}
	}
}

void DataController::lockAllContent()
{
	mEpisodeComplete = false;
	mEngageComplete = false;
	mGamePulsed = false;

	for(unsigned int i=0; i<data.size(); i++){
		data[i]->setLocked(true);
		data[i]->setOniLocked(true);
	}

	for(unsigned int i=0; i<locations.size(); i++){
		locations[i]->setLocked(true);
	}

	for(unsigned int i=0; i<scenes.size(); i++){
		std::vector<BeaconDataRef>* beacons = scenes[i]->getBeacons();
		for(unsigned int j=0; j<beacons->size(); j++){
			beacons->at(j)->isNew = true; 
		}
	}
}

SceneRef  DataController::getCurrentScene(float time)
{
	int size = scenes.size(); 
	
	for( int i=0; i<size-1; i++){
		if(scenes[i+1]->getStart() > time){
			currentScene = i; 
			return scenes[currentScene];
		}
	}

	currentScene = size-1; 
	return scenes[currentScene];
}

std::vector< HBCategoryRef >* DataController::getHBItems()
{
	return &hbItems; 
}

std::vector< HBCategoryRef >* DataController::getHBCharacters()
{
	return &hbCharacters; 
}

std::vector< HBCategoryRef >* DataController::getHBOniFiles()
{
	return &hbOniFiles; 
}

std::vector< DataNodeRef >* DataController::getCurrentSceneDataNodes(float time)
{
	return getCurrentScene(time)->getCurrentDataNodes(time);
}

LocationNodeRef DataController::getCurrentSceneLocation(float time)
{
	return getCurrentScene(time)->getCurrentLocation(time);
}

std::vector< SceneRef >* DataController::getScenes()
{
	return &scenes;
}

std::vector< DataNodeRef >* DataController::getDataNodes()
{
	return &data;
}

std::vector< LocationNodeRef >* DataController::getLocationNodes()
{
	return &locations;
}

DataController::UnlockedInfo DataController::getUnlocked()
{
	// total onis +1 as game counts as an oni symbol. 
	int totalOnis = 1; 
	int totalItems = 0; 
	int totalCharacters = 0; 
	int totalLocations = 0; 

	int onisUnlocked = 0; 
	int itemsUnlocked = 0; 
	int charactersUnlocked = 0; 
	int locationsUnlocked = 0; 
	
	//last oni symbol is from the game
	if(mEngageComplete){
		onisUnlocked++;
	}

	//check for oni symbols. 
	for(unsigned int i=0; i<data.size(); i++){
		if( data[i]->isOni() ){
			totalOnis++;
			if( !data[i]->isOniLocked() ){
				onisUnlocked++;
			}
		}
	}

	//characters
	std::vector<HBCategoryRef>* chars = getHBCharacters(); 
	for(unsigned int i=0; i<chars->size(); i++){
		std::vector< DataNodeRef >* nodes = chars->at(i)->getNodes();
		for(unsigned int j=0; j<nodes->size(); j++){
			totalCharacters++;
			if( !nodes->at(j)->isLocked() ){
				charactersUnlocked++;
			}
		}
	}

	//items
	std::vector<HBCategoryRef>* items = getHBItems(); 
	for(unsigned int i=0; i<items->size(); i++){
		std::vector< DataNodeRef >* nodes = items->at(i)->getNodes();
		for(unsigned int j=0; j<nodes->size(); j++){
			totalItems++;
			if( !nodes->at(j)->isLocked() ){
				itemsUnlocked++;
			}
		}
	}


	for(unsigned int i=0; i<locations.size(); i++){
		totalLocations++;
		if( !locations[i]->isLocked() ){
			locationsUnlocked++;
		}
	}

	if(onisUnlocked > 4) onisUnlocked = 4; 
	if(totalOnis > 4) totalOnis = 4; 

	//UnlockedInfo(int items, int characters, int locations, int onis) 
	return DataController::UnlockedInfo( itemsUnlocked, charactersUnlocked, locationsUnlocked, onisUnlocked, totalItems,  totalCharacters, totalLocations, totalOnis );

}

void DataController::setEngageComplete(bool complete) 
{ 
	mEngageComplete = complete; 
	
	if(mEngageComplete){
		std::vector< HBCategoryRef >*  cats = getHBOniFiles();
		for( unsigned int i=0; i< cats->size(); i++){
			std::vector< DataNodeRef >* nodes = cats->at(i)->getNodes(); 
			
			for( unsigned int j=0; j< nodes->size(); j++){
				nodes->at(j)->setLocked(false);
			}
		}
	}
}


void DataController::addGhettoData()
{
	
	float offset = -2.0f; 

//------------------------------------------
// SCENES
//------------------------------------------

	scenes.push_back( SceneRef( new Scene(0)) );
	scenes.push_back( SceneRef( new Scene(1.0f*60.0f + 5.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(2.0f*60.0f + 39.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(4.0f*60.0f + 53.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(4.0f*60.0f + 58.0f + offset )) );
	scenes.push_back( SceneRef( new Scene(5.0f*60.0f + 38.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(7.0f*60.0f + 1.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(8.0f*60.0f + 41.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(8.0f*60.0f + 54.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(9.0f*60.0f + 0 + offset)) );
	scenes.push_back( SceneRef( new Scene(10.0f*60.0f + 51.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(13.0f*60.0f + 45.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(14.0f*60.0f + 52.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(15.0f*60.0f + 57.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(17.0f*60.0f + 27.0f + offset)) );
	scenes.push_back( SceneRef( new Scene(18.0f*60.0f + 2.0f + offset)) );
		
//------------------------------------------
// LOCATIONS
//------------------------------------------

	//LocationInfo(std::string assetID, std::string head, std::string subhead, std::string body)

	LocationNodeRef andesia = LocationNodeRef( new LocationNode(
		new LocationNode::LocationInfo("orbital_andesia.png", "ANDESIA", "XAOLOS SYSTEM", 
		"A blue-green moon colony orbiting Morodein (the fourth planet in the \
		Xaolos system), Andesia was founded in 2428 as an Outer Colony of \
		incredible promise due to its immense equatorial rain forests and highly \
		arable land. \
		\n\n\
		After establishing its capital city Kul'dat in 2432, Andesia became \
		a major player among Outer Colonies when it came to the harvesting \
		of a number of critical medicines, found natively in the moon\'s vast \
		forestland. In 2498, Andesia would see a sharp rise of civilian unrest,\
		fomented by the burgeoning Insurrection. Discontented citizens rioted \
		in the streets of the port city Brithma, eventually downing the \
		UNSC frigate Yurei. \
		\n\n\
		These first signs of secession quickly escalated, and by 2523 Andesia \
		declared autonomy from the UEG. When chemical compounds found in \
		terrorist weapons were traced back to several of the moon's factories, \
		the UNSC sent out a fleet of rapid response shock troops to investigate.\
		The ensuing conflict continued unabated until early 2526, when key UNSC \
		military incursions severed rebel control of Kul'dat." ) ) );

	LocationNodeRef nibiru = LocationNodeRef( new LocationNode(
		new LocationNode::LocationInfo("orbital_nibiru.png", "NIBIRU", "AM-ADAD SYSTEM", 
		"For most of its early history, Nibiru was valued mainly for the Ch\'lana,\
		a communication beacon that was constructed in its outer orbit to receive \
		and send transmissions across the Am-Adad system. The desert planet was \
		deemed inhospitable to life and remained untouched by humans for \
		centuries after its discovery. \
		\n\n\
		In 2443, the Ch\'lana\'s fuel core was destroyed during a coronal mass \
		ejection and the beacon crashed on Nibiru\'s surface. During retrieval \
		and repair, engineers discovered deep reservoirs of tritium embedded \
		beneath the planet\'s crust - a central component of UNSC ships\' \
		fusion reactors. Suddenly an invaluable asset to the UEG, mines \
		were set up across the planet and  the Ch\'lana was rebuilt. \
		\n\n\
		Insurrectionist leadership learned of the resource-rich colony and slipped \
		agents into mining operations. By 2549, terrorist forces had overtaken the \
		Ch\'lana and were using it to intercept UNSC communications. The UNSC\'s \
		efforts to retake Nibiru were marred by a sudden increase in radiation \
		throughout the Am-Adad system, and the planet was eventually abandoned." ) ) );

	LocationNodeRef circ4 = LocationNodeRef( new LocationNode(
		new LocationNode::LocationInfo("orbital_circinius.png", "CIRCINIUS IV", "CIRCINIUS SYSTEM", 
		"Held within the remote Circinius star system, the fourth world was\
		discovered as a large, oxygen-rich planet in 2398. Rather than use it as\
		a civilian colony, the CAA (Colonial Administration Authority) sought to \
		transform it into a military world similar to that of Reach, but these plans \
		were never realized. Instead, the planet served only as a CMA \
		(Colonial Military Authority) outpost until 2421, when retired military \
		personnel founded Lisdun, a cloistered, rural site that eventually grew \
		to cover much of the southern continent of Mochias. Opposite of this was \
		the northern continent of Bron, which has served as the home of the Corbulo \
		Academy of Military Science since 2455."),

		new LocationNode::LocationInfo("ASSET_ID_HERE", "CAMP NALTHAM", "CENTRAL BRON", 
		"Surrounded by mountains typical of continental Bron, Camp Naltham proved \
		to be the ideal location for Corbulo Academy and its most prominent \
		architectural feature, the Space Tether. Its temperate climate and uneven \
		terrain allow for a diverse variety of combat simulations, while also \
		being strategically protected by dense forest."),

		new LocationNode::LocationInfo("ASSET_ID_HERE", "CORBULO ACADEMY", "OF MILITARY SCIENCE", 
		"A training facility for the children of the UNSC and UEG\'s most distinguished\
		and high-ranking personnel, CAMS systematically produces the most able-bodied \
		and strategically well-versed officers in the UNSC\'s ranks. \
		\n\n\
		Students are eligible to enter CAMS after they have reached fifteen years of age. \
		They then participate in a four-year curriculum that focuses on tactical thinking, \
		field training, and an aggressive physical fitness regimen. This includes multiple \
		forms of hand-to-hand combat as well as the dreaded \"cryo-drills,\" in which \
		first-years are regularly subjected to cryo-sleep to acclimate them to the process.") ) );
	
	// andesia, nibiru, circ4
	// locationIDs: 
	//   orbital, aerial, 
	//	 local_medical_bay, local_dorm, local_mess_hall, local_tether
	//   local_mehaffeys_office, local_outer_wall, local_locker_room

	locations.push_back(circ4); 
	locations.push_back(andesia); 
	locations.push_back(nibiru); 

	scenes[0]->addLocation( circ4, 0, "local_medical_bay" ); 
	scenes[1]->addLocation( andesia, 0, "orbital" ); 
	scenes[2]->addLocation( circ4, 0, "local_dorm" ); 
	scenes[3]->addLocation( circ4, 0, "aerial" ); 
	scenes[4]->addLocation( circ4, 0, "local_mess_hall" ); 
	scenes[5]->addLocation( circ4, 0, "local_mehaffeys_office" ); 
	//scenes[6]->addLocation( circ4, 0, "local_dorm" ); 
	scenes[6]->addLocation( nibiru, 0, "orbital" ); 
	
	scenes[7]->addLocation( circ4, 0, "local_dorm" ); 
	scenes[8]->addLocation( circ4, 0, "aerial" ); 
	scenes[9]->addLocation( circ4, 0, "local_outer_wall" ); 
	scenes[10]->addLocation( circ4, 0, "aerial" ); 
	scenes[11]->addLocation( circ4, 0, "local_tether" ); 
	scenes[12]->addLocation( circ4, 0, "local_tether" ); 
	scenes[13]->addLocation( circ4, 0, "local_locker_room" ); 
	scenes[14]->addLocation( circ4, 0, "local_locker_room" ); 
	scenes[15]->addLocation( circ4, 0, "local_locker_room" ); 

//------------------------------------------
// BEACONS
//------------------------------------------

	scenes[0]->addBeacon( new BeaconData("pause_icon_social_1", "Need help here", "BOO") ); 
	scenes[0]->addBeacon( new BeaconData("pause_icon_social_2", "Look Around!", "RAMA") ); 
	scenes[1]->addBeacon( new BeaconData("pause_icon_social_3", "Wow. Did you see that?", "DIANA") ); 
	scenes[2]->addBeacon( new BeaconData("pause_icon_social_2", "I want to enroll at C.A.M.S.", "JUSTIN") ); 
	scenes[2]->addBeacon( new BeaconData("pause_icon_social_3", "Look around!", "GORAN") ); 
	scenes[3]->addBeacon( new BeaconData("pause_icon_social_4", "Look around!", "ERIC") ); 
	scenes[3]->addBeacon( new BeaconData("pause_icon_social_5", "Need help here", "ROUSHEY") ); 
	scenes[4]->addBeacon( new BeaconData("pause_icon_social_6", "This is cool!", "ANTHONY") ); 
	scenes[4]->addBeacon( new BeaconData("pause_icon_social_7", "I want to enroll at C.A.M.S.", "FRANCIS") ); 
	scenes[5]->addBeacon( new BeaconData("pause_icon_social_1", "U.N.S.C. forever!", "TOM") ); 
	scenes[5]->addBeacon( new BeaconData("pause_icon_social_2", "Need help here", "FLAVIO") ); 
	scenes[5]->addBeacon( new BeaconData("pause_icon_social_3", "Look around!", "JOON") ); 
	scenes[6]->addBeacon( new BeaconData("pause_icon_social_4", "Need help here", "CJ") ); 
	scenes[7]->addBeacon( new BeaconData("pause_icon_social_5", "I want to enroll at C.A.M.S.", "DAN") ); 
	scenes[8]->addBeacon( new BeaconData("pause_icon_social_6", "Wow. Did you see that?", "KIKI") ); 
	scenes[9]->addBeacon( new BeaconData("pause_icon_social_7", "Need help here", "TONY") ); 
	scenes[10]->addBeacon( new BeaconData("pause_icon_social_1", "This is cool!", "JJ") );
	scenes[11]->addBeacon( new BeaconData("pause_icon_social_1", "U.N.S.C. forever!", "CJ") );
	scenes[11]->addBeacon( new BeaconData("pause_icon_social_1", "I want to enroll at C.A.M.S.", "BOO") );
	scenes[12]->addBeacon( new BeaconData("pause_icon_social_1", "Wow. Did you see that?", "RAMA") );
	scenes[13]->addBeacon( new BeaconData("pause_icon_social_1", "This is cool!", "ROUSHEY") );
	scenes[13]->addBeacon( new BeaconData("pause_icon_social_1", "Need help here", "FRANCIS") );
	scenes[14]->addBeacon( new BeaconData("pause_icon_social_1", "Look around!", "ERIC") );
	scenes[15]->addBeacon( new BeaconData("pause_icon_social_1", "U.N.S.C. forever!", "ANTHONY") );


//------------------------------------------
// WAYPOINT
//------------------------------------------

	DataNodeWaypointItem* camsShirt = new DataNodeWaypointItem("DATA_Waypoint_tee-halo-cams-COMBO_Still", 
		"CAMS Cadet T-Shirt",  
		"clothes",
		"If your parents are important enough for \
		YOU to attend CAMS, make sure that you \
		make them proud. Be sure to only wear \
		this shirt in your recreational quarters.\
		\n\n\
		100% cotton premium black tee\
		\n\n\
		Price: $18");

	DataNodeWaypointItem* avatarCamsShirt = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Corbulo_Academy_T-Shirt_Green_Still", 
		"Avatar Corbulo Academy T-Shirt", 
		"helmet",
		"Get your Avatar ready for training \
		with this CAMS T-shirt.\
		\n\n\
		Price: $5");

	DataNodeWaypointItem* avatarCamsFormal = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Lasky_CAMS_Formal_Dress_Still", 
		"Avatar CAMS Formal Dress", 
		"helmet",
		"Stand at attention in CAMS Formal Dress.\
		\n\n\
		Price: $5");
	
	DataNodeWaypointItem* armoryShirt = new DataNodeWaypointItem("DATA_Waypoint_tee-halo-misriah-flat_Still", 
		"Misriah Armory T-Shirt", 
		"clothes",
		"Misriah Armory stands by their slogan:\
		\"Battle tested for your home.\" Really, \
		you should never leave home without one...\
		\n\n\
		100% cotton premium military green tee.\
		\n\n\
		Price: $18" );
	
	DataNodeWaypointItem* tubeChief = new DataNodeWaypointItem("DATA_Waypoint_toy-halo-mcf-s4cryotube-package_1_Still", 
		"Halo 4: Deluxe Master Chief \n\
		in Cryotube Action Figure", 
		"figuret",
		"The Master Chief returns \
		complete with Assault Rifle.\
		\n\n\
		From the masters at McFarlane Toys.\
		\n\n\
		Price: $36" );

	DataNodeWaypointItem* playArtsChief = new DataNodeWaypointItem("DATA_Waypoint_etc-figure-halo-sqenix-masterchief-full_Still", 
		"Play Arts Master Chief",  
		"figuret",
		"From their acclaimed Play Arts Kai \
		series is Square Enix's latest \
		installment from the blockbuster \
		franchise, Halo 4.\
		\n\n\
		Figure Size: Approx. 9.6\" Tall\
		Weight: Approx. 2 lbs.\
		Specs: Full-color PVC Action Figure\
		\n\n\
		Price: $65" );
	
	DataNodeWaypointItem* avatarOdstDropPod = new DataNodeWaypointItem("DATA_Waypoint_Avatar_ODST_Drop_Pod_Still", 
		"Avatar ODST Drop Pod Prop",  
		"helmet",
		"Load out and strap your XBox Avatar \
		into their own ODST Drop Pod!\
		\n\n\
		Price: $5" );

	DataNodeWaypointItem* odstDropPod = new DataNodeWaypointItem("DATA_Waypoint_other_droppod-rookie_packaging_01_dp_Still", 
		"Halo ODST: Drop Pod", 
		"figuret", 
		"ODST Drop Pod comes complete with \
		The Rookie action figure.\
		\n\n\
		From the masters at McFarlane Toys.\
		\n\n\
		Price: $15" );

	DataNodeWaypointItem* megaBlocksCryobay = new DataNodeWaypointItem("DATA_Waypoint_megablocks_cyrobay_Still", 
		"Mega Blocks: UNSC Cryo Bay",  
		"figuret",
		"Cryo Bay building set comes complete \
		with Master Chief figure, Cortana figure, \
		Assault Riffle, and Battle Riffle.\
		\n\n\
		Price: $25" );

	DataNodeWaypointItem* avatarArbiterMask = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Arbiter_Mask_Still", 
		"Avatar Arbiter Mask",  
		"helmet",
		"Give your Avatar the Arbiter's mask.\
		\n\n\
		Price: $5" );

	DataNodeWaypointItem* avatarElite = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Elite_Costume_Still", 
		"Avatar Elite Costume",  
		"helmet",
		"Give your Avatar the full \
		Sangheili treatment.\
		\n\n\
		Price: $5" );

	DataNodeWaypointItem* banshee = new DataNodeWaypointItem("DATA_Waypoint_toy-halo-jda-banshee-packagefront_Still", 
		"Halo 4: Covenant Banshee Vehicle",  
		"figuret",
		"Includes 1 Vehicle and 2 Characters\
		- Elite Zealot & Imperial Grunt\
		2.3\" Elite Zealot with \
		10 point articulation\
		1.7\" Imperial Grunt with \
		Fuel Rod Cannon and \
		6 point articulation\
		Opening Cockpit\
		7.1\"L x 7.2\"W x 3.6\"H\
		\n\n\
		Price: $20" );

	DataNodeWaypointItem* avatarSword = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Energy_Sword_Still", 
		"Avatar Energy Sword Prop",  
		"helmet",
		"For the stab-happy space explorer.\
		\n\n\
		Price: $5" );

	DataNodeWaypointItem* zelotToy = new DataNodeWaypointItem("DATA_Waypoint_toy-halo-mcf-s4elitezealot-package_Still", 
		"Halo 4: Elite Zealot \
		Action Figure",  
		"figuret",
		"Unparalleled in skill and unsurpassed \
		in martial prowess, Zealots locate, \
		analyze, and amass enemy intel. \
		The 6-inch figure includes an \
		Energy Sword and features 26 points \
		of articulation.\
		\n\n\
		Made by the masters at McFarlane Toys.\
		\n\n\
		Price: $15" );

	DataNodeWaypointItem* avatarSwordShirt = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Energy_Sword_Shirt_Still", 
		"Avatar Energy Sword Shirt",  
		"helmet",
		"For those who wear their \
		stab-happiness with pride.\
		\n\n\
		Price: $5" );

	DataNodeWaypointItem* chiefCostume = new DataNodeWaypointItem("DATA_Waypoint_etc-costume-halo-deluxemasterchief-full_Still", 
		"Deluxe Master Chief Costume",  
		"clothes",
		"Halo's Master Chief Petty Officer John-117, \
		commonly known as the Master Chief, is a \
		towering faceless cybernetically enhanced \
		supersoldier, never seen without his green \
		colored armor or helmet. We recommend \
		occasionally wearing something else.\
		\n\n\
		Jumpsuit with molded armor and two-piece mask\
		\n\n\
		Price: $80");

	DataNodeWaypointItem* mcFarlaneChief = new DataNodeWaypointItem("DATA_Waypoint_toy-halo-mcf-s4masterchief-package_Still", 
		"Halo 4: Master Chief \n\
		Action Figure",  
		"figuret",
		"The most popular character of the \
		Halo universe, Master Chief, returns \
		ready for action in Halo 4. The 6-inch \
		figure includes an Assault Rifle and \
		features 28 points of articulation.\
		\n\n\
		Made by the masters at McFarlane Toys.\
		\n\n\
		Price: $15" );

	DataNodeWaypointItem* imHereShirt = new DataNodeWaypointItem("DATA_Waypoint_tee-halo-imhere-flat_Still", 
		"I'm Here T-Shirt",  
		"clothes",
		"The Master Chief awakens from cryosleep\
		and explodes from his chamber ready \
		to answer Cortana's cries for help.\
		\n\n\
		100% cotton premium black tee.\
		\n\n\
		Price: $18" );

	DataNodeWaypointItem* chiefIllustrationShirt = new DataNodeWaypointItem("DATA_Waypoint_tee-halo-masterchiefillustration-flat_Still", 
		"Master Chief \n\
		Illustration T-Shirt",  
		"clothes",
		"\"And you told me you were going to wear \
		something nice! Folks need heroes, Chief; \
		they give \'em hope. So smile, would\'ya? \
		While we still got somethin\' to smile about!\"\
		\t- Sergeant Johnson\
		\n\n\
		100% cotton premium charcoal tee.\
		\n\n\
		Price: $18" );

	DataNodeWaypointItem* warthogToy = new DataNodeWaypointItem("DATA_Waypoint_toy-halo-jda-warthog-packagefront_Still", 
		"Halo 4: UNSC Warthog Vehicle",  
		"figuret",
		"Includes 1 vehicle and 2 characters\
		- Master Chief & Marine\
		2\" Master Chief with 8 point articulation\
		2\" Marine with 8 point articulation\
		Removable Gun Turret\
		7\"L x 3.3\"W x 2.8\"H\
		\n\n\
		Price: $15" );

	DataNodeWaypointItem* avatarNeedler = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Needler_Prop_Still", 
		"Avatar Needler Toy Prop",  
		"helmet",
		"Because sometimes an alien firearm \
		is just the thing to tie it all together.\
		\n\n\
		Price: $5" );

	DataNodeWaypointItem* unscHoodie = new DataNodeWaypointItem("DATA_Waypoint_hoodie-halo-unsc-COMBO_Still", 
		"UNSC Hoodie",  
		"clothes",
		"Represent the United Nations Space Command,\
		even when you're in your civvies.\
		\n\n\
		Mens full zip hoodie in Black made with \
		premium heavyweight fabric\
		10 oz 80/20 cotton/poly blend fleece\
		Split stitch double needle sewing on all seams\
		Twill neck tape\
		\n\n\
		Price: $58" );

	DataNodeWaypointItem* unscBag = new DataNodeWaypointItem("DATA_Waypoint_bag-halo-unscmessenger-front_Still", 
		"UNSC Messenger Bag",  
		"clothes",
		"It is a UNSC bag, but it's much better \
		at carrying your books and computers than \
		your guns and grenades.\
		\n\n\
		1680D Black Ballistic nylon. \
		12\"h x 16.25\"w x 5.75\"d\
		Padded carrying handle\
		Laptop sleeve. Fits most 15\" laptops\
		Multiple interior organizational pockets\
		\n\n\
		Price: $60" );

	DataNodeWaypointItem* unscCap = new DataNodeWaypointItem("DATA_Waypoint_hat-halo-unscsnapback-side_Still", 
		"UNSC Snapback Cap",  
		"clothes",
		"Black, Original Snapback Cap. \
		UNSC 3D embroidery on front with \
		Halo embroidery on back.\
		\n\n\
		Price: $20" );

	DataNodeWaypointItem* haloGhostsBook = new DataNodeWaypointItem("DATA_Waypoint_book-halo-ghostsofonyx-angle_Still", 
		"Halo: Ghosts of Onyx",  
		"book",
		"The Spartan-II program has gone public. \
		Tales of super-soldiers fending off thousands \
		of Covenant attacks have become the stuff \
		of legend. But just how many Spartans are left?\
		\n\n\
		While the Master Chief defends a besieged \
		Earth, and the myriad factions of the \
		Covenant continue their crusade to eliminate \
		humanity, an ultrasecret cell of the Office \
		of Naval Intelligence known as \"Section Three\"\
		devises a plan to buy the UNSC vital time. \
		They're going to need hundreds of willing \
		soldiers, though and one more Spartan to \
		get the job done.\
		\n\n\
		The planet Onyx is virtually abandoned \
		and the perfect place to set this new plan \
		in motion. But when the Master Chief \
		destroys Halo, something is triggered \
		deep within Onyx: Ancient Forerunner \
		technology stirs, and fleets of UNSC \
		and Covenant race to claim it to change \
		the course of the Human-Covenant War. \
		\n\n\
		But this reawakened and ancient force \
		may have plans of its own...\
		\n\n\
		Author: Eric Nylund\
		Page Count: 384\
		\n\n\
		Price: $15" );

	DataNodeWaypointItem* playArtsSpartan = new DataNodeWaypointItem("DATA_Waypoint_etc-figure-halo-sqenix-redspartan-kneel_Still", 
		"Play Arts Spartan Soldier",  
		"figuret",
		"As the latest edition to Square Enix's \
		highly acclaimed Play Arts Kai action \
		figure line, these meticulously detailed \
		figures have advanced articulation making \
		a variety of action poses from the game \
		possible. Series 2 action figures include \
		2 weapon accessories and display stand.\
		\n\n\
		Weapons: Forerunner Suppressor \
		and UNSC Magnum\
		Display Stand included.\
		Figure Size: Approx. 9 inches\
		Weight: Approx. 1.4 lbs\
		Full-Color PVC Action Figure\
		\n\n\
		Price: $70" );

	DataNodeWaypointItem* avatarWarthog = new DataNodeWaypointItem("DATA_Waypoint_Avatar_Warthog_Prop_Still", 
		"Avatar Halo 4 Warthog Prop",  
		"helmet",
		"Let your Avatar roll out on the Warthog!\
		\n\n\
		Price : $5" );

	DataNodeWaypointItem* avatarRCWarthog = new DataNodeWaypointItem("DATA_Waypoint_Avatar_RC_Warthog_Prop_Still", 
		"Avatar RC Warthog Prop",  
		"helmet",
		"A toy Warthog for your Avatar.\
		\n\n\
		Price: $5" );

	DataNodeWaypointItem* collectorsWarthog = new DataNodeWaypointItem("DATA_Waypoint_toy-halo-jda-warthogce-packagefront_Still", 
		"Halo 4: UNSC Warthog \n\
		Collector's Edition Vehicle",  
		"figuret",
		"Working Shocks\
		Working Steering Wheel\
		Opening Hood with Detailed\
		Engine Compartment\
		Removable Gun Turret\
		14\"L x 6.6\"W x 5.6\"H\
		\n\n\
		Price: $15" );


//------------------------------------------
// CHARACTERS
//------------------------------------------

//LASKY
	DataNodeRef lasky = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_cams", "data_LASKY", 
		"THOMAS LASKY", "CADET, CAMS (HASTATI SQUAD)", 
		"Raised on Mare Erythraeum, Mars, Thomas Lasky is the youngest in \
		a long line of decorated UNSC officers. Inculcated with an appreciation \
		for strategic oversight from an early age (by both his mother Col. \
		Audrey Lasky and brother Cadmon), his ability to assess combat situations \
		without emotional attachment has made him a noted asset among at the Corbulo \
		Academy of Military Science. \
		\n\n\
		Unfortunately, his inability to communicate his ideas clearly often puts\
		him at odds with his peers, and as a result his academic performance has \
		sometimes fallen short of expectations. Despite this, he demonstrates \
		a firm resolve and unwavering moral compass - essential traits to becoming \
		a successful member of the UNSC." ));

	lasky->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_LASKY", "scenes/data/video/data_video_LASKY.mp4", "Psych Eval") );
	lasky->addWaypointItem( camsShirt );
	lasky->addWaypointItem( avatarCamsShirt );
	lasky->addWaypointItem( avatarCamsFormal );
	lasky->addWaypointItem( megaBlocksCryobay );

	data.push_back( lasky ); 
	scenes[0]->addData( lasky, 0 );
	scenes[2]->addData( lasky, 2*60.0f + 46 + offset );
	scenes[4]->addData( lasky, 0);
	scenes[5]->addData( lasky, 5*60.0f + 38  + offset);
	scenes[6]->addData( lasky, 7*60.0f + 1 + offset );
	scenes[7]->addData( lasky, 8*60.0f + 41 + offset );
	scenes[9]->addData( lasky, 9*60.0f + 5 + offset );
	scenes[10]->addData( lasky, 10*60.0f + 51 + offset );
	scenes[11]->addData( lasky, 13*60.0f + 45 + offset );
	scenes[12]->addData( lasky, 14*60.0f + 52 + offset );
	scenes[13]->addData( lasky, 15*60.0f + 57 + offset );
	scenes[14]->addData( lasky, 17*60.0f + 27 + offset );
	scenes[15]->addData( lasky, 18*60.0f + 2 + offset );

//CHYLER
	DataNodeRef chyler = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_cams", "data_SILVA", 
		"CHYLER SILVA", "CADET, CAMS (HASTATI SQUAD)", 
		"A native of Cygnus, Chyler was raised in the midst of the \
		Human Civil War. Her immediate family was killed over the course \
		of one day - her mother in the initial bombing of Cygnus\' Pentara \
		Medical Center, and her father and brother in the secondary blast. \
		She was recommended to CAMS officials by her uncle, a lieutenant \
		in the Marine Corps. \
		\n\n\
		Though she has proven to be extremely effective on the field, \
		Chyler fights out of a sense of obligation rather than primal need, \
		and tends to her squadmates as the de facto mother of Hastati." ));

	chyler->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_CHYLER", "scenes/data/video/data_video_CHYLER.mp4", "Psych Eval") );
	chyler->addWaypointItem( camsShirt );
	chyler->addWaypointItem( avatarCamsShirt );
	chyler->addWaypointItem( avatarCamsFormal );

	data.push_back( chyler ); 
	scenes[2]->addData( chyler, 2*60.0f + 39 + offset );
	scenes[4]->addData( chyler, 0 );
	scenes[9]->addData( chyler, 9*60.0f + 0 + offset );
	scenes[10]->addData( chyler, 10*60.0f + 51 + offset );
	scenes[11]->addData( chyler, 13*60.0f + 45 + offset );
	scenes[12]->addData( chyler, 14*60.0f + 52 + offset );
	scenes[13]->addData( chyler, 15*60.0f + 57 + offset );
	scenes[14]->addData( chyler, 17*60.0f + 27 + offset );
	scenes[15]->addData( chyler, 18*60.0f + 2  + offset );

//SULLY
	DataNodeRef sully = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_cams", "data_SULLY", 
		"MICHAEL SULLIVAN", "CADET, CAMS (HASTATI SQUAD)", 
		"Michael Sullivan is the son of renowned ONI officer Marcus Sully (MIA). \
		Though his performance on the field is consistently substandard, \
		Sully\'s chief strength is a strategic acumen none of his peers can match. \
		His disregard for authority and 150+ level IQ make for a somewhat colorful \
		academic record, featuring two official Letters of Reprimand in his first year \
		alone: one in response to an unsuccessful attempt to access the Corbulo Tether\'s \
		operational manifests, and another for forging an administrative authentication key. \
		The record states that he used the latter to reprogram one of the mess hall\'s \
		Autocooks, which then baked a meal consisting entirely of different \
		kinds of cake for the entire school." ));

	sully->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_SULLY", "scenes/data/video/data_video_SULLY.mp4", "Psych Eval") );
	sully->addWaypointItem( camsShirt );
	sully->addWaypointItem( avatarCamsShirt );
	sully->addWaypointItem( avatarCamsFormal );

	data.push_back( sully ); 
	scenes[2]->addData( sully, 3*60.0f + 30 + offset );
	scenes[4]->addData( sully, 0 );
	scenes[6]->addData( sully, 8*60.0f + 9 + offset );
	scenes[10]->addData( sully, 11*60.0f + 32 + offset );
	scenes[11]->addData( sully, 13*60.0f + 45 + offset );
	scenes[12]->addData( sully, 14*60.0f + 52 + offset );
	scenes[13]->addData( sully, 15*60.0f + 57 + offset );
	scenes[14]->addData( sully, 17*60.0f + 27 + offset );
	scenes[15]->addData( sully, 18*60.0f + 2 + offset );

//VICKERS
	DataNodeRef vickers = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_cams", "data_VICKERS", 
		"WALTER VICKERS", "CADET, CAMS (HASTATI SQUAD)", 
		"Walter Vickers was thirteen years old when his parents were killed by \
		Insurrectionist forces on his embattled homeworld of Tyrnam-IX. Vickers \
		survived the assault and gained the attention of ODST on-site, who brought \
		him aboard the UNSC Hannibal. Working odd jobs aboard the gigantic \
		cruiser, Vickers was eventually able to use his late father's military \
		credentials to enroll at CAMS." ));

	vickers->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_VICKERS", "scenes/data/video/data_video_VICKERS.mp4", "Psych Eval") );
	vickers->addWaypointItem( camsShirt );
	vickers->addWaypointItem( avatarCamsShirt );
	vickers->addWaypointItem( avatarCamsFormal );

	data.push_back( vickers ); 
	scenes[2]->addData( vickers, 3*60.0f + 40 + offset );
	scenes[4]->addData( vickers, 0 );
	scenes[10]->addData( vickers, 11*60.0f + 32 + offset );
	scenes[11]->addData( vickers, 13*60.0f + 45 + offset );
	scenes[12]->addData( vickers, 14*60.0f + 52 + offset );
	scenes[13]->addData( vickers, 15*60.0f + 57 + offset );
	scenes[14]->addData( vickers, 17*60.0f + 27 + offset );
	scenes[15]->addData( vickers, 18*60.0f + 2  + offset );

//DIMAH
	DataNodeRef dimah = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_cams", "data_DIMAH", 
		"DIMAH TCHAKOVA", "CADET, CAMS (HASTATI SQUAD)", 
		"Dimah Tchakova is the daughter and only child of Admiral Irina Tchakova, \
		Secretary of FLEETCOM, and Aleksandr Tchakov, a high-ranking Naval \
		Intelligence officer. Born and raised on Reach, Dimah was tutored privately \
		and sheltered from the general population until she was eligible to enroll \
		at CAMS. Though she may seem aloof at times, Dimah is well-liked amongst \
		her squad mates - and no one in Hastati is more capable of navigating the \
		academy's strict protocols of conduct than she is." ));

	dimah->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_DIMAH", "scenes/data/video/data_video_DIMAH.mp4", "Psych Eval") );
	dimah->addWaypointItem( camsShirt );
	dimah->addWaypointItem( avatarCamsShirt );
	dimah->addWaypointItem( avatarCamsFormal );

	data.push_back( dimah ); 
	scenes[2]->addData( dimah, 3*60.0f + 40 + offset );
	scenes[4]->addData( dimah, 0 );
	scenes[10]->addData( dimah, 11*60.0f + 32 + offset );
	scenes[11]->addData( dimah, 13*60.0f + 45 + offset );

//JJ
	DataNodeRef jj = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_cams", "data_CHEN", 
		"JUNJIE \"JJ\" CHEN", "CADET, CAMS (HASTATI SQUAD)", 
		"Junjie Chen was born on Earth but spent his early childhood aboard a UNSC \
		cruiser, not reentering a planet's atmosphere until the age of ten. \
		His diplomat father eventually settled their family on the demilitarized \
		colony of Tairen. Finally rooted and with a new world to call home, \
		Junjie's life was torn asunder when a fringe terrorist cell detonated \
		explosives in Tairen's commercial center, killing thousands - including \
		Junjie's mother. Imbued with a new sense of purpose, he applied to CAMS \
		with his father's blessing, and is one of the rare students at the academy \
		who was admitted without descending from a military legacy." ));

	jj->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_JJ", "scenes/data/video/data_video_JJ.mp4", "Psych Eval") );
	jj->addWaypointItem( camsShirt );
	jj->addWaypointItem( avatarCamsShirt );
	jj->addWaypointItem( avatarCamsFormal );

	data.push_back( jj ); 
	scenes[2]->addData( jj, 3*60.0f + 40 + offset );
	scenes[4]->addData( jj, 0 );
	scenes[10]->addData( jj, 11*60.0f + 32 + offset );
	scenes[11]->addData( jj, 13*60.0f + 45 + offset );
	scenes[12]->addData( jj, 14*60.0f + 52 + offset );
	scenes[13]->addData( jj, 15*60.0f + 57 + offset );
	scenes[14]->addData( jj, 17*60.0f + 27 + offset );

//APRIL
	DataNodeRef april = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_cams", "data_ORENSKI", 
		"APRIL ORENSKI", "CADET, CAMS (HASTATI SQUAD)", 
		"Hailing from Harvest, April Orenski's father was one of the \
		few military personnel stationed on the otherwise peaceful \
		agriculture world. From infancy she was raised to respect \
		chain of command - after her father was killed, she deferred \
		to her elder brother\'s wishes to remain on Harvest and take care \
		of her remaining family. When it was revealed that her brother was \
		an Insurrectionist sympathizer, however, she immediately relocated \
		her mother and sister and enrolled at CAMS. \
		\n\n\
		A natural fit for the academy, she hit the ground running and was \
		assigned as squad leader for Hastati after her first term." ));

	april->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_APRIL", "scenes/data/video/data_video_APRIL.mp4", "Psych Eval") );
	april->addWaypointItem( camsShirt );
	april->addWaypointItem( avatarCamsShirt );
	april->addWaypointItem( avatarCamsFormal );

	data.push_back( april ); 
	scenes[10]->addData( april, 11*60.0f + 32 + offset );
	scenes[11]->addData( april, 13*60.0f + 45 + offset );
	scenes[12]->addData( april, 14*60.0f + 52 + offset );
	scenes[13]->addData( april, 15*60.0f + 57 + offset );
	scenes[14]->addData( april, 17*60.0f + 27 + offset );
	scenes[15]->addData( april, 18*60.0f + 2  + offset );

//CADMON
	DataNodeRef cadmon = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_unsc", "data_CADMON", 
		"LCPL. CADMON LASKY", "ODST", 
		"Lance Corporal Cadmon Lasky was a demolitions specialist \
		serving as an ODST with the 10th Battalion. Having graduated \
		from the Corbulo Academy of Military Science second in his class, \
		Lasky was poised to begin what many assumed would be a promising \
		career in Naval Intelligence - however, Cadmon surprised his schoolmates \
		and instructors by joining the Marine Infantry and soon after \
		volunteering for the ODST. \
		\n\n\
		He and the rest of his squad sacrificed their lives in the line \
		of duty during an attack on an Insurrectionist stronghold on Andesia. \
		He is survived by his younger brother Thomas and his mother \
		Colonel Audrey Lasky." ));

	cadmon->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_CADMON_1", "scenes/data/video/data_video_CADMON_1.mp4", "Comm") );
	cadmon->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_CADMON_2", "scenes/data/video/data_video_CADMON_2.mp4", "Comm") );
	cadmon->addWaypointItem( avatarOdstDropPod );
	cadmon->addWaypointItem( odstDropPod );
	
	data.push_back( cadmon ); 
	scenes[1]->addData( cadmon, 0 );


//MAMMA LASKY 
	DataNodeRef mammaLasky = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_unsc", "data_COL_LASKY", 
		"COL. AUDREY LASKY", "COLONEL, UNSCDF", 
		"As the child of secessionist sympathizers in an otherwise conservative \
		state on Mars, Audrey Lasky eschewed her family's values and joined the \
		UNSC at an early age. Her preternatural gift for sub-orbital combat strategy \
		saw her ascend the ranks and become one of the youngest colonels in the Marine \
		Corps' history. Over her storied career, she has come to be recognized as one \
		of the preeminent minds in the field of 26th-century asymmetrical warfare. \
		\n\n\
		Her brazen ambition and sense of duty extends to her children Cadmon and \
		Thomas (father deceased), whom she has positioned to become military \
		leaders in their own right." ));
	
	mammaLasky->addWaypointItem( avatarCamsFormal );
	mammaLasky->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_COL_LASKY", "scenes/data/video/data_video_COL_LASKY.mp4", "Comm") );
	data.push_back( mammaLasky ); 
	scenes[6]->addData( mammaLasky, 7*60.0f + 31 + offset );

//HUGES
	DataNodeRef huges = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_oni", "data_HUGHES", 
		"DR. ALAN HUGHES", "SENIOR MEDICAL OFFICER, CAMS", 
		"A decorated ONI medical officer who previously served 12 years as a UNSC\
		Field Surgeon for much of the Insurrectionist conflict, Alan Hughes was \
		reassigned to Corbulo Academy after being injured during the Battle of \
		Attenasio-II. While performing emergency surgery on a wounded comrade, \
		the medical facility Hughes was stationed in was ambushed by Insurrectionist \
		forces. Hughes and a select few of his colleagues took it upon themselves \
		to retake the facility and held it for seven grueling hours of \
		combat before reinforcements arrived.\
		\n\n\
		For his actions and exceptional bravery, \
		Hughes was awarded a medal of distinguished service." ));

	huges->setOni(true);
	huges->addWaypointItem( avatarCamsFormal );
	huges->addWaypointItem( megaBlocksCryobay );

	data.push_back( huges ); 
	scenes[0]->addData( huges, 0 );

//------------------------------------------
// ITEMS
//------------------------------------------


//HASTATI SQUAD
	DataNodeRef hastati = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_cams", "data_HASTATI", 
		"HASTATI SQUAD", " ", 
		"Named for Ancient Roman soldiers drawn from the humblest classes, \
		Hastati Squad is led by Officer Cadet April Orenski and consists of \
		Cadets Chen, Kaye, Lasky, Silva, Shere, Sullivan, Tchakova, and Vickers. \
		Of the first-year squads, Hastati presently ranks third in their school \
		standings, having recently failed a training exercise against Maratha Squad." ));
	
	hastati->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/16_HASTATI_Logo") );
	data.push_back( hastati );
	scenes[10]->addData( hastati, 11*60.0f + 32 + offset );
	scenes[11]->addData( hastati, 13*60.0f + 45 + offset );
	scenes[12]->addData( hastati, 14*60.0f + 52 + offset );
	scenes[13]->addData( hastati, 15*60.0f + 57 + offset );
	scenes[14]->addData( hastati, 17*60.0f + 27 + offset );
	scenes[15]->addData( hastati, 18*60.0f + 2  + offset );

	DataNodeRef teather = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_SpaceTether", 
		"SPACE TETHER", " ", 
		"A space tether is comprised of two central pieces: the conveyance chamber, \
		an immense elevator platform designed to assist in off-world shipping and \
		transportation, and the communications link, a monolithic antenna capable \
		of broadcasting transmissions between the colonies of the \
		Circinius system and beyond. \
		\n\n\
		Held aloft by a planet or moon\'s natural rotation, these enormous \
		spires are constructed out of vast meshes of carbon nanofiber many \
		kilometers in length, climbing into the upper atmosphere to orbital \
		facilities known as Terminuses. \
		\n\n\
		Tethers have been created in a variety \
		of sizes and shapes based on their specific needs. While most colonies \
		maintain only a single tether due to the cost involved, Earth holds six \
		enormous tethers and Reach holds a total of nine. Some high priority sites \
		dictated the creation of smaller, more discrete elevators, \
		such as the one utilized by CAMS." ));
	
	teather->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/12.SpaceTether") );
	data.push_back( teather );
	scenes[3]->addData( teather, 4*60.0f + 53 + offset );
	scenes[8]->addData( teather, 8*60.0f + 54 + offset );
	scenes[10]->addData( teather, 11*60.0f + 23 + offset );

//RAIN FORREST WAR
	DataNodeRef forrestWar = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_UNSC", 
		"THE RAIN FOREST WAR", " ", 
		"The Insurrection is a period of history beginning in the 2490\'s in which \
		pro-colonial forces aggressively fought for secession and autonomy for a \
		select number of Outer Colonies. This was observed by a number of key groups, \
		including the People\'s Occupation and the Secessionist Union, many of which \
		clashed with the UNSC through standard combat and terrorist strikes. Despite \
		the rampant and well-documented sectarian violence Insurrectionist forces \
		(commonly referred to as \"Innies\") wrought across the Colonies, many still \
		consider the UNSC\'s response as excessive and question the Unified Earth \
		Government\'s motives behind their drive to align all the Outer \
		Colonies under single governance." ));
	
	forrestWar->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/17_UNSC_Logo") );
	forrestWar->addWaypointItem( haloGhostsBook );

	data.push_back( forrestWar );
	scenes[1]->addData( forrestWar, 1*60.0f + 44 + offset );
	scenes[4]->addData( forrestWar, 5*60.0f + 18 + offset );



//SOEIV Pod
	DataNodeRef pod = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_SOEIV_Pod", 
		"SOEIV \"POD\"", "SINGLE OCCUPLANT EXOATMOSPHERIC INSERTION VEHICLE", 
		"The Single Occupant Exoatmospheric Insertion Vehicle is the ODST\'s \
		transport of choice when initiating assaults against terrestrial combatants. \
		The carriage stands 18 ft (5.4m) tall, 8.5 (2.6m) wide, 9 ft (2.7m) \
		deep and contains a single crash seat, harness, and equipment racks/lockers \
		to be equipped at the individual soldier\'s discretion. Despite the pod\'s size, \
		once fully loaded out, movement within the pod is extremely restricted. \
		\n\n\
		A major advantage to SOEIVs over traditional dropships is that a spacecraft \
		can make a fast pass over a planet and deploy hundreds of soldiers in quick \
		succession. This means that the ship is capable of evasive maneuvers, \
		limiting its vulnerability to attack." ));
	
	pod->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/5.Pod") );
	pod->addWaypointItem( avatarOdstDropPod );
	pod->addWaypointItem( odstDropPod );

	data.push_back( pod );
	scenes[4]->addData( pod, 5*60.0f + 18 + offset );
	scenes[10]->addData( pod, 12*60.0f + 01 + offset );

//CYTOPRETHALINE 
	DataNodeRef cyto = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_Cytoprethaline", 
		"CYTOPRETHALINE", "CRYOGENICS MEDICATION", 
		"Cytoprethaline is a chemical compound that protects cell membranes \
		against ice damage during cryo-sleep. It is regularly administered \
		to active-duty UNSC officers undergoing long-distance travel, \
		as well as cadets in training to acclimate them to the \
		effects of the drug. \
		\n\n\
		In rare cases, subjects can exhibit allergic responses to \
		Cytoprethaline - the individual\'s circulatory system during the \
		\"freeze-down\" overcompensates and rapidly heats the body, \
		resulting in rash and blistering." ));

	cyto->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/13.Cytopetthaline") );
	cyto->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "items_Cytoprethaline_DATA_video", "scenes/data/video/items_Cytoprethaline_DATA_video.mp4", "Video") );
	cyto->addWaypointItem( megaBlocksCryobay );
	data.push_back( cyto );
	scenes[0]->addData( cyto, 0*60.0f + 27 + offset );

//ASSULT RIFLE
	DataNodeRef rifle = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_Rifles", 
		"ASSAULT RIFLE", "MISRIAH ARMORY", 
		"The term \"Assault Rifle\" defines a large platform of automatic \
		rifles belonging to the MA series of Individual Combat Weapon \
		Systems from Misriah Armory. In terms of fundamental architecture, \
		they are air-cooled, gas-operated, magazine-fed rifles with a pronounced \
		shroud and a rugged, compact design. Although magazine size can vary \
		(typically between 32 and 60 rounds), most Assault Rifles fire \
		M118 7.62 x 51mm FMJ-AP rounds capable of deterring soft targets \
		quickly and cleanly. The MA37 Assault Rifle is the weapon of choice \
		within the UNSC Army for combat at short-to-mid range, being favored \
		above the most recent upgrades thanks to its impressive durability." ));
	
	rifle->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/6.Rifles") );
	rifle->addWaypointItem( armoryShirt );
	rifle->addWaypointItem( tubeChief );
	rifle->addWaypointItem( playArtsChief );
	rifle->addWaypointItem( megaBlocksCryobay );

	data.push_back( rifle );
	scenes[2]->addData( rifle, 4*60.0f + 2 + offset );
	scenes[6]->addData( rifle, 8*60.0f + 20 + offset );


//SPARTAN II 
	DataNodeRef spartanII = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_spart", "data_SPARTAN_II", 
		"SPARTAN II", "AUGMENTED HUMAN", 
		"Under the directorship of Dr. Catherine Halsey, The SPARTAN-II Program \
		was a direct descendant of the ORION project - the UNSC and ONI\'s \
		concerted effort to create the next generation of super-soldier. \
		Where the cost of maintaining SPARTAN-I ballooned due to an \
		overextended pool of test subjects (culled from volunteer UNSC personnel), \
		SPARTAN-II was unique in that its subjects were not volunteers at all. \
		Instead, they were an elite, hand-picked assembly of children with exceptional \
		physical and intellectual aptitude, trained from pre-adolescence in the \
		theory and practice of combat. \
		\n\n\
		When the children came of age, several genetic enhancements were \
		introduced to their nervous systems, increasing reaction times and sensitivity \
		to heat, light, and movement. Lastly, the SPARTAN-II's skeletal structures \
		would be enhanced and in some cases replaced with shock-absorbent ceramic plating. \
		Coupled with the newly-developed MJOLNIR armor, the SPARTAN-II was truly \
		the most formidable unit the UNSC could have hoped for in the fight \
		against the Insurrection." ));
	
	spartanII->addWaypointItem( playArtsSpartan );
	
	spartanII->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/9.Spartons") );
	data.push_back( spartanII );
	scenes[2]->addData( spartanII, 3*60.0f + 54 + offset );


//ONI SULLY VIDEO 
	DataNodeRef sullyVideo = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_oni", "data_video_ONI_head", 
		"CLASSIFIED TRANSMISSION", " ", 
		"This transmission contains classified footage of a battle between \
		UNSC and Insurrectionists. Decrypted by Michael Sully, it seems to \
		indicate that the two forces united to fight off unidentified enemy \
		combatants, and were led by some sort of cybernetically enhanced soldier. \
		The point of origin appears to have been somewhere in the Am-Adad system." ));
	
	sullyVideo->setOni(true);
	sullyVideo->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "data_video_ONI", "scenes/data/video/data_video_ONI.mp4", "CLASSIFIED TRANSMISSION" ) );
	data.push_back( sullyVideo );
	scenes[2]->addData( sullyVideo, 3*60.0f + 55 + offset );



//ODST
	DataNodeRef odst = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_unsc", "data_ODST", 
		"Orbital Drop Shock Trooper", " ", 
		"For decades, Orbital Drop Shock Troopers have been widely known and \
		well regarded for their fearlessness. Their name is derived from their \
		standard method of deployment: plummeting into combat from exoatmospheric \
		capital ships by way of heavily shielded drop pods. A Special Forces segment \
		of the UNSC Marine Corps, ODST is a volunteer military force and are \
		considered the strongest and most tactically proficient soldiers in the UNSC. \
		\n\n\
		While ODST personnel can operate in a wide range of capacities, they specialize \
		in ground surveillance and long-range reconnaissance, direct combat engagement, \
		unconventional warfare and tactics, counter-terrorism, and specialized \
		escort operations." ));
	
	odst->addWaypointItem( avatarOdstDropPod );
	odst->addWaypointItem( odstDropPod );

	data.push_back( odst );
	scenes[4]->addData( odst, 5*60.0f + 18 + offset );
	scenes[10]->addData( odst, 12*60.0f + 01 + offset );


//ENERGY SWORD 
	DataNodeRef sword = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_cov", "data_EnergySword", 
		"ENERGY SWORD", "SANGHEILI ARMAMENT", 
		"An Energy Sword on an approaching Sangheili warrior is enough \
		to make any human flee in terror.The plasma sheet that makes up \
		the blade of the weapon does not hack or carve, but rather boils \
		away anything it comes into contact with. Anyone unlucky enough \
		to be in the path of its slice is assured a most painful and \
		gruesome death. The only saving grace (for human attackers, at least) \
		is that any Sangheili brave enough to wield such a weapon are \
		temporarily exposed to long-range attacks." ));
	
	sword->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/7.EnergySwords") );
	sword->addWaypointItem( avatarSword );
	sword->addWaypointItem( avatarSwordShirt );
	sword->addWaypointItem( zelotToy );

	data.push_back( sword );

	scenes[6]->addData( sword, 8*60.0f + 20 + offset );
	scenes[11]->addData( sword, 14*60.0f + 47 + offset );
	scenes[13]->addData( sword, 16*60.0f + 47 + offset );
	scenes[14]->addData( sword, 17*60.0f + 27 + offset );
	scenes[15]->addData( sword, 1*60.0f + 2 + offset );

//ONI INFO
	DataNodeRef oni = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_oni", "data_ONI", 
		"ONI", "OFFICE OF NAVAL INTELLIGENCE", 
		"The Office of Naval Intelligence was initially created to \
		establish operational intelligence for naval strategy and combat. \
		It  would later become its own UNSC subordinate authority, \
		and ultimately be the driving factor for much of the UNSC\'s \
		military action in the 24th and 25th centuries. The agency holds \
		dozens of major facilities across human space, the most notable of \
		which are on Earth and Reach. Their base of operations is in Sydney, \
		Australia\'s Bravo-6 headquarters, also known as \"The Hive.\" \
		\n\n\
		ONI is divided up into a number of sections with varying degrees \
		of power and functional priorities: Section One deals with espionage, \
		Section Two handles external communication and public morale, \
		Section Three manages a number of classified special projects, \
		and finally, Section Zero, whose activities remain a matter \
		of speculation, operate outside of all other ONI, UNSC, \
		and UEG jurisdictions." ));
	
	oni->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/15_ONI") );
	oni->addWaypointItem( unscHoodie );
	oni->addWaypointItem( unscBag );
	oni->addWaypointItem( unscCap );
	oni->addWaypointItem( haloGhostsBook );

	data.push_back( oni );
	scenes[6]->addData( oni, 8*60.0f + 34 + offset );

//COVENANT CORVETTE
	DataNodeRef corvette = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_cov", "data_Corvette", 
		"CORVETTE", "COVENANT", 
		"Used primarily in reconnaissance-gathering missions, Corvettes represent \
		just a fraction of the Covenant\'s considerable naval resources. There have \
		been sightings of several types of Corvette - the two most common being the \
		DAV (Light Corvette) and SDV (Heavy Corvette). \
		\n\n\
		The smaller, more nimble DAV-class is equipped with active camouflage \
		and can transmit data from a planet\'s surface, where the SDV-class is \
		more often associated with troop deployment. Corvettes are usually equipped \
		with only light armament having been built for speed and stealth. Nevertheless, \
		they are more than capable of leveling enemy targets with their \
		plasma cannon batteries." ));
	
	corvette->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/11.Corvette") );
	data.push_back( corvette );
	scenes[10]->addData( corvette, 13*60.0f + 21 + offset );

//NEEDLER
	DataNodeRef needler = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_cov", "data_Needler", 
		"NEEDLER", "TYPE-33 GUIDED MUNITIONS LAUNCHER", 
		"The Needler is one of the Covenant's more definitively alien armaments. \
		It is loaded with fourteen charged, slender energy shards that protrude \
		from the top of the weapon, which are fed through the chamber and fired \
		at high speed. The shards are capable of ricocheting in oblique angles \
		and homing in on organic targets. Once they penetrate flesh, the \"needles\" \
		detonate after a preprogrammed delay.\
		\n\n\
		Though the engineering behind the T-33 is poorly understood, its purpose - \
		for dispensing sheer chaos - is certain." ));
	
	needler->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/3.Needler") );
	needler->addWaypointItem( avatarNeedler );

	data.push_back( needler );

	scenes[12]->addData( needler, 15*60.0f + 05 );

//WARTHOG
	DataNodeRef hog = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_Warthog", 
		"WARTHOG", "M12 LIGHT RECONNAISSANCE VEHICLE", 
		"Nothing symbolizes UNSC ground forces more than the M12 Warthog LRV. \
		With almost a dozen different variations (including one built for \
		civilian transport), it is as integral a part of the UNSC as boots, \
		guns, and tasteless coffee. Built for speed, maneuverability, \
		and maximum deterrence, it is one of the cornerstones of human \
		terrestrial combat deployment." ));
	
	hog->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/4.Warthog") );
	hog->addWaypointItem( warthogToy );
	hog->addWaypointItem( avatarWarthog );
	hog->addWaypointItem( avatarRCWarthog );
	hog->addWaypointItem( collectorsWarthog );

	data.push_back( hog );
	
//MEHAFFEY 
	DataNodeRef mehaffey = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_unsc", "data_MEHAFFEY", 
		"COL. TARA MEHAFFEY", "COLONEL, UNSC", 
		"A hardened veteran of the Insurrectionist War, Tara Mehaffey was \
		raised on the volcanic planet of Abure Prime. Initially deployed as \
		an ODST, a young Mehaffey was placed in a squadron with a \
		then-first-lieutenant Audrey Lasky. The two ascended the ranks together \
		and developed fearsome reputations - Lasky for her encyclopedic intellect \
		and Mehaffey for her relentlessness on the field. \
		\n\n\
		Her penchant for engaging Insurrectionists with her combat knife as \
		her primary weapon earned her the handle \"Icepick.\" To her enemies, \
		she is commonly referred to as \"The Butcher.\" \
		\n\n\
		Mehaffey was famously transferred from active duty \"without a scratch,\" \
		leaving students to speculate about the origins of her scar. \
		She serves as the director of CAMS, and has been training the UNSC's \
		best and brightest for almost ten years." ));

	mehaffey->addWaypointItem( avatarCamsFormal );
	data.push_back( mehaffey ); 
	scenes[5]->addData( mehaffey, 5*60.0f + 41 + offset );
	scenes[12]->addData( mehaffey, 15*60.0f + 20 );

	
//DISCHARGE LETTER
	DataNodeRef letter = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_letter_head", 
		"DISCHARGE LETTER", " ", 
		"A formal recommendation that Thomas Lasky be removed from active combat." ));
	
	letter->setOni(true);
	letter->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::DOCUMENT, "data_letter", "scenes/data/data_letter.png", "DISCHARGE LETTER") );
	data.push_back( letter );
	scenes[5]->addData( letter, 5*60.0f + 50 + offset );

//------------------------------------------
// ONI FILES
//------------------------------------------
	
//ELITE
	DataNodeRef elite = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_cov", "data_Elite_Zealot", 
		"ELITE", "SANGHEILI, COVENANT OF RACES", 
		"The Sangheili (commonly referred to as the \"Elites\") are a proud \
		warrior species and among the first to have aligned themselves with \
		the Covenant of races. They are biped saurians with tall, muscular \
		bodies covered in fine, leathery scales. Their blunt, elongated heads \
		bear four mandibles, each laced with rows of serrated teeth. \
		Their bodies are built on a lean frame with long, sinewy arms that \
		end in zygodactyl hands, bearing impressively dexterous fingers. \
		\n\n\
		Sangheili youth are raised in collectives called \"keeps\" and \
		never inherit family names; this has created a unique society \
		fueled on individual accomplishment. With their imposing physiology, \
		cultural pressure to prove themselves in combat, and religious zeal \
		towards the mission of the Covenant, the Elites are some of the most \
		deadly enemies humanity has ever faced." ));
	
	elite->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/8.EliteZlot") );
	elite->addWaypointItem( avatarArbiterMask );
	elite->addWaypointItem( avatarElite );
	elite->addWaypointItem( banshee );
	elite->addWaypointItem( avatarSword );
	elite->addWaypointItem( avatarSwordShirt );
	elite->addWaypointItem( zelotToy );

	data.push_back( elite );
	scenes[11]->addData( elite, 14*60.0f + 47 + offset );
	scenes[13]->addData( elite, 16*60.0f + 47 + offset );
	scenes[14]->addData( elite, 17*60.0f + 27 + offset );
	scenes[15]->addData( elite, 1*60.0f + 2 + offset );

//FRED
	DataNodeRef fred = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_spart", "data_FRED", 
		"FREDERIC-104", "LIEUTANANT, SPARTAN-II PROJECT", 
		"Frederic-104 was one of the first children to be selected for the \
		SPARTAN-II project. A natural leader and brilliant tactician, \
		his heroic actions during the Fall of Reach would cement his place \
		amongst the most admired war heroes in UNSC history. \
		\n\n\
		Frederic-104 and his Blue Team contingent were last recorded near \
		the planet Onyx. Their current whereabouts are unknown." ));
	
	data.push_back( fred );
	//scenes[2]->addData( fred, 4*60.0f + 36 + offset );


//CHIEF
	DataNodeRef chief = DataNodeRef( new DataNode(DataNode::DataType::CHARACTER, "icon_title_spart", "data_CHIEF", 
		"JOHN-117", "MASTER CHIEF PETTY OFFICER, SPARTAN-II PROJECT", 
		"Despite having hidden his face from the world for most of his \
		military career, John-117 - popularly referred to as \
		\"Master Chief\" - is one of the most recognizable personalities \
		across the explored universe. Renowned for his quick thinking on \
		the battlefield, legendary leadership skills, and respect for his \
		brothers in arms, Master Chief has come to serve as a symbol \
		of hope for humankind." ));
	
	chief->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/10.MasterChief") );
	chief->addWaypointItem( playArtsChief );
	chief->addWaypointItem( tubeChief );
	chief->addWaypointItem( megaBlocksCryobay );
	chief->addWaypointItem( chiefCostume );
	chief->addWaypointItem( mcFarlaneChief );
	chief->addWaypointItem( imHereShirt );
	chief->addWaypointItem( chiefIllustrationShirt );
	chief->addWaypointItem( warthogToy );

	data.push_back( chief );


//NIBIRU ENGAGEMENT
	DataNodeRef nibiruEngage = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_unsc", "data_UNSC", 
		"NIBIRU ENGAGEMENT", "CLASSIFIED ONI BRIEF", 
		"On 1/12/2526, an Insurrectionist strike team hiding on the second moon of \
		Nibiru assaulted a UNSC-operated logistical compound. Reinforcements were \
		dispatched in what was expected to be a routine operation. Once on-site, \
		however, the op spiraled out of control due to the presence of an unforeseen \
		second threat: a Covenant scouting expedition. \
		\n\n\
		UNSC and Insurrection units \
		had no choice but to fight side by side and would have almost certainly been \
		overwhelmed by the Covenant if not for the quick intervention of the Spartan \
		Frederic-104 and his ODST tactical group." ));
	
	nibiruEngage->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/17_UNSC_Logo") );
	data.push_back( nibiruEngage );


//10/01-11/30, 2525
	DataNodeRef file1001_1130 = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_oni", "data_ONI", 
		"10/01-11/30, 2525", "FILES RECORDED", 
		"CONFERENCE: ONI DIRECTORS, COL. AUDREY LASKY\
		\n\n\
		AUTOMATED EXTRACTION REQUEST FOR CADET THOMAS LASKY\
		\n\n\
		DISPATCH TO CENTRAL FROM DR. ALAN HUGHES, CAMS" ));
	
	file1001_1130->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/15_ONI") );
	file1001_1130->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::AUDIO, "data_audio_thumb", 
		"scenes/data/oni/ONI_FILES_REC_1001_1130_CONFERENCE_AUDIO_ANIMATION.mp4", 
		"CONFERENCE: ONI DIRECTORS, COL. AUDREY LASKY") );

	file1001_1130->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::DOCUMENT, "data_oni_10_01_11_30_automated_extraction", 
		"scenes/data/oni/data_oni_10_01_11_30_automated_extraction.png", 
		"AUTOMATED EXTRACTION REQUEST FOR CADET THOMAS LASKY" ) );

	file1001_1130->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::DOCUMENT, "data_oni_10_01_11_30_dispatch", 
		"scenes/data/oni/data_oni_10_01_11_30_dispatch.png", 
		"DISPATCH TO CENTRAL FROM DR. ALAN HUGHES, CAMS" ) );

	data.push_back( file1001_1130 );


//12/01-01/31, 2526
	DataNodeRef file1201_0131 = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_oni", "data_ONI", 
		"12/01-01/31, 2526", "FILES RECORDED", 
		"PRIVATE LOG OF E1R MARCUS SULLY, 12/05/2525 \
		\n\n\
		PRIVATE LOG OF E1R MARCUS SULLY, 12/22/2525\
		\n\n\
		AUTOMATED EXTRACTION REQUEST FOR CADET THOMAS LASKY" ));
	
	file1201_0131->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/15_ONI") );
	file1201_0131->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::AUDIO, "data_audio_thumb", "scenes/data/oni/ONI_FILES_REC_1201_0131_Log_1205.mp4", 
		"PRIVATE LOG OF E1R MARCUS SULLY, 12/05/2525") );
	file1201_0131->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::AUDIO, "data_audio_thumb", "scenes/data/oni/ONI_FILES_REC_1201_0131_Log_1222.mp4", 
		"PRIVATE LOG OF E1R MARCUS SULLY, 12/22/2525") );
	file1201_0131->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::DOCUMENT, "data_oni_12_01_01_31_automated_extraction", "scenes/data/oni/data_oni_12_01_01_31_automated_extraction.png", 
		"AUTOMATED EXTRACTION REQUEST FOR CADET THOMAS LASKY") );

	data.push_back( file1201_0131 );


//02/01-03/31, 2526
	DataNodeRef file1201_0331 = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_oni", "data_ONI", 
		"02/01-03/31, 2526", "FILES RECORDED", 
		"PERSONAL COMMUNICATION COL. LASKY\
		\n\n\
		PERSONAL COMMUNICATION COL. MEHAFFEY" ));
	
	file1201_0331->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/15_ONI") );
	file1201_0331->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::DOCUMENT, "data_oni_02_01_03_31_lasky", "scenes/data/oni/data_oni_02_01_03_31_lasky.png", 
		"PERSONAL COMMUNICATION COL. LASKY") );

	file1201_0331->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::DOCUMENT, "data_oni_02_01_03_31_mehaffey", "scenes/data/oni/data_oni_02_01_03_31_mehaffey.png", 
		"PERSONAL COMMUNICATION COL. MEHAFFEY") );
	
	data.push_back( file1201_0331 );


//04/15, 2526
	DataNodeRef file0415 = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_oni", "data_oni_armor", 
		"04/15, 2526 [armour unlocked]", "FILES RECORDED", 
		"Operation Castling Long featured the first implementation of Section 3's new Revenant \
		Class armor. Equipped with enhanced stealth capability and a telescopic lensing mechanism \
		built into the headset, the wearer is instantly transformed into a highly effective reconnaissance unit.\
		Awaiting your instructions for deployment.\
		\"ACTIVATE\" button links to Waypoint/in-game, Revenant Class Armor can be equipped on player's character." ));
	
	//file0415->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/15_ONI") );
	file0415->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::MOVIE, "oni_armor_unlock", "scenes/data/video/oni_armor_unlock.mp4", 
		"REVENANT CLASS ARMOR") );
	
	data.push_back( file0415 );


//Operation Castling Long
	DataNodeRef opCastingLong = DataNodeRef( new DataNode(DataNode::DataType::ITEM, "icon_title_oni", "data_ONI", 
		"Operation Castling Long", "FILES RECORDED", 
		"// STRICTLY CONFIDENTIAL. EXAMINATION OR TAMPERING OF THIS OR ANY DOCUMENT \
		CONTAINED HEREIN WITHOUT THE EXPRESS SANCTION OF REAR ADMIRAL EDMUND RICH CONSTITUTES A DIRECT VIOLATION OF UNSC LAW. //\
		\n\n\
		This case file constitutes the remainder of our unredacted data concerning Operation Castling Long. All contents are scheduled \
		for archival pending your final approval. For your and the Directors\' convenience, all relevant media has been organized into the following sections:\
		\n\n\
		I: SUBJECTS OF INTEREST\
		\n\n\
		Featuring profiles of the lead SPARTAN personnel in the op, further information regarding the failed engagement on Nibiru, \
		and what little intel we could gather on the Sangheili sample ODST retrieved from said conflict.\
		\n\n\
		II: FILES RECORDED\
		\n\n\
		Arranged in chronological order, these present inter-command communications regarding the death of LCPL \
		Cadmon Lasky and proposed solutions for diverting his surviving sibling Thomas from active duty. A separate \
		file also contains the private audio-logs of E1R Marcus Sully during his research on SPARTAN Frederic-104.\
		\n\n\
		III: REVENANT CLASS ARMOR TEST RESULTS\
		\n\n\
		The final file contains information from our first field tests using the newly-designed Revenant Class armor. \
		Diagnostics have been run on the augmented active camouflage architecture, and the results are promising.\
		Your expedient reply is of the essence, and as always the department appreciates your discretion.\
		\n\n\
		V/R,\
		\n\n\
		LCDR Mara Yun" ));
	
	opCastingLong->addDataItem( new DataNodeDataItem(DataNodeDataItem::DataItemType::OBJECT3D, "rotation/15_ONI") );
	data.push_back( opCastingLong );


//------------------------------------------
// DEFAULT UNLOCKED ITEMS!?!
//------------------------------------------
	//needler->setAlwaysUnlocked(true);
	hog->setAlwaysUnlocked(true);


//------------------------------------------
// HB Categories
//------------------------------------------


//items
	HBCategoryRef itemsArchive = HBCategoryRef( new HBCategory("ARCHIVE") );
	itemsArchive->addData(sullyVideo); 
	itemsArchive->addData(oni); 
	itemsArchive->addData(forrestWar); 

	HBCategoryRef itemsArmory = HBCategoryRef( new HBCategory("ARMORY") );
	itemsArmory->addData(rifle); 
	itemsArmory->addData(sword); 
	itemsArmory->addData(needler); 

	HBCategoryRef itemsTransport = HBCategoryRef( new HBCategory("TRANSPORT") );
	itemsTransport->addData(corvette); 
	itemsTransport->addData(pod); 
	itemsTransport->addData(teather); 
	itemsTransport->addData(hog); 

	HBCategoryRef itemsMisc = HBCategoryRef( new HBCategory("MISC") );
	itemsMisc->addData(cyto); 
	itemsMisc->addData(letter);
	itemsMisc->addData(hastati); 

	hbItems.push_back(itemsArchive);
	hbItems.push_back(itemsArmory);
	hbItems.push_back(itemsTransport);
	hbItems.push_back(itemsMisc);


//characters
	HBCategoryRef cams = HBCategoryRef( new HBCategory("CAMS") );
	cams->addData(april); 
	cams->addData(chyler); 
	cams->addData(dimah); 
	cams->addData(jj); 
	cams->addData(sully); 
	cams->addData(lasky); 
	cams->addData(vickers); 

	HBCategoryRef unsc = HBCategoryRef( new HBCategory("UNSC") );
	unsc->addData(cadmon); 
	unsc->addData(mammaLasky);
	unsc->addData(mehaffey); 
	unsc->addData(odst); 
	unsc->addData(spartanII); 
	
	HBCategoryRef onicat = HBCategoryRef( new HBCategory("ONI") );
	onicat->addData(huges); 


	hbCharacters.push_back(cams);
	hbCharacters.push_back(unsc);
	hbCharacters.push_back(onicat);
 
 
//oni files
	HBCategoryRef oniBriefing = HBCategoryRef( new HBCategory("BRIEFING") );
	oniBriefing->addData(opCastingLong);

	HBCategoryRef oniSoi = HBCategoryRef( new HBCategory("SUBJECTS OF INTEREST") );
	oniSoi->addData(fred); 
	oniSoi->addData(chief); 
	oniSoi->addData(elite); 
	oniSoi->addData(nibiruEngage); 

	HBCategoryRef oniFiles = HBCategoryRef( new HBCategory("FILES RECORDED") );
	oniFiles->addData(file1001_1130); 
	oniFiles->addData(file1201_0131); 
	oniFiles->addData(file1201_0331); 
	oniFiles->addData(file0415);
	
	hbOniFiles.push_back(oniBriefing);
	hbOniFiles.push_back(oniSoi);
	hbOniFiles.push_back(oniFiles);

}

DataController::~DataController(void)
{
}
