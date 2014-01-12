#include <iostream>
#include <chrono>
#include <thread>
#include "application.h"

using namespace std;

int main() 
{
	Application* app;
	chrono::milliseconds TicTac(10000);

	app = new Application();

	while (1) {
		cout << "Main thread running.\n";
		this_thread::sleep_for(TicTac);
	}
	
	delete app;

	return 0;
}
