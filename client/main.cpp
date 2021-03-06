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
#include <SFML/Graphics.hpp>

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
GameGUI* gameGUI;


int initConnection(char *argv[]);
int connectToServer();
void closeConnection();
int loginMenu();
int registerMenu();
int lobby();
int gameMenu(bool status);
void mainMenu();

int sendCommandToServer(int serverSocket, string command);
string getResponseFromServer(int serverSocket);


int main (int argc, char *argv[]){

/* exista toate argumentele in linia de comanda? */
if (argc != 3)
{
  printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
  return -1;
}

initConnection(argv);
//window.create(sf::VideoMode(1600,900),"Shogi Net Client");
gameGUI = new GameGUI();

mainMenu();
//lobby();

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

isConnectionClosed = true;

}

int connectToServer(){

cout<<"Connected to server.\n";

if (!isConnectionClosed)
    return 1;

if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
{
  perror ("Eroare la socket().\n");
  return errno;
}

if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
          perror ("[client]Eroare la connect().\n");
          exit(1);
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



int sendCommandToServer(int serverSocket, string command) {

    char* commandChar = new char[command.size()+1];
    copy(command.begin(),command.end(),commandChar);
    commandChar[command.size()]=0;

    //cout<<"[main sendCommandToServer] I am sending this command to server: "<<command<<"|\n";

    if (write(serverSocket,commandChar,command.size()) <= 0)
        {
        perror ("[client]Eroare la write() spre server.\n");
        exit(1);
        return errno;
        }

}
string getResponseFromServer(int serverSocket) {

    char response[MAX_RESPONSE_LENGTH];
    int responseLength;
    string responseString;

    if ( ( responseLength = read (serverSocket, &response,MAX_RESPONSE_LENGTH) ) <= 0)
            {
                perror ("[client]Eroare la read() de la server.\n");
                exit(1);
            }

    response[responseLength] = 0;
    responseString = string(response);

    //cout<<"[main getResponseFromServer()] I have received this response from server: "<<responseString<<'\n';

    return responseString;
}

void mainMenu(){

string command;

gameGUI->drawMenu("main");

gameGUI->clearEventQueue();

while (gameGUI->getWindow()->isOpen())
    {
        sf::Event event;
        while (gameGUI->getWindow()->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameGUI->getWindow()->close();
                closeConnection();
            }

            if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (gameGUI->getMenu("main")->isButtonPressed("play",event.mouseButton.x,event.mouseButton.y))
                            {
                            //connectToServer();
                            //cout<<"\n";
                            loginMenu();
                            return;
                            }

                        if (gameGUI->getMenu("main")->isButtonPressed("exit",event.mouseButton.x,event.mouseButton.y))
                            {
                            closeConnection();
                            gameGUI->getWindow()->close();
                            return;
                            }
                    }
                }

        gameGUI->drawMenu("main");
        gameGUI->drawTitle();
        }

    }

}

int loginMenu(){

string commandString;
string response;
string username, password;

//cout<<"Am ajuns in login\n";
connectToServer();

gameGUI->clearEventQueue();

gameGUI->getMenu("login")->getButtonByName("username")->setText("Enter username");
gameGUI->getMenu("login")->getButtonByName("password")->setText("Enter password");
gameGUI->getMenu("login")->getButtonByName("warning")->setText("Please provide your credentials.");

gameGUI->drawMenu("login");
gameGUI->getWindow()->display();

//cout<<"[client: login]Enter command: ";

while (gameGUI->getWindow()->isOpen())
    {
        sf::Event event;
        while (gameGUI->getWindow()->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameGUI->getWindow()->close();
                closeConnection();
            }

            if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {

                        if (gameGUI->getMenu("login")->isButtonPressed("username",event.mouseButton.x,event.mouseButton.y))
                            username = gameGUI->getInputFromClient(gameGUI->getMenu("login"),gameGUI->getMenu("login")->getButtonByName("username"),VISIBLE);

                        if (gameGUI->getMenu("login")->isButtonPressed("password",event.mouseButton.x,event.mouseButton.y))
                            password = gameGUI->getInputFromClient(gameGUI->getMenu("login"),gameGUI->getMenu("login")->getButtonByName("password"),HIDDEN);

                        if (gameGUI->getMenu("login")->isButtonPressed("login",event.mouseButton.x,event.mouseButton.y))
                            {
                                commandString = string("login ") + username + string(" ") + password;
                                sendCommandToServer(sd,commandString);
                                response = getResponseFromServer(sd);

                                if (!response.compare("Success!"))
                                    {
                                        lobby();
                                        return 1;
                                    }
                                    else
                                    {
                                        gameGUI->getMenu("login")->setButtonText("warning",response);
                                    }
                            }

                        if (gameGUI->getMenu("login")->isButtonPressed("registerL",event.mouseButton.x,event.mouseButton.y))
                            {
                                registerMenu();
                                return 1;
                            }

                        if (gameGUI->getMenu("login")->isButtonPressed("back",event.mouseButton.x,event.mouseButton.y))
                            {
                                sendCommandToServer(sd,string("back"));
                                response = getResponseFromServer(sd);

                                cout<<"[Login Menu] Response from server when back: "<<response<<"|\n";

                                if (!response.compare("Returning to main menu."))
                                    {
                                        cout<<"[Login Menu] Returning to main menu.\n";
                                        closeConnection();
                                        mainMenu();
                                        return 1;
                                    }
                            }
                    }
                }

        gameGUI->drawMenu("login");
        gameGUI->getWindow()->display();

        }

    }

}

