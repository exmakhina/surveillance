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
