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

#define HAND 123
#define BOARD 124


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
    GameGUI* gameGUI;
    bool gameStatus;
    bool gameResult;
    int serverSocket;
    bool playerStatus;
    bool whoMovesNow;
    bool isKingInCheck;

    void sendMoveToServer(string moveString);
    string getResponseFromServer();
    string getMoveFromUser();

    CommandData* extractDataFromCommand(string command);
    void solveCommand(CommandData* command);
    void warnCheck();
    void updateGameState(string command);

    void drawPieces();
    void drawPlayerHand(short int orientation);
    void highlightPositions(int where,unordered_set<Position*> positions);
    void drawGameScreen();

    public:
    GameManager(int serverSocket,GameGUI* gameGUI);
    ~GameManager();

    GameBoard* getGameBoard();
    bool playGame(bool status);
    void displayEndGameScreen(bool result);
};
