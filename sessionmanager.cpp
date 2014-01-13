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
#include "appobject.h"

using namespace std;

SessionManager::SessionManager():
		stopAdvertising(false),
		stopListening(false),
		running(true)
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

	clientAppList.clear();

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

	running = false;
	cout << "SessionManager advertising thread stopped.\n";
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

	try {
		serverSocket.create();
	}
	catch (SocketException& e) {
		stopListening = true;
		cout << e.description();
	}

	while (!stopListening) {
		cout << "Wait for a connection from a host.\n";
		try {
			serverSocket.accept(clientSocket);
		}
		catch (SocketException& e) {
			cout << e.description();
			break;
		}

		while(!stopListening) {
			// Waiting for a request
			try {
				clientSocket >> request;
			}
			catch (SocketException& e) {
				cout << e.description();
				break;
			}

			processMessage(request, response);

			try {
				clientSocket << response;
			}
			catch (SocketException& e) {
				cout << e.description();
				break;
			}
		}   /* while (!stopListening) */
		clientSocket.close();
	}  /* while (!stopListening) */

	serverSocket.close();
	running = false;
	cout << "SessionManager listener thread stopped.\n";
}

void SessionManager::processMessage(string& request, string& response)
{
	Json::Value requestMessage, requestContent;
	Json::Reader messageParser;
	bool res;

	res = messageParser.parse(request, requestMessage);
	if (res) {
		if (requestMessage.isMember("Request")) {
			requestContent = requestMessage["Request"];
			if (requestContent.isMember("Command")) {
				handleRequest(requestContent["Command"].asInt(), response);
			}
		}
	}
	/* else ... return the originally formatted error response */
}

void SessionManager::handleRequest(int request, std::string& response)
{
	list<AppObject*>::const_iterator it;
	int status;
	Json::Value responseObject;
	string applicationName;
	string requestName;

	for (it=clientAppList.begin(); it!=clientAppList.end(); ++it) {
		switch (request) {
		case SessionManager::Start:
			requestName = "Start";
			status = (*it)->start();
			break;
		case SessionManager::Stop:
			requestName = "Stop";
			status = (*it)->stop();
			break;
		case SessionManager::Kill:
			requestName = "Kill";
			/* Stop application and Session Manager threads */
			status = (*it)->stop();
			stopListening = true;
			stopAdvertising = true;
			break;
		default:
			requestName = "Unknown";
			status = -1;
		}

		if (status < 0) {
			// fill the response object and exit on error
			(*it)->getName(applicationName);
			responseObject["Response"]["Application"] = applicationName;
			responseObject["Response"]["Request"] = requestName;
			responseObject["Response"]["Status"] = "error";
			responseObject["Response"]["Code"] = status;
			response = responseObject.toStyledString();
			return;
		}
	}

	// all start() calls succeeded
	responseObject["Response"]["Application"] = "all";
	responseObject["Response"]["Request"] = requestName;
	responseObject["Response"]["Status"] = "ok";
	responseObject["Response"]["Code"] = 0;
	response = responseObject.toStyledString();
}

void SessionManager::registerClient(AppObject* object)
{
	if (object != NULL)
		clientAppList.push_back(object);
}

bool SessionManager::isRunning()
{
	return running;
}
