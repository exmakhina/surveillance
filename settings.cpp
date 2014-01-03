#include <jsoncpp/json.h>
#include <fstream>
#include <iostream>
#include <string>
#include "settings.h"

using namespace std;

// Default values if no config file is found
#define CAM_ID		0
#define FPS			30		// capture speed in fps
#define MAX_FRAMES	30		// circular buffer depth
#define THRESHOLD 	30		// detector threshold
#define BROADCASTIP "192.168.255.255"		// default broadcast address
#define BROADCASTPORT 1976		// default broadcast port
#define DEVICENAME "Thomas_Eye"		// Default device name
#define LISTENERPORT 2008		// Default port for the TCP socket server
#define SERVERMAXCONNECTIONS 128  // Default maximum number of connections to the TCP server

// Private methods
int Settings::refreshConfig()
{
	ifstream configFile;

	configFile.open("settings.json");
	if (configFile.is_open()) {
		// config file exists, read it
		string configContent;
		Json::Reader reader;
		bool res;

		configFile.seekg(0, ios::end);
		int length = configFile.tellg();
		configContent.resize(length);
		configFile.seekg(0, ios::beg);
		configFile.read(&configContent[0], length);
		configFile.close();

		res = reader.parse(configContent, config);
		if (!res) {
			cout << "Error parsing the JSON configuration file\n";
			return -1;
		}
	} else {
		// reset config with default values and write the config file
		ofstream newConfigFile;

		config["camID"] = CAM_ID;
		config["FPS"] = FPS;
		config["MAX_FRAMES"] = MAX_FRAMES;
		config["THERSHOLD"] = THRESHOLD;
		config["BROADCASTIP"] = BROADCASTIP;
		config["BROADCASTPORT"] = BROADCASTPORT;
		config["DEVICENAME"] = DEVICENAME;
		config["LISTENERPORT"] = LISTENERPORT;
		config["SERVERMAXCONNECTIONS"] = SERVERMAXCONNECTIONS;

		newConfigFile.open("settings.json");
		if (!newConfigFile.is_open()) {
			cout << "Error creating a new settings.json file.\n";
			return -1;
		}

		newConfigFile << config.toStyledString();
		newConfigFile.close();
	}
	return 0;
}

// Public API
Settings& Settings::instance()
{
	static Settings _settings;
	return _settings;
}

int Settings::getCamID()
{
	if (!config.isMember("camID")) {
		refreshConfig();
	}

	return config["camID"].asInt();
}

int Settings::getFPS()
{
	if (!config.isMember("FPS")) {
		refreshConfig();
	}

	return config["FPS"].asInt();
}

int Settings::getMaxFrames()
{
	if (!config.isMember("MAX_FRAMES")) {
		refreshConfig();
	}

	return config["MAX_FRAMES"].asInt();
}

int Settings::getThreshold()
{
	if (!config.isMember("THRESHOLD")) {
		refreshConfig();
	}

	return config["THRESHOLD"].asInt();
}

string Settings::getBroadcastIP()
{
	if (!config.isMember("BROADCASTIP")) {
		refreshConfig();
	}

	return config["BROADCASTIP"].asString();
}

int Settings::getBroadcastPort()
{
	if (!config.isMember("BROADCASTPORT")) {
		refreshConfig();
	}

	return config["BROADCASTPORT"].asInt();
}

string Settings::getDeviceName()
{
	if (!config.isMember("DEVICENAME")) {
		refreshConfig();
	}

	return config["DEVICENAME"].asString();
}

int Settings::getListenerPort()
{
	if (!config.isMember("LISTENERPORT")) {
		refreshConfig();
	}

	return config["LISTENERPORT"].asInt();
}

int Settings::getMaxAllowedConnections()
{
	if (!config.isMember("SERVERMAXCONNECTIONS")) {
		refreshConfig();
	}

	return config["SERVERMAXCONNECTIONS"].asInt();
}
