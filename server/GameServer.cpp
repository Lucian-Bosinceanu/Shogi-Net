#include "GameServer.h"
#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;


GameServer::GameServer(int client1Descriptor,int client2Descriptor) {

    gameLogic = new GameLogic();
    client1 = client1Descriptor;
    client2 = client2Descriptor;
    gameStatus = PROGRESS;

    winCommandSent = false;
    loseCommandSent = false;
    isWinByCheckmate = false;

}

GameServer::~GameServer(){

    gameLogic->~GameLogic();
}

int GameServer::serve(){

    int movingPlayer, opponent;
    string command,result;
    movingPlayer = client1;
    opponent = client2;
    short int orientation=UPPLAYER;

    cout<<"[GameServer::serve()] I have started to serve the game between "<<client1<<" and "<<client2<<'\n';

    while (gameStatus == PROGRESS) {

        cout<<"-------------------------------------------------------------------------------\n";
        cout<<"[GameServer::serve()] I am waiting for "<<movingPlayer<<" to make a move.\n";
        command = getMoveFromCurrentPlayer(movingPlayer);
        result = interpret(command,orientation);
        sendResponseToOpponent(opponent,result);
        swapPlayers(movingPlayer,opponent);
        orientation = -1*orientation;
    }

    cout<<"[GameServer::serve()] The game is over. Client "<<winner<<" has won the game!\n";
    //sendResponseToOpponent(winner ^ client1 ^ client2,"lose");
    sendLastResponse();
    return this->winner;
}


void GameServer::sendLastResponse(){

    int lastMessageReceiver = lastSender ^ client1 ^ client2;

    cout<<"[GameServer::sendLastResponse()] I am sending the last response to client: "<<lastMessageReceiver<<'\n';

    if (isWinByCheckmate)
        {
        cout<<"[GameServer::sendLastResponse()] The game is over due to checkmate. I am sending the winner a win command.\n";
        sendResponseToOpponent(winner,string("win"));
        return;
        }

    if (winCommandSent)
        {
        cout<<"[GameServer::sendLastResponse()] Client "<<lastSender<<" has received a win command. Thus, I am sending a lose command to client "<<lastMessageReceiver<<'\n';
        sendResponseToOpponent(lastMessageReceiver,string("lose"));
        return;
        }

    if (loseCommandSent)
        {
        cout<<"[GameServer::sendLastResponse()] Client "<<lastSender<<" has received a lose command. Thus, I am sending a win command to client "<<lastMessageReceiver<<'\n';
        sendResponseToOpponent(lastMessageReceiver,string("win"));
        }
}

string GameServer::interpret(string command, short int orientation) {

    CommandData* commandData = extractDataFromCommand(command, orientation);

    cout<<"[GameServer::interpret()] I am interpreting command: "<<command<<'\n';

    if (command == "quit")
        {
            gameStatus = DONE;
            setWinner(-1*orientation);
            winCommandSent = true;
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
    string loseString = "lose";
    string pieceNameAndPosition;
    string pieceName;
    string piecePosition;
    CommandData* result = new CommandData;

    cout<<"[GameServer::extractDataFromCommand] I am extracting data from command "<<command<<'\n';

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

            cout<<"[GameServer::extractDataFromCommand] pieceNameAndPosition:"<<pieceNameAndPosition<<'\n';
            cout<<"[GameServer::extractDataFromCommand] pieceName:"<<pieceName<<'\n';
            cout<<"[GameServer::extractDataFromCommand] piecePosition:"<<piecePosition<<'\n';

            result->to.lin = piecePosition[0] - '0';
            result->to.col = piecePosition[2] - '0';
            result->droppedPieceName = pieceName;

            cout<<"[GameServer::extractDataFromCommand] Result position= "<<result->to.lin<<' '<<result->to.col<<'\n';

        }

    if (command.find(loseString) != std::string::npos  )
        result->isWin = true;

    if ( command.find(promotionString) != std::string::npos )
        result->isPromotion = true;

    return result;
}

