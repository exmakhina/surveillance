#include <iostream>
#include <chrono>
#include "motionapp.h"

using namespace std;

int main() 
{
	chrono::milliseconds TicTac( 5000 );	// 5s
	MotionApp * app;
	
	app = new MotionApp();
	app->start();
	
	while (1) {
		// nothing else to but sleeping...
		cout << "Surveillance in progress ...\n";
		this_thread::sleep_for( TicTac );
	}
	
	return 0;
}
