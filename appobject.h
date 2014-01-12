/*
 * survobject.h
 *
 *  Created on: 2014-01-11
 *      Author: tgrimault
 */

#ifndef _APPOBJECT_H_
#define _APPOBJECT_H_

class AppObject
{
public:
	AppObject() {};
	virtual ~AppObject() {};

	virtual int start() {return 0;};
	virtual int stop() {return 0;};
};


#endif /* _APPOBJECT_H_ */
