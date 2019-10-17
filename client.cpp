#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>

#include "helpers.h"
#include "Print.h"

#define MAX_B_READ 1557  // topic+dataT+nrB+payload+'\0' + 1

void usage(char *file)
{
	fprintf(stderr, "Usage: %s ID_Client IP_Server Port_Server\n", file);
	exit(0);
}



class Client {
 public:
	char id[10];
	int sockfd;
	char buffer[BUFLEN];
	char serverBuff[2 * BUFLEN];
	int sbSize;  // bytes read to serverBuff

	Client(const int sockfd, char* id) : sockfd(sockfd) {
		strcpy(this->id, id);  // set client id
		memset(serverBuff, 0, 2 * BUFLEN);
		sbSize = 0;
	}

	/* Send id to server. */
	void sendId() {
		TcpMsg msg;
		strcpy(msg.topic, "id");
		strcpy(msg.payload, id);
		memcpy(buffer, &msg, sizeof(msg));
		// daca folosesc BUFLEN zice ca sunt doua mesaje
		// pt depasesc nr de caractere care ii plac lui tcp sa trimita
		int n = send(sockfd, buffer, BUFLEN - 1, 0);
		DIE(n < 0, "send");
	}

	/*
	* Sends subscribe request to server.
	* @param1 char* the topic to subscribe to
	* @param2 int if set(= 1) the client doesn't want to lose msgs
	*/
	void subscribe(const char* topic, int sf) {
		TcpMsg msg;
		strcpy(msg.topic, "subscribe");
		strcpy(msg.payload, topic);
		msg.sf = sf;
		memcpy(buffer, &msg, sizeof(msg));
		int n = send(sockfd, buffer, BUFLEN, 0);
		DIE(n < 0, "subscribe");
	}

	/*
	* Informs the server that the client wants to unsubscribe from topic.
	* @param1 char* the topic the client wants to unsubscribe from
	*/
	void unsubscribe(const char* topic) {
		TcpMsg msg;
		strcpy(msg.topic, "unsubscribe");
		strcpy(msg.payload, topic);
		memcpy(buffer, &msg, sizeof(msg));
		int n = send(sockfd, buffer, BUFLEN - 1, 0);
		DIE(n < 0, "unsubscribe");
	}

	/*
	* Closes the sockets and shutdown the client.
	*/
	void exitClient() {
		close(sockfd);
		exit(0);
	}

	void testExit(const char* buff) {
		if (strncmp(buff, "exit", 4) == 0) {
			exitClient();
		}
	}

	/*
	* Parse the input and decide what type of command it is
	*/
	void parseInput(const char* input) {
		std::stringstream ss(input);
		std::string word;
		std::string sf;
		ss >> word >> word;   // get the topic
		if (strncmp(input, "subscribe", 9) == 0) {
			ss >> sf;
			subscribe(word.c_str(), atoi(sf.c_str()));
		} else if (strncmp(input, "unsubscribe", 11) == 0){
			unsubscribe(word.c_str());
		} else {
			testExit(input);
			puts("illegal command issued.");
			puts("Legal commands:");
			puts("subscribe <topic> <sf>");
			puts("unsubscribe <topic>");
		}
	}

	void decodeFlow(const char* initB, int n) {
		if (n == MAX_B_READ) {
			if (initB[n - 1] == 5) {  // the messages are integral
				UDPtoTCP msg;
				memcpy(&msg, initB, n);
				Print::printUDPtoTCP(msg);
				memset(serverBuff, 0, 2 * BUFLEN);
				sbSize = 0;
			} else {  // there are bytes not yet here
				strcat(serverBuff, initB);
				sbSize += n;
			}
		} else {  //  a single msg is here
			UDPtoTCP msg;
			memcpy(&msg, initB, n);
			Print::printUDPtoTCP(msg);
			memset(serverBuff, 0, 2 * BUFLEN);
			sbSize = 0;
		}
	}
};

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];
	int i;
	int fdmax = 0;

	fd_set read_fds;	// reading set used in select()
	fd_set tmp_fds;		// temporary reading set
	
	if (argc < 4) {  // test corect client usage
		usage(argv[0]);
	}

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	FD_SET(STDIN_FILENO, &read_fds);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");
	printf("Creez conexiune pe %s, port %d, socket server %d\n",
							inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port), ret);

	FD_SET(sockfd, &read_fds);
	if (sockfd > fdmax) {
		fdmax = sockfd;
	}

	Client client(sockfd, argv[1]);
	client.sendId();

	while (1) {
		tmp_fds = read_fds; 
		
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if (i == sockfd) {
					memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, MAX_B_READ + sizeof(struct sockaddr_in), 0);
					DIE(n < 0, "recv");
					if (strncmp(buffer, "exit", 4) == 0) {
						close(sockfd);
						exit(0);
					} else {
						client.decodeFlow(buffer, n);
					}

				} else if (i == STDIN_FILENO){
					// reading from stdin
					memset(buffer, 0, BUFLEN);
					fgets(buffer, BUFLEN - 1, stdin);
					client.parseInput(buffer);
				} else {
					printf("This case must not exist\n");
					client.exitClient();
				}

			}
		}
	}

	close(sockfd);  // it mustn't get here
	// rather closes in while
	return 0;
}

/* va apare un mesaj singuratic la final intotdeauna
* pt activare FD_ISSET(STDIN_FILENO, &tmp_fds) == 1
* va fi intotdeauna inaintea acestei afisari.
* deci cu afisarea dinaintea  while - ului 
* va fi intotdeauna una in plus
*/
