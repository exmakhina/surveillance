#include <iostream>
#include "motionapp.h"

int main() 
{
	MotionApp * app;
	
	app = new MotionApp();
	app->start();
	
	while (1);
	
	return 0;
}
