/*
 * application.h
 *
 *  Created on: 2014-01-10
 *      Author: tgrimault
 */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "appobject.h"
#include "sessionmanager.h"

class Application
{
public:
	Application();
	~Application();

private:
	AppObject * motionApp;
	SessionManager * sessionManager;
};


#endif /* _APPLICATION_H_ */
