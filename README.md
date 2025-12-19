# RetiCalc-2526-ITPSUniba-G10
Francesco Liuzzi
Il progetto implementa due sistemi Client/Server (TCP e UDP) che consentono lo scambio di stringhe di caratteri tra due host. 
Il server elabora il messaggio ricevuto eliminando tutte le vocali prima di reinviarle al client.

------- GUIDA ALL'ESECUZIONE PASSO-PASSO TCP -------
FASE 1: Avvio del Server
Eseguire: server-TCP_G10.exe
Prompt Server:
  Server in ascolto sulla porta 27020
  In attesa di connessione di client...

FASE 2: Connessione Client e scambio di messaggi
Eseguire: client-TCP_G10.exe
Prompt Client:
  In attesa di connessione con 127.0.0.1:27020...
  Connessione riuscita
  Inviato il primo messaggio: Hello

FASE 3: Identificazione Client
Prompt Server:
  Client connesso
  Ricevuti dati dal client con indirizzo 127.0.0.1
  Ho ricevuto seguente messaggio: Hello

FASE 4: Invio Stringa
Prompt Client:
  Inserisci messaggio: foglia

FASE 5: Elaborazione Server
Prompt Server:
  Ho ricevuto seguente messaggio: foglia
  Invio al client messaggio aggiornato

FASE 6: Risultato Finale e Chiusura
Prompt Client:
  Messaggio ricevuto dal server: fgl
  Chiudo connessione con server

FASE 7: Server in attesa di connessioni di client
Prompt Server:
  Client disconnesso
  Server in ascolto sulla porta 27020
  In attesa di connessione di client...

------- GUIDA ALL'ESECUZIONE PASSO-PASSO UDP ------- 
FASE 1: Avvio del Server UDP
Eseguire: server-UDP_G10.exe
Prompt Server:
  Server in ascolto sulla porta 3700

FASE 2: Configurazione Client (Risoluzione DNS tramite file "hosts")
Eseguire: client-UDP_G10.exe
Prompt Client:
  Inserisci nome host: reti
  Inserisci porta host: 3700

FASE 3: scambio di messaggi "Hello" e ricezione del server
Il client invia automaticamente il messaggio "Hello".
Prompt Server:
  Ricevuti dati dal client nome: localhost indirizzo: 127.0.0.1 
  Ricevuto: Hello

FASE 4: Invio Stringa Client
Prompt Client:
  Inserisci la stringa echo da inviare al server: foglia

FASE 5: Elaborazione Server
Il server riceve la stringa, rimuove le vocali e la rispedisce.
Prompt Server:
  Ricevuto: foglia

FASE 6: Visualizzazione Risposta Finale e chiusura connessione Client
Prompt Client:
  Stringa fgl ricevuta dal server nome: reti indirizzo: 127.0.0.1 
  Chiudo connessione con server

FASE 7: Server in attesa di connessioni di client
Prompt Server:
  Server in ascolto sulla porta 3700
