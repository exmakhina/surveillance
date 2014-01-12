/*
 * survobject.h
 *
 *  Created on: 2014-01-11
 *      Author: tgrimault
 */

#ifndef _APPOBJECT_H_
#define _APPOBJECT_H_

#include <string>

class AppObject
{
public:
	AppObject(std::string const& objName): prettyName(objName) {};
	virtual ~AppObject() {};

	virtual int start() {return 0;};
	virtual int stop() {return 0;};
	virtual void getName(std::string& objName) {objName = prettyName;};

private:
	std::string prettyName;
};


#endif /* _APPOBJECT_H_ */
