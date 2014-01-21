#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <thread>
#include "tcpsocket.h"
#include "appobject.h"

class Connector
{
public:
	Connector(AppObject*, TcpSocket&);
	~Connector();

private:
	/* Messages */
	enum {
		Start = 0,
		Stop,
		Kill
	};

	TcpSocket tcpSocket;
	std::thread* listener;
	static void listenerLauncher(void*);
	void listenerThread();
	bool stopListening;

	/* Message processing */
	AppObject* clientApp;
	void processMessage(std::string&, std::string&);
	void handleRequest(int, std::string&);	// request=SessionManager::enum value, string -> response to the host
};

#endif  /* _CONNECTOR_H_ */
