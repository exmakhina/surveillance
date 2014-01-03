#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <vector>

class UdpSocket {
public:
	UdpSocket();
	~UdpSocket();

	const UdpSocket& operator << (const std::string&) const;
	void getIPs(std::vector<std::string>&) const;

private:
	int sockHandle;
	sockaddr_in broadcastAddr; /* Broadcast address */
	std::vector<std::string> localIPs;	/* list of all IP addresses attached to local adaptors */
};

#endif /* _UDPSOCKET_H_ */
