#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class TcpSocket {
public:
	TcpSocket();
	~TcpSocket();

	const TcpSocket& operator << (const std::string&) const;
	const TcpSocket& operator >> (std::string&) const;

	void create();
	void accept(TcpSocket&);
	void connect(std::string&, int); 	// Connect to a TCP IP:Port socket
	void close();
	void setHandle(int);	// set handle of a new created object - Does not work if the current handle >= 0

private:
	int sockHandle;
	sockaddr_in sockAddr; /* TCP socket server address */
};

#endif /* _TCPSOCKET_H_ */
