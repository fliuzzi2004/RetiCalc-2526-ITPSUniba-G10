#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Necessario per strlen

#define SERVER_PORT 27020
#define BACKLOG 6 // Lunghezza massima della coda di connessioni in sospeso
#define BUFSIZE 256

void ClearWinSock() {
	WSACleanup();
	printf("\nWinsock cleanup completed.\n");fflush(stdout);
}

// Funzione di gestione degli errori migliorata
void ErrorHandler(const char *errorMessage) {
	fprintf(stderr, "%s (Winsock Error Code: %d)\n", errorMessage, WSAGetLastError());fflush(stderr);
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

// Funzione di gestione della comunicazione con il singolo client
void HandleTCPClient(SOCKET clientSocket, struct sockaddr_in clientAddress) {
	char buffer[BUFSIZE];

	int recvMsgSize;
	int nMessaggiRicevuti = 0; // Contatore dei messaggi

	printf("Ricevuti dati dal client con indirizzo %s\n", inet_ntoa(clientAddress.sin_addr) );fflush(stdout);

	// Ciclo WHILE per gestire l'invio e la ricezione continua
	while ((recvMsgSize = recv(clientSocket, buffer, BUFSIZE - 1, 0)) > 0)
	{
		// 1. Ricezione e Terminazione
		buffer[recvMsgSize] = '\0';
		nMessaggiRicevuti++;	// Incremento del contatore

		printf("Ho ricevuto seguente messaggio: %s\n", buffer); fflush(stdout);

		int bytes_to_send;

		if (nMessaggiRicevuti != 1)
		{
			RemoveVocali(buffer);

			printf("Invio al client messaggio aggiornato\n"); fflush(stdout);
			bytes_to_send = strlen(buffer);

			if (send(clientSocket, buffer, bytes_to_send, 0) != bytes_to_send) {
				ErrorHandler("send() failed.");
				break;
			}
		}
	}

	if (recvMsgSize == 0) {
		printf("Client disconnesso\n");fflush(stdout);
	}
	else {
		ErrorHandler("recv() failed.");
	}
	closesocket(clientSocket);
}

int main(void)
{
	WSADATA wsaData;
	SOCKET socket_server;
	struct sockaddr_in sad;

	// Configurazione dell'indirizzo del server (localhost)
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(SERVER_PORT);

	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);fflush(stdout);
		return -1;
	}

	// Creazione del Socket
	socket_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_server == INVALID_SOCKET)
	{
		ErrorHandler("Socket - creation failed.");
		ClearWinSock();
		return -1;
	}

	// Binding del Socket
	if (bind(socket_server, (struct sockaddr*) &sad, sizeof(sad)) == SOCKET_ERROR)
	{
		ErrorHandler("bind() failed.");
		closesocket(socket_server);
		ClearWinSock();
		return -1;
	}

	// Ascolto delle connessioni
	if (listen (socket_server, BACKLOG) == SOCKET_ERROR) {
		ErrorHandler("listen() failed.");
		closesocket(socket_server);
		ClearWinSock();
		return -1;
	}

	struct sockaddr_in cad; // Struttura per l'indirizzo del client
	SOCKET clientSocket;
	int clientLen;
	while (1) {
		clientLen = sizeof(cad);

		// Accetta la connessione in entrata (bloccante)
		printf("Server in ascolto sulla porta %d\n", SERVER_PORT);fflush(stdout);
		printf("In attesa di connessione di client...\n");fflush(stdout);
		clientSocket = accept(socket_server, (struct sockaddr *)&cad, &clientLen);

		if (clientSocket == INVALID_SOCKET) {
			ErrorHandler("accept() failed. Continuing to listen...");
			continue; // Continua ad ascoltare in caso di errore
		}

		printf("Client connesso\n");fflush(stdout);
		HandleTCPClient(clientSocket, cad);
	}

	// Pulizia del socket server e di Winsock
	closesocket(socket_server);
	ClearWinSock();

	return 0;
}