void GameServer::solveCommand(CommandData* command) {

    cout<<"[GameServer::solveCommand] I am solving the command above.\n";
    if (command->moveType == "move")
        {
            cout<<"[GameServer::solveCommand] I am solving a move command.\n";
            gameLogic->movePiece(command->from,command->to,command->orientation);

            if (command->isPromotion)
                gameLogic->promotePiece(command->to);
        }

    if (command->moveType == "drop")
        {
            cout<<"[GameServer::solveCommand] I am solving a drop command.\n";
            if (gameLogic->dropPiece(command->droppedPieceName,command->to,command->orientation) == INVALID_DROP)
                {
                cout<<"[GameServer::solveCommand] The game is over due to illegal pawn drop.\n";
                command->isWin = true;
                gameStatus = DONE;
                setWinner(-1*command->orientation);
                return;
                }

        }

    cout<<"[GameServer::solveCommand] I am checking if this move has cheked the opposing king.\n";
    if (gameLogic->isCheckFromPiece(command->to))
                {
                cout<<"[GameServer::solveCommand] The king is checked. I am verifying if the king is checkmated.\n";
                command->isCheck = true;
                if (gameLogic->isKingCheckMated(-1*command->orientation))
                    {
                    command->isCheckMate = true;
                    command->isLose = true;
                    gameStatus = DONE;
                    setWinner(command->orientation);
                    }
                }

    cout<<"[GameServer::solveCommand] I am done solving this move.\n";
}

void GameServer::mirrorCommandPositions(CommandData* command) {

    cout<<"[GameServer::mirrorCommandPositions] I am mirroring position "<<command->from.lin<<' '<<command->from.col<<" -> ";
    command->from = gameLogic->getMirroredPosition(command->from);
    cout<<command->from.lin<<' '<<command->from.col<<'\n';

    cout<<"[GameServer::mirrorCommandPositions] I am mirroring position "<<command->to.lin<<' '<<command->to.col<<" -> ";
    command->to = gameLogic->getMirroredPosition(command->to);
    cout<<command->to.lin<<' '<<command->to.col<<'\n';
}

string GameServer::getCommandFromCommandData(CommandData* command) {

    string result="";

    result.append(command->moveType);
    result.append(" ");

    cout<<"[GameServer::getCommandFromCommandData()] I am trying to obtain a string from the command above\n.";

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
            result.append(command->droppedPieceName);
            result.append(" ");
            result.append(to_string(command->to.lin));
            result.append(" ");
            result.append(to_string(command->to.col));
            result.append(" ");
        }

    if ( command->isPromotion )
        result.append("promote ");

    if (command->isCheck)
        {
            if (command->isCheckMate)
            {
                result.append("checkmate ");
                isWinByCheckmate = true;
                if (command->isWin)
                    {
                    result.append("win ");
                    winCommandSent = true;
                    }
                if (command->isLose)
                    {
                    result.append("lose ");
                    loseCommandSent = true;
                    }
            }
            else
            result.append("check ");
        }

    if (command->isWin)
        {
        result.append("win ");
        winCommandSent = true;
        }
    if (command->isLose)
        {
        result.append("lose ");
        loseCommandSent = true;
        }
    cout<<"[GameServer::getCommandFromCommandData()] My output is: "<<result<<'\n';
    return result;
}

string GameServer::getMoveFromCurrentPlayer(int clientDescriptor) {

    char command[COMMAND_MAX_SIZE];
    int commandLength;
    string commandString;

    if ( ( commandLength = read (clientDescriptor, &command,COMMAND_MAX_SIZE) ) <= 0)
            {
                perror ("[GameServer::getCommandFromCommandData]Eroare la read() de la client.\n");
                gameStatus = DONE;
                return "quit";
            }

    command[commandLength] = 0;
    commandString = string(command);

    cout<<"[GameServer::getMoveFromCurrentPlayer()] I have received move: "<<command<<" from client "<<clientDescriptor<<" with length "<<commandString.size()<<'\n';

    return commandString;
}

void GameServer::sendResponseToOpponent(int clientDescriptor, string response) {

char* responseToClient = new char[response.size()+1];
copy(response.begin(),response.end(),responseToClient);
responseToClient[response.size()]=0;

cout<<"[GameServer::sendResponseToOpponent()] I am sending this move "<<responseToClient<<" to client "<<clientDescriptor<<'\n';

if (write (clientDescriptor, responseToClient, response.size()) <= 0)
    {
        perror ("[GameServer::getCommandFromCommandData]Eroare la write() catre client.\n");
        gameStatus = DONE;
        winner = client1 ^ client2 ^ clientDescriptor;
    }

lastSender = clientDescriptor;
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
