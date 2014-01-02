#ifndef _SOCKETEXCEPTION_H_
#define _SOCKETEXCEPTION_H_

#include <string>

class SocketException {
public:
	SocketException(std::string);
	~SocketException() {};

	std::string description();
private:
	std::string descriptionMessage;
};

#endif   /* _SOCKETEXCEPTION_H_ */
