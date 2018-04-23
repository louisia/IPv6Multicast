
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<iostream>
using namespace std;
 
int
main(int argc, char *argv[])
{
	struct sockaddr_in6 saddr, maddr;
	struct ipv6_mreq mreq;
	char buf[1400];
	int sd,on = 1, hops = 255;



	if (argc < 3) {
		cout<<"Usage Example: ./recv ff02::5:6 12345"<<endl;
		return 1;
	}

	sd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (sd < 0) {
		cout<<"socket"<<endl;;
		return 1;
	}
 
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
		cout<<"setsockopt"<<endl;;
		return 1;
	}


	if (setsockopt(sd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops, sizeof(hops))) {
		cout<<"setsockopt"<<endl;;
		return 1;
	}

 
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin6_family = AF_INET6;
	saddr.sin6_port = htons(atoi(argv[2]));
	saddr.sin6_addr = in6addr_any;
 
	if (bind(sd, (struct sockaddr *) &saddr, sizeof(saddr))) {
		cout<<"bind"<<endl;
		return 1;
	}
 
	memset(&maddr, 0, sizeof(maddr));
	inet_pton(AF_INET6, argv[1], &maddr.sin6_addr);

	memcpy(&mreq.ipv6mr_multiaddr, &maddr.sin6_addr, sizeof(mreq.ipv6mr_multiaddr));
	mreq.ipv6mr_interface = 0;
	if (setsockopt(sd, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *) &mreq, sizeof(mreq))) {
		cout<<"setsockopt"<<endl;;
		return 1;
	}

	

	
	while (1) {
		
		socklen_t addrlen = sizeof(saddr);

		int len=recvfrom(sd, buf, sizeof(buf),
				0, (struct sockaddr *) &saddr, &addrlen);
		buf[len]='\0';

		cout<<buf<<endl;
	}

	close(sd);


	return 0;
}
