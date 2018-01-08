#include "GameManager.h"

using namespace std;

string GameManager::getHostInfo(Game* game) {
    string result = "";
    UserData* user = loginComp->findUserByClient(game->getHost());
    result.append(user->getUsername());
    result.append(" ");
    result.append(to_string(user->getNrVictories()));
    result.append("V ");
    result.append(to_string(user->getNrMatches()));
    result.append("M");
    return result;
}

GameManager::~GameManager() {

gameInstances.clear();

}

vector<string> GameManager::retrieveWaitingGames(){

vector<string> results;

for ( auto it : gameInstances)
    if (it->getGameStatus() == WAITING)
        {
        cout<<"Game found!\n";
        results.push_back(getHostInfo(it));
        }

return results;
}

bool GameManager::joinGameByUsername(string username,int joinerDescriptor){

UserData* user;

cout<<joinerDescriptor<<" wants to join the game of "<<username<<'\n';

for ( auto it : gameInstances )
    {
    user = loginComp->findUserByClient(it->getHost());
    if ( !username.compare( user->getUsername()  )  )
        {
            if (it->getGameStatus() == WAITING)
                {
                cout<<"It was possible for "<<joinerDescriptor<<" to join the game of "<<username<<'\n';
                it->join(joinerDescriptor);
                return true;
                }
                else
                return false;
        }
    }

return false;
}

Game* GameManager::findGameByClientDescriptors(int client1, int client2){

for (auto it : gameInstances)
    if (it->getHost() == client1 && it->getJoiner() == client2)
        return it;

return NULL;

}
