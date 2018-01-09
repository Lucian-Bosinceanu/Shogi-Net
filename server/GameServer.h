#include "GameLogic.h"

#define PROGRESS true
#define DONE false

#define COMMAND_MAX_SIZE 64

using namespace std;

struct CommandData {

    string moveType;
    string droppedPieceName;
    Position from;
    Position to;
    short int orientation;
    bool isPromotion;
    bool isCheck;
    bool isCheckMate;
    bool isWin;
    bool isLose;
};

class GameServer{

    private:
    GameLogic* gameLogic;
    int client1;
    int client2;
    bool gameStatus;
    int winner;

    string interpret(string command, short int orientation);
    string getMoveFromCurrentPlayer(int clientDescriptor);
    void sendResponseToOpponent(int clientDescriptor, string response);
    void swapPlayers(int& player1, int& player2);

    CommandData* extractDataFromCommand(string command, short int orientation);
    void solveCommand(CommandData* command);
    void mirrorCommandPositions(CommandData* command);
    string getCommandFromCommandData(CommandData* command);
    void setWinner(short int orientation);

    public:
    GameServer(int client1Descriptor,int client2Descriptor);
    ~GameServer();
    int serve();

};
