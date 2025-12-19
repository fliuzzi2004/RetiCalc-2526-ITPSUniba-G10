#include <winsock.h>
#include <stdio.h>
#include <string.h>

#define ECHOMAX 255

void ErrorHandler(char *errorMessage) {
	printf(errorMessage);
}

void ClearWinSock() {
	WSACleanup();
}

int main() {
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSASturtup\n");
		return EXIT_FAILURE;
	}

	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	unsigned int fromSize;
	char echoString[ECHOMAX] = "Hello";
	char echoBuffer[ECHOMAX];
	int echoStringLen;
	int respStringLen;
	char HostName[ECHOMAX];
	char HostIP[ECHOMAX];
	int Port;
	struct in_addr* ina;

	printf("Inserisci nome host: ");
	scanf("%s", HostName);

	printf("Inserisci porta host: ");
	scanf("%d", &Port);

	if ((echoStringLen = strlen(echoString)) > ECHOMAX)
		ErrorHandler("echo word too long");

	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		ErrorHandler("socket() failed");

	struct hostent *host;
	host = gethostbyname(HostName);
	if (host == NULL) {
		fprintf(stderr, "gethostbyname() failed.\n");
		exit(1);
	} else {
		struct in_addr* ina = (struct in_addr*) host->h_addr_list[0];
		strcpy(HostIP, inet_ntoa(*ina));
	}

	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	echoServAddr.sin_port = htons(Port);
	echoServAddr.sin_addr.s_addr = inet_addr(HostIP);

	// PRIMO INVIO: "Hello"
	echoStringLen = strlen(echoString);
	sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr));

	fromSize = sizeof(fromAddr);
	recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize);
	// Non la stampiamo se non ti serve

	// SECONDO INVIO: Messaggio da input
	printf("Inserisci la stringa echo da inviare al server: ");
	scanf("%s", echoString);
	echoStringLen = strlen(echoString); // Ricalcola la lunghezza dell'input!

	sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr));

	respStringLen = recvfrom(sock, echoBuffer, ECHOMAX - 1, 0, (struct sockaddr*)&fromAddr, &fromSize);
	echoBuffer[respStringLen] = '\0';

	printf("Stringa %s ricevuta dal server nome: %s indirizzo: %s \n", echoBuffer, HostName, HostIP); // Questa sarà senza vocali

	printf("Chiudo connessione con server\n");fflush(stdout);
	closesocket(sock);
	ClearWinSock();

	return 0;
}
