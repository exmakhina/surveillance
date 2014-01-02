#include <iostream>
#include <chrono>
#include "motionapp.h"
#include "sessionmanager.h"

using namespace std;

int main() 
{
	chrono::milliseconds TicTac( 5000 );	// 5s
	MotionApp * app;
	SessionManager * sm;
	
	app = new MotionApp();
	app->start();

	sm = new SessionManager();

	while (1) {
		// nothing else to but sleeping...
		cout << "Surveillance in progress ...\n";
		this_thread::sleep_for( TicTac );
	}
	
	delete app;
	delete sm;

	return 0;
}
