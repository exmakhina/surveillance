#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class UdpSocket {
public:
	UdpSocket();
	~UdpSocket();

	const UdpSocket& operator << (const std::string&) const;

private:
	int sockHandle;
	sockaddr_in broadcastAddr; /* Broadcast address */
};

#endif /* _UDPSOCKET_H_ */
