#include "GameServer.h"
#include <unistd.h>
#include <string>

using namespace std;


GameServer::GameServer(int client1Descriptor,int client2Descriptor) {

    gameLogic = new GameLogic();
    client1 = client1Descriptor;
    client2 = client2Descriptor;

}

int GameServer::serve(){

    int movingPlayer, opponent;
    string command,result;
    movingPlayer = client1;
    movingPlayer = client2;
    short int orientation=UP;

    while (gameStatus == PROGRESS) {

        command = getMoveFromCurrentPlayer(movingPlayer);
        result = interpret(command,orientation);
        sendResponseToOpponent(opponent,result);
        swapPlayers(movingPlayer,opponent);
        orientation = -1*orientation;
    }

    return this->winner;
}


string GameServer::interpret(string command, short int orientation) {

    CommandData* commandData = extractDataFromCommand(command, orientation);

    if (command == "quit")
        {
            gameStatus = DONE;
            winner = setWinner(orientation);
            return "win";
        }

    if (orientation == UP)
        {
            solveCommand(commandData);
            mirrorCommandPositions(commandData);
        }
        else
        {
            mirrorCommandPositions(commandData);
            solveCommand(commandData);
        }

    return getCommandFromCommandData(commandData);
}

CommandData* GameServer::extractDataFromCommand(string command, short int orientation){

    string moveString = "move";
    string promotionString = "promote";
    string dropString = "drop";
    string pieceNameAndPosition;
    string pieceName;
    string piecePosition;
    CommandData* result = new CommandData;

    result->orientation = orientation;
    result->isCheck = false;
    result->isCheckMate = false;
    result->isPromotion = false;
    result->isWin = false;
    result->isLose = false;

    if ( command.find(moveString) != std::string::npos )
         {
            result->moveType = moveString;
            result->from.lin = command[5] - '0';
            result->from.col = command[7] - '0';
            result->to.lin = command[9] - '0';
            result->to.col = command[11] - '0';
         }

    if ( command.find(dropString) != std::string::npos )
         {
            result->moveType = dropString;
            pieceNameAndPosition = command.substr(5);
            pieceName = pieceNameAndPosition.substr(0,pieceNameAndPosition.find(" "));
            piecePosition = pieceNameAndPosition.substr(pieceName.length()+1);
            result->to.lin = piecePosition[0] - '0';
            result->to.col = piecePosition[2] - '0';
            result->droppedPieceName = pieceName;
        }

    if ( command.find(promotionString) != std::string::npos )
        result->isPromotion = true;

    return result;
}

void GameServer::solveCommand(CommandData* command) {

    if (command->moveType == "move")
        {
            gameLogic->movePiece(command->from,command->to);

            if (command->isPromotion)
                gameLogic->promotePiece(command->to);
        }

    if (command->moveType == "drop")
        {
            if (gameLogic->dropPiece(command->droppedPieceName,command->from,command->orientation) == INVALID_DROP)
                {
                command->isWin = true;
                gameStatus = DONE;
                setWinner(-1*command->orientation);
                return;
                }

        }

    if (gameLogic->isCheckFromPiece(command->to))
                {
                if (gameLogic->isKingCheckMated(-1*command->orientation))
                    {
                    command->isCheckMate = true;
                    command->isLose = true;
                    gameStatus = DONE;
                    setWinner(command->orientation);
                    }
                    else
                    command->isCheck = true;
                }

}

void GameServer::mirrorCommandPositions(CommandData* command) {

    command->from = gameLogic->getMirroredPosition(command->from);
    command->to = gameLogic->getMirroredPosition(command->to);
}

string GameServer::getCommandFromCommandData(CommandData* command) {

    string result="";

    result.append(command->moveType);
    result.append(" ");


    if (command->moveType == "move")
        {
            result.append(to_string(command->from.lin));
            result.append(" ");
            result.append(to_string(command->from.col));
            result.append(" ");
            result.append(to_string(command->to.lin));
            result.append(" ");
            result.append(to_string(command->to.col));
            result.append(" ");
        }

    if (command->moveType == "drop")
        {
            result.append(to_string(command->to.lin));
            result.append(" ");
            result.append(to_string(command->to.col));
            result.append(" ");
        }

    if ( command->isPromotion )
        result.append("promote ");

    if (command->isCheck)
        result.append("check ");

    if (command->isCheckMate)
        {
            result.append("checkmate");
            if (command->isWin)
                result.append("win ");
            if (command->isLose)
                result.append("lose ");
        }

    return result;
}

string GameServer::getMoveFromCurrentPlayer(int clientDescriptor) {

    char command[COMMAND_MAX_SIZE];
    string commandString;

    if (read (clientDescriptor, &command,COMMAND_MAX_SIZE) <= 0)
            perror ("Eroare la read() de la client la lobby.\n");

    commandString = string(command);

    return commandString;
}

void GameServer::sendResponseToOpponent(int clientDescriptor, string response) {

char* responseToClient = new char[response.size()+1];
copy(response.begin(),response.end(),responseToClient);
responseToClient[response.size()+1]=0;

if (write (clientDescriptor, responseToClient, sizeof(responseToClient)) <= 0)
    perror ("[Thread]Eroare la write() catre client.\n");

}

void GameServer::swapPlayers(int& player1, int& player2) {

    player1 = player1 ^ player2;
    player2 = player1 ^ player2;
    player1 = player1 ^ player2;

}

void GameServer::setWinner(short int orientation) {

    if (orientation == UP)
        winner = client1;
        else
        winner = client2;
}
