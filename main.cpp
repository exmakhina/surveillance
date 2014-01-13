#include <iostream>
#include <chrono>
#include <thread>
#include "appobject.h"
#include "motionapp.h"
#include "sessionmanager.h"

using namespace std;

int main() 
{
	chrono::milliseconds TicTac(10000);
	AppObject * motionApp;
	SessionManager * sessionManager;

	cout << "Motion Application with Session Manager started.\n";

	sessionManager = new SessionManager();
	motionApp = new MotionApp();

	sessionManager->registerClient(motionApp);

	while (sessionManager->isRunning()) {
		cout << "Main thread running.\n";
		this_thread::sleep_for(TicTac);
	}
	
	delete sessionManager;
	delete motionApp;

	cout << "Main application stopped. Exiting.\n";

	return 0;
}
