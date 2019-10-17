# Protocoale de comunicatii:
# Tema 2 : Gheoace Mihai 324CA
# Makefile

CFLAGS = -Wall -g

# Portul pe care asculta serverul (de completat)
PORT = 3001

# Adresa IP a serverului (de completat)
IP_SERVER = 127.0.0.1

all: server client

# Compileaza server.c
server: server.cpp UdpMsg.cpp DataUDP.cpp Print.cpp ssTCPClient.cpp ServerAction.cpp UDPtoTCP.cpp
	g++ ${CFLAGS} server.cpp UdpMsg.cpp DataUDP.cpp Print.cpp ssTCPClient.cpp ServerAction.cpp UDPtoTCP.cpp -o server
# Compileaza client.c
# este nevoie de ssTCPClient. nu compileaza fara
client: client.cpp UdpMsg.cpp DataUDP.cpp Print.cpp UDPtoTCP.cpp ssTCPClient.cpp
	g++ ${CFLAGS} client.cpp UdpMsg.cpp DataUDP.cpp Print.cpp UDPtoTCP.cpp ssTCPClient.cpp -o client
test: test.cpp
	g++ ${CFLAGS} test.cpp -o test
.PHONY: clean run_server run_client

# Ruleaza serverul
run_server:
	./server ${PORT}

# Ruleaza clientul
run_client:
	./client ${IP_SERVER} ${PORT}

clean:
	rm -f server client
