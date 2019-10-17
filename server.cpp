#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <vector>

#include <list>

#include "helpers.h"
#include "UdpMsg.h"
#include "DataUDP.h"
#include "Print.h"
#include "TcpMsg.h"
#include "ServerAction.h"
#include "ssTCPClient.h"

void usage(char *file) {
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, udpfd;
	char buffer[BUFLEN];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, ret;
	socklen_t clilen;
	Print print;
	TCPClients cli;
	ServerAction sAction;

	fd_set read_fds;	// multimea de citire folosita in select()
	fd_set tmp_fds;		// multime folosita temporar
	int fdmax = 0;			// valoare maxima fd din multimea read_fds

	if (argc < 2) {
		usage(argv[0]);
	}

	// se goleste multimea de descriptori de citire (read_fds) si multimea temporara (tmp_fds)
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	FD_SET(STDIN_FILENO, &read_fds);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	portno = atoi(argv[1]);
	DIE(portno == 0, "atoi");

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	int yes = 1;
	/* TODO legare proprietăți de socket */
	if (setsockopt(sockfd,
			SOL_TCP,
			TCP_NODELAY,
			&yes,
			sizeof(int)) < 0) {
			perror("setsockopt(SO_REUSEADDR) failed");
	}

	ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");

	ret = listen(sockfd, MAX_CLIENTS);
	DIE(ret < 0, "listen");

	// se adauga noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
	FD_SET(sockfd, &read_fds);
	if (sockfd > fdmax) {
		fdmax = sockfd;
	}

	/* create UDP socket */ 
	if ( (udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	}
	// binding server addr structure to udp sockfd 
	bind(udpfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	// add udpListenSocket to set
	FD_SET(udpfd, &read_fds);
	if (udpfd > fdmax) {
		fdmax = udpfd;
	}

	while (1) {
		tmp_fds = read_fds; 
		
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if (i == sockfd) {
					// a venit o cerere de conexiune pe socketul inactiv (cel cu listen),
					// pe care serverul o accepta
					clilen = sizeof(cli_addr);
					newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
					DIE(newsockfd < 0, "accept");

					// se adauga noul socket intors de accept() la multimea descriptorilor de citire
					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fdmax) { 
						fdmax = newsockfd;
					}
					// save new client info
					cli.addr.push_back(cli_addr);
					cli.sockfd.push_back(newsockfd);
				} else if (i == udpfd) {
						clilen = sizeof(cli_addr);
						UdpMsg myUdpMsg;
						n = recvfrom(udpfd, (char *)&myUdpMsg, BUFLEN - 1, 
									0, ( struct sockaddr *) &cli_addr, 
									(socklen_t *)&clilen);
						myUdpMsg.pSize =
							n - TOPIC_SIZE - sizeof(myUdpMsg.data_type);
						// the payload may not be '\0' ended
						myUdpMsg.payload[n - sizeof(myUdpMsg.topic) - sizeof(myUdpMsg.data_type)] = '\0';
						if (sAction.putMsg(myUdpMsg, cli_addr)) {
							// send msg to all subscribers
							sAction.sendMsg(myUdpMsg.topic);
						}
				}else if (i != STDIN_FILENO) {
					// messages are here
					memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, BUFLEN, 0);
					DIE(n < 0, "recv");
					if (n == 0) {
						// conexiunea s-a inchis
						printf("Client %s disconnected.\n", sAction.clientMap[i].id);
						sAction.clientMap[i].isOnline = false;
						close(i);
						
						// se scoate din multimea de citire socketul inchis 
						FD_CLR(i, &read_fds);
					} else {
						if (strncmp(buffer, "id", 2) == 0) {  // is a id msg
							DIE(i != (int) cli.sockfd.front(), "id msg receive");

							sAction.putClient(buffer, cli, i);

							print.newTCPConnection(sAction, i);
						} else {
							if (strcmp(buffer, "subscribe") == 0) {
								sAction.subscribe(i, buffer);
							} else if (strcmp(buffer, "unsubscribe") == 0){
								sAction.unsubscribe(i, buffer);
							}
						}
					}
				} else {  // Server close
					fgets(buffer, BUFLEN - 1, stdin);
					if (strncmp(buffer, "exit", 4) == 0) {  //send exit msg to all clients
						for (int j = 0; j <= fdmax; ++j) {
							if (FD_ISSET(j, &read_fds)) {
								if (j != sockfd && j != STDIN_FILENO && j != udpfd) {
									if (send(j, buffer, BUFLEN, 0) < 0) {
										printf("Eroare trimitere mesaj inchidere.\n");
									}
								} 
							}
						}
						close(sockfd);
						exit(0);
					} else {
						printf("Illegal server command.\n");
					}
				}
			}
		}
	}

	close(sockfd);
	return 0;
}
