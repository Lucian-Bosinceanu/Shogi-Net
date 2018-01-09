#include "LobbyManager.h"
#include <unistd.h>
#include <iostream>

using namespace std;

int LobbyManager::treatClient(int clientDescriptor) {

char command[COMMAND_MAX_SIZE];
int commandLength;
string commandString, username;
string hostCommand("host");
string exitCommand("exit");
string haltCommand("halt");
string joinCommand("join");
string refreshCommand("refresh");

//sendGameList(clientDescriptor);

char host[] = "^Someone has joined the game!";
char join[] = "$Joining the game.";
char error[] = "Game does not exist. Please refresh.";
char exit[] = "Exiting lobby. Returned to login screen.";

while (1) {

    if ( ( commandLength = read (clientDescriptor, &command,COMMAND_MAX_SIZE) ) <= 0)
        perror ("Eroare la read() de la client la lobby.\n");

    command[commandLength] = 0;
    commandString = string(command);

    if (commandString.find(refreshCommand) != std::string::npos)
        {
            sendGameList(clientDescriptor);
            continue;
        }

    if ( commandString.find(hostCommand) != std::string::npos )
        {
            Game* game = new Game();
            game->host(clientDescriptor);
            cout<<clientDescriptor<<" is hosting a game!"<<'\n';
            gameManager->addGame(game);

            while (1) {
                if (game->getGameStatus() == READY)
                    {
                        if (write (clientDescriptor, &host, sizeof(host)) <= 0)
                            perror ("[Thread]Eroare la write() catre client.\n");
                        return game->getJoiner();
                    }
            }

        }

    if ( commandString.find(joinCommand) != std::string::npos  )
        {
            username = commandString.substr(commandString.find(" ") +1);
            if (gameManager->joinGameByUsername(username,clientDescriptor))
                {
                    if (write (clientDescriptor, &join, sizeof(join)) <= 0)
                            perror ("[Thread]Eroare la write() catre client.\n");
                    return JOINER;
                }
                else
                {
                    if (write (clientDescriptor, &error, sizeof(error)) <= 0)
                            perror ("[Thread]Eroare la write() catre client.\n");
                    continue;
                }

        }

    if ( commandString.find(haltCommand) != std::string::npos )
        {
            //TO DO?
            continue;
        }

    if ( commandString.find(exitCommand) !=std::string::npos )
        {
        if (write (clientDescriptor, &exit, sizeof(exit)) <= 0)
                            perror ("[Thread]Eroare la write() catre client.\n");
        return EXIT;
        }
}

}

void LobbyManager::terminateGame(int client1,int client2){

Game* gameToTerminate = gameManager->findGameByClientDescriptors(client1,client2);
gameManager->removeGame(gameToTerminate);
cout<<"[LobbyManager::terminateGame()] The game between "<<client1<<" and "<<client2<<" has been terminated!\n";
}

void LobbyManager::sendGameList(int clientDescriptor){

vector<string> gameList;
string response = "";

gameList = gameManager->retrieveWaitingGames();

if (gameList.size()==0)
    response.append("There are no games to join at the moment. Host one!\n");
    else
    for ( auto it : gameList)
        {
            response.append(it);
            response.append(" \n");
        }

char* responseToClient = new char[response.size()+1];
copy(response.begin(),response.end(),responseToClient);
responseToClient[response.size()+1]=0;

cout<<responseToClient<<'\n';
cout<<response.size()<<'\n';
if (write (clientDescriptor, responseToClient, response.size()) <= 0)
    perror ("[Thread]Eroare la write() catre client.\n");
cout<<"Game list sent\n";
delete responseToClient;
}