int registerMenu(){

string commandString;
string response;
string username, password, confirmPassword;

gameGUI->clearEventQueue();

gameGUI->getMenu("register")->getButtonByName("username")->setText("Enter username");
gameGUI->getMenu("register")->getButtonByName("password")->setText("Enter password");
gameGUI->getMenu("register")->getButtonByName("confirmPass")->setText("Re-enter password");
gameGUI->getMenu("register")->getButtonByName("warning")->setText("Please provide your credentials.");

gameGUI->drawMenu("register");
gameGUI->getWindow()->display();

//cout<<"[client: login]Enter command: ";

while (gameGUI->getWindow()->isOpen())
    {
        sf::Event event;
        while (gameGUI->getWindow()->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameGUI->getWindow()->close();
                closeConnection();
            }

            if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {

                        if (gameGUI->getMenu("register")->isButtonPressed("username",event.mouseButton.x,event.mouseButton.y))
                            username = gameGUI->getInputFromClient(gameGUI->getMenu("register"),gameGUI->getMenu("register")->getButtonByName("username"),VISIBLE);

                        if (gameGUI->getMenu("register")->isButtonPressed("password",event.mouseButton.x,event.mouseButton.y))
                            password = gameGUI->getInputFromClient(gameGUI->getMenu("register"),gameGUI->getMenu("register")->getButtonByName("password"),HIDDEN);

                        if (gameGUI->getMenu("register")->isButtonPressed("confirmPass",event.mouseButton.x,event.mouseButton.y))
                            confirmPassword= gameGUI->getInputFromClient(gameGUI->getMenu("register"),gameGUI->getMenu("register")->getButtonByName("confirmPass"),HIDDEN);

                        if (gameGUI->getMenu("register")->isButtonPressed("registerR",event.mouseButton.x,event.mouseButton.y))
                            {
                                commandString = string("register ") + username + string(" ") + password + string(" ") + confirmPassword;
                                sendCommandToServer(sd,commandString);
                                response = getResponseFromServer(sd);

                                if (!response.compare("Success!"))
                                    {
                                        lobby();
                                        return 1;
                                    }
                                    else
                                    {
                                        gameGUI->getMenu("register")->setButtonText("warning",response);
                                    }
                            }


                        if (gameGUI->getMenu("register")->isButtonPressed("back",event.mouseButton.x,event.mouseButton.y))
                            {
                                sendCommandToServer(sd,string("back"));
                                response = getResponseFromServer(sd);

                                if (!response.compare("Returning to main menu."))
                                    {
                                        closeConnection();
                                        mainMenu();
                                        return 1;
                                    }
                            }
                    }
                }

        gameGUI->drawMenu("register");
        gameGUI->getWindow()->display();
        }

    }

}

