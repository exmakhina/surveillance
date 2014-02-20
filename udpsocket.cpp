#include <string>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
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

	/* Find out the local IP address */
	ifaddrs *ifap, *ifa;
	sockaddr_in* sa;
	char addrbuf[20];						// sizeof("xxx.xxx.xxx.xxx") < 20

	getifaddrs (&ifap);
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET) {	// only care about IPv4 adaptors
			sa = (sockaddr_in*)ifa->ifa_addr;
			inet_ntop(AF_INET, &sa->sin_addr, addrbuf, sizeof(addrbuf));
			localIPs.push_back(string(addrbuf));
		}
	}
}

UdpSocket::~UdpSocket()
{
	if (sockHandle >= 0)
		::shutdown(sockHandle, SHUT_RDWR);
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

void UdpSocket::getIPs(vector<string>& IPs) const
{
	IPs = localIPs;
}
