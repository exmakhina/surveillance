/*
 * application.cpp
 *
 *  Created on: 2014-01-10
 *      Author: tgrimault
 */

#include <iostream>
#include "appobject.h"
#include "motionapp.h"
#include "sessionmanager.h"
#include "application.h"

using namespace std;

Application::Application()
{
	sessionManager = new SessionManager();
	motionApp = new MotionApp();

	sessionManager->registerClient(motionApp);
}

Application::~Application()
{
	delete sessionManager;
	delete motionApp;
}
