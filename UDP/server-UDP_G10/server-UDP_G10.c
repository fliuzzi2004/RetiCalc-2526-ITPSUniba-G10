#include <winsock.h>
#include <stdio.h>
#include <string.h>

#define ECHOMAX 255
#define PORT 3700

void ErrorHandler(char *errorMessage) {
	printf(errorMessage);
}

void ClearWinSock() {
	WSACleanup();
}

void RemoveVocali(char *buffer) {
	int read_index = 0;   // Legge TUTTI i caratteri
	int write_index = 0;  // Scrive SOLO i caratteri non-spazio

	while (buffer[read_index] != '\0') {

		if (buffer[read_index] != 'a' && buffer[read_index] != 'e' && buffer[read_index] != 'i' &&
				buffer[read_index] != 'o' && buffer[read_index] != 'u')
		{
			buffer[write_index] = buffer[read_index];
			write_index++;
		}
		read_index++;
	}
	buffer[write_index] = '\0';
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
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	int recvMsgSize;

	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		ErrorHandler("socket() failed");

	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_port = htons(PORT);
	echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// BIND DELLA SOCKET
	if ((bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) < 0)
		ErrorHandler("bind() failed");

	// RICEZIONE DELLA STRINGA ECHO DAL CLIENT
	while(1) {
		printf("Server in ascolto sulla porta %d\n", PORT);fflush(stdout);

		// RICEZIONE
		cliAddrLen = sizeof(echoClntAddr);
		recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX - 1, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen);
		echoBuffer[recvMsgSize] = '\0';
		
		struct sockaddr_in *sin = (struct sockaddr_in *) &echoClntAddr;

		struct hostent *host;
		host = gethostbyaddr((const char *) &sin->sin_addr, sizeof(sin->sin_addr), AF_INET);

		printf("Ricevuti dati dal client nome: %s indirizzo: %s \n", host->h_name, inet_ntoa(echoClntAddr.sin_addr));fflush(stdout);

		printf("Ricevuto: %s\n", echoBuffer);

		RemoveVocali(echoBuffer);

		int newLen = strlen(echoBuffer);
		if (sendto(sock, echoBuffer, newLen, 0, (struct sockaddr *)&echoClntAddr, sizeof(echoClntAddr)) != newLen)
			ErrorHandler("sendto() failed");
	}
}
