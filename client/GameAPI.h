#include "GameLogic.h"
#include <unistd.h>
#include "GameGUI.h"

#define HOST true
#define GUEST false

#define HOST_FLAG '^'
#define GUEST_FLAG '$'

#define PROGRESS true
#define DONE false

#define VICTORY true
#define DEFEAT false

#define MAX_RESPONSE_LENGTH 64
#define MAX_COMMAND_LENGTH 64


using namespace std;

struct CommandData {

    string moveType;
    string droppedPieceName;
    Position from;
    Position to;
    bool isPromotion;
    bool isCheck;
    bool isCheckMate;
    bool isWin;
    bool isLose;
};

class GameManager {

    private:
    GameLogic* gameLogic;
    bool gameStatus;
    bool gameResult;
    int serverSocket;
    bool playerStatus;
    bool whoMovesNow;

    void sendMoveToServer(string moveString);
    string getResponseFromServer();
    string getMoveFromUser();

    CommandData* extractDataFromCommand(string command);
    void solveCommand(CommandData* command);
    void warnCheck();
    void updateGameState(string command);

    public:
    GameManager(int serverSocket);
    ~GameManager();

    GameBoard* getGameBoard();
    bool playGame(bool status);
    void displayEndGameScreen(bool result);
};
