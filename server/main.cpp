#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#include <pthread.h>
#include <iostream>
#include "LobbyManager.h"
#include "GameServer.h"

/* portul folosit */
#define PORT 2906

using namespace std;

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;


thread th[100];
//pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
int i=0;

static void treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void serveAuthentication(void *);
void serveLobby(void *);
void serveGame(int client1, int client2);
void endGame(int winnerClient, int defeatedClient);

loginComponent* loginComp;
LobbyManager* lobbyManager;


int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;
  int sd;		//descriptorul de socket

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));

  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }


  loginComp = new loginComponent();
  lobbyManager = new LobbyManager(loginComp);

  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client;
      thData * td; //parametru functia executata de thread
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

	 //client= malloc(sizeof(int));
      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, (socklen_t *)&length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}

        /* s-a realizat conexiunea, se astepta mesajul */

	int idThread; //id-ul threadului
	int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));
	td->idThread=i++;
	td->cl=client;

	//pthread_create(&th[i], NULL, &treat, td);
    th[i] = thread(treat,td);

    loginComp->updateDatabase();
	}//while
};
static void treat(void * arg){

struct thData tdL;
tdL= *((struct thData*)arg);
printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
fflush (stdout);
pthread_detach(pthread_self()); //?
//th[tdL.idThread].detach();
serveAuthentication(arg);
};

void serveAuthentication(void* arg) {

struct thData tdL;
tdL= *((struct thData*)arg);

if (loginComp->authenticateUser(tdL.cl))
    serveLobby((struct thData*)arg);
    else
    {
    //loginComp->logoutUser(tdL.cl);
     /* am terminat cu acest client, inchidem conexiunea */
     close ((intptr_t)arg);
     //return(NULL);
    }

}

void serveLobby(void *arg)
{
	struct thData tdL;
	int result=0;
	tdL= *((struct thData*)arg);

	result = lobbyManager->treatClient(tdL.cl);

	if ( result == EXIT)
        {
        loginComp->logoutUser(tdL.cl);
        serveAuthentication(arg);
        return;
        //close((intptr_t)arg);
        }
        else
        if ( result == JOINER)
            {
            cout<<"The thread of client "<<tdL.cl<<" has been closed because he/she has joined a game.\n";
            return;
            }
            else
            serveGame(tdL.cl,result);
}

void serveGame(int client1,int client2) {

    int winner, defeated;
    GameServer* gameServer = new GameServer(client1,client2);

    cout<<"[main::serverGame()]"<<client1<<" and "<<client2<<" are playing a game!\n";

    winner = gameServer->serve();
    defeated = client1 ^ client2 ^ winner;

    lobbyManager->terminateGame(client1,client2);
    endGame(winner,defeated);
}

void endGame(int winnerClient, int defeatedClient) {

cout<<"[server main::endGame()] I am ending the game between "<<winnerClient<<" and "<<defeatedClient<<'\n';

loginComp->findUserByClient(winnerClient)->incrementMatches();
loginComp->findUserByClient(winnerClient)->incrementVictories();

loginComp->findUserByClient(defeatedClient)->incrementMatches();

loginComp->logoutUser(winnerClient);
loginComp->logoutUser(defeatedClient);
close(winnerClient);
close(defeatedClient);
}
