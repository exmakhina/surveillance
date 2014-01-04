#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <jsoncpp/json.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "udpsocket.h"
#include "tcpsocket.h"
#include "socketexception.h"
#include "sessionmanager.h"
#include "settings.h"

using namespace std;

SessionManager::SessionManager():
		stopAdvertising(false),
		stopListening(false)
{
	advertising = new thread(advertisingLauncher, this);
	listener = new thread(listenerLauncher, this);
}

SessionManager::~SessionManager()
{
	stopAdvertising = true;
	stopListening = true;

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
	vector<string> IPAddr;

	broadcastMessage["DeviceName"] = Settings::instance().getDeviceName();
	broadcastMessage["ListenerPort"] = Settings::instance().getListenerPort();
	udpSocket.getIPs(IPAddr);
	for (int i=0; i<IPAddr.size(); i++) {
		broadcastMessage["ListenerIP"][to_string(i)] = IPAddr[i];		//TODO: figure out what's best: [tostring(i)] vs [i]
	}

	while (!stopAdvertising) {
		try {
			udpSocket << broadcastMessage.toStyledString();
		}
		catch (SocketException& e) {
			stopAdvertising = true;
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
	TcpSocket serverSocket;
	TcpSocket clientSocket;
	string request, response;

	cout << "Wait for a connection from a host.\n";
	try {
		serverSocket.create();
	}
	catch (SocketException& e) {
		stopListening = true;
		cout << e.description();
	}

	while (!stopListening) {
		try {
			serverSocket.accept(clientSocket);
		}
		catch (SocketException& e) {
			stopListening = true;
			cout << e.description();
		}

		while(!stopListening) {
			// Waiting for a request
			cout << "Wait for a request from the host.\n";
			try {
				clientSocket >> request;
			}
			catch (SocketException& e) {
				stopListening = true;
				cout << e.description();
				break;
			}
			cout << "Received: " << request << "\n";

			// Prepare and send a response
			response = "OK";
			try {
				clientSocket << response;
			}
			catch (SocketException& e) {
				stopListening = true;
				cout << e.description();
			}
			cout << "Response: " << response << " sent successfully.\n";
		}
	}
}
