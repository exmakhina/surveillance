#include <thread>
#include <iostream>
#include <chrono>
#include <jsoncpp/json.h>
#include "udpsocket.h"
#include "socketexception.h"
#include "sessionmanager.h"
#include "settings.h"

using namespace std;

SessionManager::SessionManager():
		stopAdvertising(false),
		stopListennig(false)
{
	advertising = new thread(advertisingLauncher, this);
	listener = new thread(listenerLauncher, this);
}

SessionManager::~SessionManager()
{
	stopAdvertising = true;
	stopListennig = true;

	advertising->join();
	listener->join();

	delete advertising;
	delete listener;
}

void SessionManager::advertisingLauncher(void* instance)
{
	static_cast<SessionManager*>(instance)->advertisingThread();
}

void SessionManager::advertisingThread()
{
	UdpSocket udpSocket;
	Json::Value broadcastMessage;
	chrono::milliseconds TicTac( 1000 );	// 1s

	broadcastMessage["DeviceName"] = Settings::instance().getDeviceName();
	broadcastMessage["ListenerPort"] = 1000;
	broadcastMessage["ListenerIP"] = "192.168.x.x";

	while (!stopAdvertising) {
		try {
			udpSocket << broadcastMessage.toStyledString();
		}
		catch (SocketException& e) {
			cout << e.description();
		}

		this_thread::sleep_for( TicTac );
	}
}

void SessionManager::listenerLauncher(void* instance)
{
	static_cast<SessionManager*>(instance)->listenerThread();
}

void SessionManager::listenerThread()
{
	chrono::milliseconds TicTac( 10000 );	// 10s

	while (!stopListennig) {
		cout << "Listener working.\n";

		this_thread::sleep_for(TicTac);
	}
}
