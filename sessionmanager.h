#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_

#include <thread>
#include <list>
#include <jsoncpp/json.h>
#include "msgqueue.h"

class SessionManager {
public:
	SessionManager();
	~SessionManager();

	void registerQueue(MsgQueue<Json::Value>&);

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
	void prepareErrorResponse(std::string&, int);
	void prepareSuccessResponse(std::string&, int);

	/* Clients message queues */
	std::list<MsgQueue<Json::Value>*> clientQueues;
	void dispatchMessage(Json::Value&);		// dispatch a message to all registered queues
};

#endif  /* _SESSIONMANAGER_H_ */
