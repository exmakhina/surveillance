#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <jsoncpp/json.h>

class Settings
{
public:
	static Settings& instance();

	int getCamID();
	int getFPS();
	int getMaxFrames();
	int getThreshold();

private:
	Settings() {};
	Settings(Settings const&);		// make sure default C++ constructor is not implemented
	void operator=(Settings const&); // make sure default = operator is not implemented

	Json::Value config;
	int refreshConfig();
};

#endif /* _SETTINGS_H_ */
