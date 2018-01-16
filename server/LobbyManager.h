#include "GameManager.h"
#define JOINER -2
#define EXIT -1
#define DISCONNECTED -3


using namespace std;


class LobbyManager {

    private:
    GameManager* gameManager;

    public:
    LobbyManager(loginComponent* loginComp) {gameManager = new GameManager(loginComp); /*cout<<"New LobbyManager instance!\n";*/};
    int treatClient(int clientDescriptor);
    void sendGameList(int clientDescriptor);
    void terminateGame(int client1,int client2);
};
