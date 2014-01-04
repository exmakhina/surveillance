#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "settings.h"
#include "tcpsocket.h"
#include "socketexception.h"

using namespace std;

TcpSocket::TcpSocket():
		sockHandle(-1)
{
	memset(&sockAddr, 0, sizeof(sockaddr_in));
}

TcpSocket::~TcpSocket()
{
	if (sockHandle >= 0)
		::close(sockHandle);
}

void TcpSocket::create()
{
	if (sockHandle >= 0)
		throw SocketException("Socket already initialized.\n");

	sockHandle = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sockHandle < 0)
		throw SocketException("Unable to create a TCP socket.\n");

	/* Set socket to bind to an existing IP/PORT combination */
	int reuseAddrOpt = 1;
	if (::setsockopt(sockHandle, SOL_SOCKET, SO_REUSEADDR, (void *) &reuseAddrOpt, sizeof(int) ) < 0)
		throw SocketException("Unable to set socket options.\n");

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	sockAddr.sin_port=htons(Settings::instance().getListenerPort());
	if (::bind(sockHandle, (sockaddr *)&sockAddr, sizeof(sockaddr)) < 0)
		throw SocketException("Unable to bind the socket the the IP:PORT.\n");

	/* Starts listening to any connection attempts */
	if (::listen(sockHandle, Settings::instance().getMaxAllowedConnections()) < 0)
		throw SocketException("Unable to start listening to upcoming connections.\n");
}

void TcpSocket::accept(TcpSocket& newClient)
{
	int newSocketHandle;
	socklen_t addrLength = sizeof(sockaddr);

	newSocketHandle = ::accept(sockHandle, (sockaddr*)&sockAddr, &addrLength);
	if (newSocketHandle < 0)
		throw SocketException("Error while accepting a new TCP connection.\n");

	newClient.setHandle(newSocketHandle);
}

void TcpSocket::connect(string& IP, int port)
{
	if (sockHandle >= 0)
		throw SocketException("Socket already initialized.\n");

	sockHandle = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sockHandle < 0)
		throw SocketException("Unable to create a TCP socket.\n");

	/* Set socket to bind to an existing IP/PORT combination */
	int reuseAddrOpt = 1;
	if (::setsockopt(sockHandle, SOL_SOCKET, SO_REUSEADDR, (void *) &reuseAddrOpt, sizeof(int) ) < 0)
		throw SocketException("Unable to set socket options.\n");

	if (inet_pton(AF_INET, IP.c_str(), &sockAddr.sin_addr) != 1)
		throw SocketException("Could convert IP address to a sockaddr structure.\n");

	if (::connect(sockHandle, (sockaddr*)&sockAddr, sizeof(sockaddr)) < 0)
		throw SocketException("Cound not connect to the server socket.\n");
}

void TcpSocket::setHandle(int newHandle)
{
	if (sockHandle < 0)
		sockHandle = newHandle;
	else
		throw SocketException("Attempt to change a socket handle which is already initialized.\n");
}

const TcpSocket& TcpSocket::operator << (const string& message) const
{
	if (sockHandle >= 0) {
		if (::send(sockHandle, message.c_str(), message.size(), 0) < message.size())
			throw SocketException("Error while transmitting message.\n");
	} else
		throw SocketException("Socket not initialized.\n");

	return *this;
}

const TcpSocket& TcpSocket::operator >> (string& message) const
{
	if (sockHandle >= 0) {
		char buf[1024];
		int res;

		res = ::recv(sockHandle, buf, 1024, 0);
		if (res > 0)
			message = buf;
		else
			throw SocketException("Received 0 bytes on TCP socket.\n");
	} else
		throw SocketException("Socket not initialized.\n");

	return *this;
}