int lobby(){

string commandString;
string response;
string hostCommand = string("host");
string refreshCommand = string("refresh");
string backCommand = string("exit");
string playerName = "?????";

gameGUI->clearEventQueue();

//gameGUI->getMenu("lobby")->getButtonByName("warning")->setText("");
gameGUI->getMenu("lobby")->getButtonByName("playerSelect")->setText("Type here the player name you wish to play against, then click JOIN.");
gameGUI->getMenu("lobby")->getButtonByName("gameList")->setText("");

sendCommandToServer(sd,refreshCommand);
response = getResponseFromServer(sd);
gameGUI->getMenu("lobby")->setButtonText("gameList",response);

gameGUI->drawMenu("lobby");
gameGUI->getWindow()->display();

while (gameGUI->getWindow()->isOpen())
    {
        sf::Event event;
        while (gameGUI->getWindow()->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameGUI->getWindow()->close();
                closeConnection();
            }

           if (event.mouseButton.button == sf::Mouse::Left)
                {

                if (gameGUI->getMenu("lobby")->isButtonPressed("host",event.mouseButton.x,event.mouseButton.y))
                    {
                        gameGUI->getMenu("lobby")->getButtonByName("playerSelect")->setText("You are hosting a game. Please wait for someone to join.");
                        gameGUI->drawMenu("lobby");
                        gameGUI->getWindow()->display();

                        sendCommandToServer(sd,hostCommand);
                        response = getResponseFromServer(sd);

                        if ( response[0] == HOST_FLAG )
                            {
                             gameMenu(HOST);
                             return 1;
                            }
                    }

                if(gameGUI->getMenu("lobby")->isButtonPressed("refresh",event.mouseButton.x,event.mouseButton.y))
                    {
                        sendCommandToServer(sd,refreshCommand);
                        response = getResponseFromServer(sd);

                        gameGUI->getMenu("lobby")->setButtonText("gameList",response);
                    }

                if(gameGUI->getMenu("lobby")->isButtonPressed("playerSelect",event.mouseButton.x,event.mouseButton.y))
                        playerName = gameGUI->getInputFromClient(gameGUI->getMenu("lobby"),gameGUI->getMenu("lobby")->getButtonByName("playerSelect"),VISIBLE);

                if (gameGUI->getMenu("lobby")->isButtonPressed("join",event.mouseButton.x,event.mouseButton.y))
                    {
                        commandString = string("join ");
                        commandString.append(playerName);
                        sendCommandToServer(sd,commandString);
                        response = getResponseFromServer(sd);

                        if ( response[0] == GUEST_FLAG )
                            {
                             gameMenu(GUEST);
                             return 1;
                            }
                            else
                            gameGUI->getMenu("lobby")->getButtonByName("playerSelect")->setText(response);

                    }

                if (gameGUI->getMenu("lobby")->isButtonPressed("back",event.mouseButton.x,event.mouseButton.y))
                    {
                        sendCommandToServer(sd,backCommand);
                        response = getResponseFromServer(sd);

                        if (!response.compare("Exiting lobby. Returned to login screen."))
                                    {
                                        closeConnection();
                                        loginMenu();
                                        return 1;
                                    }

                    }

                }

        gameGUI->drawMenu("lobby");
        gameGUI->getWindow()->display();

        }
    }

}

int gameMenu(bool status){

    bool result;
    GameManager* gameManager = new GameManager(sd,gameGUI);

    //gameGUI.setGameBoard(gameManager->getGameBoard());
    //cout<<"[main: gameMenu] I am starting the game.\n";

    gameGUI->getMenu("game")->getButtonByName("forfeit")->setText("FORFEIT");
    gameGUI->getMenu("game")->getButtonByName("forfeit")->setText("FORFEIT");
    gameGUI->getMenu("game")->getButtonByName("status")->setText("");

    result = gameManager->playGame(status);
    gameManager->displayEndGameScreen(result);


    gameGUI->clearEventQueue();

    while (gameGUI->getWindow()->isOpen())
    {
        sf::Event event;
        while (gameGUI->getWindow()->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                {
                    gameGUI->getWindow()->close();
                    closeConnection();
                }

           if (event.mouseButton.button == sf::Mouse::Left)
                if (gameGUI->getMenu("game")->isButtonPressed("forfeit",event.mouseButton.x,event.mouseButton.y))
                    {
                        closeConnection();
                        mainMenu();
                        return 1;
                    }
        }
    }

    return 1;
}

