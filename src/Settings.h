#pragma once
class Settings
{
public:
	Settings(void);
	~Settings(void);

	static const bool debug = true;

	enum ViewID { START, PAUSE, PREFS, HOST, HANDBOOK, DATA, LOCATION, ANALYZE, ENGAGE, PLAY, END, BACK, BEACON };

};

