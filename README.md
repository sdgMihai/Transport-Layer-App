# Transport-Layer-App
Optimised layer three application code for restricted data types, working with UDP and TCP clients.
// Copyright Gheoace Mihai

Strutura:

Arhiva contine 2 seturi de clase.
	Clasele folosite de server : server.cpp UdpMsg.cpp DataUDP.cpp Print.cpp ssTCPClient.cpp ServerAction.cpp UDPtoTCP.cpp helpers.h
	si cele folosite de client : client.cpp UdpMsg.cpp DataUDP.cpp Print.cpp 
	ssTCPClient.cpp UDPtoTCP.cpp helpers.h

Observam ca doar ServerAction.cpp este intr-o singur set.
Utilizarea acestora:
	server.cpp - contine logica server - ului.
	UdpMsg.cpp si UDPtoTCP.h - contin o clasa folosit in manipularea datelor
		trimise de clientii UDP.
	DataUDP.cpp si DataUDP.h - folosit in extragerea/mainpularea datelor pentru
	stocare eficienta.
	Print.cpp si Print.h - pentru printarea mesajelor la consola si pentru
	future dev. cat si debug.
	ssTCPClient.cpp si ssTCPClient.h - pentru mentinerea informatiei despre clienti.
	ServerAction.cpp si ServerAction.h - pentru incapsularea actiunilor luate
	in server.cpp.
	UDPtoTCP.cpp si UDPtoTCP.h - pentru trimiterea mesajelor de la server, salvate
	intr-un format asemanator cu cel de la UDPMsg(adica DataUDP) catre clientii
	TCP.
	helpers.h - contine cateva constante si informatii necesare pentru a putea modifica lungimea payload  - lui si tipul datelor in toate clasele simultan.
Makefile ul din pachet permite compilarea intregului proiect - prin:
	make
compilarea separata :
	make server
	make client
