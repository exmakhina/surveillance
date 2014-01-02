#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "settings.h"
#include "udpsocket.h"
#include "socketexception.h"

using namespace std;

UdpSocket::UdpSocket()
{
	sockHandle = ::socket(AF_INET,SOCK_DGRAM,0);	// UDP socket, Internet domain
	if (sockHandle < 0)
		throw SocketException("Error creating the UDP socket\n");

	/* Set socket to allow broadcast */
	int broadcastPermission = 1;
	if (::setsockopt(sockHandle, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(int) ) < 0)
		throw SocketException("Unable to set socket options.\n");

	/* Construct local address structure */
	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
	broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
	broadcastAddr.sin_addr.s_addr = inet_addr(Settings::instance().getBroadcastIP().c_str());   /* Broadcast IP address */
	broadcastAddr.sin_port = htons(Settings::instance().getBroadcastPort());      /* Broadcast port */
}

UdpSocket::~UdpSocket()
{
	if (sockHandle >= 0)
		::close(sockHandle);
}

const UdpSocket& UdpSocket::operator << (const string& message) const
{
	int sent;

	if (sockHandle >= 0) {
		sent = ::sendto(sockHandle, message.c_str(), message.size(), 0, (sockaddr*)&broadcastAddr, sizeof(sockaddr_in));
	} else
		throw SocketException("Socket not created.\n");

	if (sent < message.size())
		throw SocketException("Error sending data packet.\n");

	return *this;
}
