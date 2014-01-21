#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_

#include <thread>
#include <list>
#include <jsoncpp/json.h>
#include "appobject.h"
#include "connector.h"

class SessionManager {
public:
	SessionManager();
	~SessionManager();

	void registerClient(AppObject*);
	bool isRunning();

private:
	bool running;

	/* advertising thread: broadcast an advertisement message over UDP */
	std::thread* advertising;
	static void advertisingLauncher(void *);
	void advertisingThread();
	bool stopAdvertising;

	/* listener thread: listen to the TCP port */
	std::thread* listener;
	static void listenerLauncher(void *);
	void listenerThread();
	bool stopListening;
	std::list<Connector*> connectorList;

	/* Clients applications */
	AppObject* clientApp;
};

#endif  /* _SESSIONMANAGER_H_ */
