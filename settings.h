#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <jsoncpp/json.h>
#include <string>

class Settings
{
public:
	static Settings& instance();

	int getCamID();
	int getFPS();
	int getMaxFrames();
	int getThreshold();
	std::string getBroadcastIP();
	int getBroadcastPort();
	std::string getDeviceName();
	int getListenerPort();
	int getMaxAllowedConnections();

private:
	Settings() {};
	Settings(Settings const&);		// make sure default C++ constructor is not implemented
	void operator=(Settings const&); // make sure default = operator is not implemented

	Json::Value config;
	int refreshConfig();
};

#endif /* _SETTINGS_H_ */
