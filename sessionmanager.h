#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_

#include <thread>

class SessionManager {
public:
	SessionManager();
	~SessionManager();

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
};

#endif  /* _SESSIONMANAGER_H_ */
