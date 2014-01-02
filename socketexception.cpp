#include <string>
#include "socketexception.h"

using namespace std;

SocketException::SocketException(string desc):
	descriptionMessage(desc)
{
}

string SocketException::description()
{
	return descriptionMessage;
}
