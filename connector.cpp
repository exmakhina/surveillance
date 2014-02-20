#include <thread>
#include <string>
#include <jsoncpp/json.h>
#include <iostream>
#include "tcpsocket.h"
#include "connector.h"
#include "socketexception.h"
#include "appobject.h"

using namespace std;

Connector::Connector(AppObject* app, TcpSocket& socket):
		stopListening(false),
		clientApp(app),
		tcpSocket(socket)
{
	listener = new thread(listenerLauncher, this);
}

Connector::~Connector()
{
	stopListening = true;
	listener->join();
	delete listener;
}

void Connector::listenerLauncher(void* instance)
{
	static_cast<Connector*>(instance)->listenerThread();
}

void Connector::listenerThread()
{
	string request, response;

	while(!stopListening) {
		// Waiting for a request
		try {
			tcpSocket >> request;
		}
		catch (SocketException& e) {
			cout << e.description();
			break;
		}

		processMessage(request, response);

		try {
			tcpSocket << response;
		}
		catch (SocketException& e) {
			cout << e.description();
			break;
		}
	}   /* while (!stopListening) */

	tcpSocket.close();
}

void Connector::processMessage(string& request, string& response)
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

void Connector::handleRequest(int request, std::string& response)
{
	int status = 0;
	Json::Value responseObject;
	string applicationName;
	string requestName;

	switch (request) {
	case Connector::Start:
		requestName = "Start";
		clientApp->start();
		break;
	case Connector::Stop:
		requestName = "Stop";
		clientApp->stop();
		break;
	case Connector::Kill:
		requestName = "Kill";
		/* Stop application and Session Manager threads */
		clientApp->stop();
		stopListening = true;
		break;
	default:
		requestName = "Unknown";
		status = -1;
	}

	if (status < 0) {
		// fill the response object and exit on error
		clientApp->getName(applicationName);
		responseObject["Response"]["Application"] = applicationName;
		responseObject["Response"]["Request"] = requestName;
		responseObject["Response"]["Status"] = "error";
		responseObject["Response"]["Code"] = status;
		response = responseObject.toStyledString();
		return;
	}

	// all start() calls succeeded
	responseObject["Response"]["Application"] = "all";
	responseObject["Response"]["Request"] = requestName;
	responseObject["Response"]["Status"] = "ok";
	responseObject["Response"]["Code"] = 0;
	response = responseObject.toStyledString();
}

