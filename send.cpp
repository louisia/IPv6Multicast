#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

int
main(int argc, char *argv[])
{
	struct sockaddr_in6 saddr;
	char buf[1400]="hello";
	ssize_t len = 1;
	int sd;

	if (argc < 3) {
		cout<<"Usage Example: ./send ff02::5:6 12345"<<endl;;
		return 1;
	}

	sd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (sd < 0) {
		cout<<"socket"<<endl;
		return 1;
	}



	memset(&saddr, 0, sizeof(struct sockaddr_in6));
	saddr.sin6_family = AF_INET6;
	saddr.sin6_port = htons(atoi(argv[2]));
	inet_pton(AF_INET6, argv[1], &saddr.sin6_addr);


	while (1) {

		sendto(sd, buf, sizeof(buf), 0, (const struct sockaddr *) &saddr, sizeof(saddr));
		sleep(5); 


	}

	close(sd);


	return 0;
}
