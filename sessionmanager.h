#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_

#include <thread>
#include <list>
#include <jsoncpp/json.h>
#include "appobject.h"

class SessionManager {
public:
	SessionManager();
	~SessionManager();

	void registerClient(AppObject*);

private:
	/* Messages */
	enum {
		Start = 0,
		Stop,
		Kill
	};

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

	/* Message processing */
	void processMessage(std::string&, std::string&);
	void handleRequest(int, std::string&);	// request=SessionManager::enum value, string -> response to the host

	/* Clients applications */
	std::list<AppObject*> clientAppList;
};

#endif  /* _SESSIONMANAGER_H_ */
