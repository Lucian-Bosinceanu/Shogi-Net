#include <set>
#include "Game.h"
#include "loginComponent.h"
#include <string>
#include <vector>

using namespace std;

class GameManager {

    private:
    set<Game*> gameInstances;
    loginComponent* loginComp;

    string getHostInfo(Game* game);

    public:
    GameManager(loginComponent* loginComp) {this->loginComp = loginComp; cout<<"New GameManager instance!\n";};
    set<Game*> getGameInstancesList() {return gameInstances;};
    void addGame(Game* game) {gameInstances.insert(game); };
    void removeGame(Game* game) {gameInstances.erase(game); };
    vector<string> retrieveWaitingGames();
    bool joinGameByUsername(string username, int JoinerDescriptor);

    ~GameManager();
};
