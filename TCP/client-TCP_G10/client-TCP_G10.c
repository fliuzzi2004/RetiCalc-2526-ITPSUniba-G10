#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 27020
#define BUFSIZE 256

void ClearWinSock() {
	WSACleanup();
}

void ErrorHandler(const char *errorMessage) {
	fprintf(stderr, "%s (Winsock Error Code: %d)\n", errorMessage, WSAGetLastError());
}

int main(void)
{
	WSADATA wsaData;
	SOCKET MyCSocket;
	struct sockaddr_in sad;

	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return -1;
	}

	// Configurazione dell'indirizzo del server
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(TARGET_IP);
	sad.sin_port = htons(TARGET_PORT);

	// Creazione del socket
	// PF_INET (IPv4), SOCK_STREAM (TCP), IPPROTO_TCP
	MyCSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MyCSocket == INVALID_SOCKET)
	{
		ErrorHandler("Creazione Socket fallita.");
		ClearWinSock();
		return -1;
	}

	printf("In attesa di connessione con %s:%d...\n", TARGET_IP, TARGET_PORT);fflush(stdout);

	// CONNESSIONE AL SERVER
	if (connect(MyCSocket,(struct sockaddr *) &sad, sizeof(sad)) == SOCKET_ERROR)
	{
		ErrorHandler("Failed to connect.");
		closesocket(MyCSocket);
		ClearWinSock();
		return 0;
	}

	printf("Connessione riuscita\n");fflush(stdout);

	char input_buffer[BUFSIZE] = "Hello";
	int bytes_sent;
	int bytes_received;
	int PrimaVolta = 0;
	int exit = 0;

	while (exit == 0) {

		if (PrimaVolta == 0)
		{
			printf("Inviato il primo messaggio: %s\n", input_buffer);fflush(stdout);
		}
		else
		{
			exit = 1;
			printf("Inserisci messaggio: ");fflush(stdout);
			// Acquisizione input (fgets)
			if (fgets(input_buffer, BUFSIZE, stdin) == NULL) {
				break; // Errore di lettura o EOF
			}
		}

		// Pulizia del \n e Gestione della Lunghezza
		size_t len = strlen(input_buffer);
		if (len > 0 && input_buffer[len - 1] == '\n') {
			input_buffer[len - 1] = '\0';
			len--; // Aggiorna la lunghezza senza il '\n'
		}

		// Invio al Server
		bytes_sent = send(MyCSocket, input_buffer, len, 0);
		if (bytes_sent == SOCKET_ERROR) {
			ErrorHandler("Send failed.");
			break;
		}

		if (PrimaVolta == 1)
		{
			// Ricezione della Risposta
			bytes_received = recv(MyCSocket, input_buffer, BUFSIZE - 1, 0);

			if (bytes_received > 0) {
				input_buffer[bytes_received] = '\0';
				printf("Messaggio ricevuto dal server: %s\n", input_buffer);fflush(stdout);
			} else if (bytes_received == 0) {
				printf("Il server ha chiuso connessione.\n");fflush(stdout);
				break;
			} else {
				ErrorHandler("Receive failed.");
				break;
			}
		}
		else
		{
			PrimaVolta = 1;
		}
	}

	printf("Chiudo connessione con server\n");fflush(stdout);
	closesocket(MyCSocket);
	ClearWinSock();

	return 0;
}
