/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.

   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include "GameAPI.h"

#define MAX_RESPONSE_LENGTH 4096
#define MAX_COMMAND_LENGTH 128

using namespace std;

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;
int sd;			// descriptorul de socket
struct sockaddr_in server;	// structura folosita pentru conectare
bool isConnectionClosed;

int initConnection(char *argv[]);
int connectToServer();
void closeConnection();
int authentication();
int lobby();
int gameMenu(bool status);
void mainMenu();

int main (int argc, char *argv[]){

/* exista toate argumentele in linia de comanda? */
if (argc != 3)
{
  printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
  return -1;
}

initConnection(argv);

mainMenu();
//lobby();

}

void mainMenu(){

string command;

cout<<"[client: Main Menu]Enter command: ";
cin>>command;

if (!command.compare("start"))
    {
    connectToServer();
    cout<<"\n";
    authentication();
    }
    else
    {
    closeConnection();
    return;
    }
}

int initConnection(char* argv[]){

/* stabilim portul */
port = atoi (argv[2]);
/* cream socketul */
if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
{
  perror ("Eroare la socket().\n");
  return errno;
}

/* umplem structura folosita pentru realizarea conexiunii cu serverul */
/* familia socket-ului */
server.sin_family = AF_INET;
/* adresa IP a serverului */
server.sin_addr.s_addr = inet_addr(argv[1]);
/* portul de conectare */
server.sin_port = htons (port);
/* ne conectam la server */

}

int connectToServer(){

cout<<"Connected to server.\n";
cin.get();

if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
{
  perror ("Eroare la socket().\n");
  return errno;
}

if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
          perror ("[client]Eroare la connect().\n");
          return errno;
    }

isConnectionClosed = false;

}

void closeConnection(){

/* inchidem conexiunea, am terminat */
if (isConnectionClosed == false)
    {
    cout<<"Disconnected from server.\n";
    close (sd);
    isConnectionClosed = true;
    }
}

int authentication(){

char command[MAX_COMMAND_LENGTH];
char response[MAX_RESPONSE_LENGTH];
int responseLength;
int comparison;
string responseString;

while (1){
    cout<<"[client: Authentication]Enter command: ";

    cin.clear();
    cin.getline(command,MAX_COMMAND_LENGTH);
    cin.clear();

    cout.clear();
    if (write(sd,&command,sizeof(command)) <= 0)
        {
        perror ("[client]Eroare la write() spre server in autentificare.\n");
        return errno;
        }

    if ( (responseLength = read (sd, &response,MAX_RESPONSE_LENGTH) ) < 0)
        {
          perror ("[client]Eroare la read() de la server.\n");
          return errno;
        }

    response[responseLength] = 0;
    responseString=response;

    cout<<"[Response: Authentication]"<<responseString<<'\n';

    comparison = responseString.compare("Success!");

    if (comparison == 0)
        {
        lobby();
        return 1;
        }

    if ( strcmp(response,"Returning to main menu.") ==0 )
        {
            closeConnection();
            mainMenu();
            return 0;
        }
}

cout<<"Am terminat\n";

}

int lobby(){
    // mesajul trimis
char command[MAX_COMMAND_LENGTH];
char refresh[] = "refresh";
char response[MAX_RESPONSE_LENGTH];
int responseLength;

if (write(sd,&refresh,sizeof(refresh)) <= 0)
    {
    perror ("[client]Eroare la write() spre server in autentificare.\n");
    return errno;
    }

cout<<"List of available games:\n";
if ( (responseLength = read (sd, &response,MAX_RESPONSE_LENGTH) ) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }

response[responseLength] = 0;

cout<<response;

while (1){
    cout<<"[client: Lobby]Enter command: ";
    cin.getline(command,MAX_RESPONSE_LENGTH);
    if (write(sd,&command,sizeof(command)) <= 0)
        {
        perror ("[client]Eroare la write() spre server in autentificare.\n");
        return errno;
        }
    if ( (responseLength = read (sd, &response,MAX_RESPONSE_LENGTH) ) < 0)
        {
          perror ("[client]Eroare la read() de la server.\n");
          return errno;
        }

    response[responseLength] = 0;

    cout<<"[Response: Lobby]"<<response<<'\n';

    if ( response[0] == HOST_FLAG )
        {
         gameMenu(HOST);
         return 1;
        }

    if ( response[0] == GUEST_FLAG )
        {
         gameMenu(GUEST);
         return 1;
        }

    if (strcmp(response,"Exiting lobby. Returned to login screen.") == 0)
        {
        authentication();
        return 1;
        }
}
}

int gameMenu(bool status){

    bool result;
    GameManager* gameManager = new GameManager(sd);

    result = gameManager->playGame(status);
    gameManager->displayEndGameScreen(result);

    //TODO Return to main menu through button
    return 1;
}
