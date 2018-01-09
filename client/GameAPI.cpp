#include "GameAPI.h"
#include <iostream>

using namespace std;

GameManager::GameManager(int serverSocket) {

    gameLogic = new GameLogic();
    gameStatus = PROGRESS;
    gameResult = DEFEAT;
    this->serverSocket = serverSocket;
}

bool GameManager::playGame(bool status) {

    string moveString, response;
    if (status == HOST)
        {
            cout<<"[GameManager::playGame()] The game has started. I am the host. I move first.\n";
            playerStatus = HOST;
        }
        else
        {
            playerStatus= GUEST;
            cout<<"[GameManager::playGame()] The game has started. I am the host. I move second.\n";
        }

    if (status == HOST)
        {
            moveString = getMoveFromUser();
            /*if (moveString == "quit")
                {
                    gameStatus = DONE;
                    gameResult = DEFEAT;
                }*/
            whoMovesNow = playerStatus;
            updateGameState(moveString);
            sendMoveToServer(moveString);
        }

    while (gameStatus !=DONE )
        {
            response = getResponseFromServer();
            whoMovesNow = !playerStatus;

            updateGameState(response);

            if ( gameStatus == DONE )
                break;

            moveString = getMoveFromUser();
            whoMovesNow = playerStatus;
            updateGameState(moveString);

            sendMoveToServer(moveString);

            /*if (moveString == "quit")
                {
                    gameStatus = DONE;
                    gameResult = DEFEAT;
                    break;
                }*/
        }

    return gameResult;
}

string GameManager::getResponseFromServer() {

    string result;
    int responseLength;
    char response[MAX_RESPONSE_LENGTH];

    if ( (responseLength = read (serverSocket, &response,MAX_RESPONSE_LENGTH) ) < 0)
            {
              perror ("[client]Eroare la read() de la server.\n");
              //return errno;
            }

    response[responseLength] = 0;

    result = string(response);

    cout<<"[GameManager::getResponseFromServer()] I have received this response from server: "<<result<<" with length "<<responseLength<<'\n';

    return result;
}

string GameManager::getMoveFromUser() {

    char userInput[MAX_COMMAND_LENGTH];
    vector<string> piecesName;

    cout<<"You have the following pieces in your hand: \n";
    piecesName = gameLogic->getUpHandPieces();

    for (auto it: piecesName)
        cout<<it<<'\n';

    piecesName.clear();

    cout<<"Your enemy has the following pieces in their hand: \n";

    piecesName = gameLogic->getDownHandPieces();

    for (auto it: piecesName)
        cout<<it<<'\n';

    piecesName.clear();

    cout<<"[Client: Game] Enter move: ";
    //cin>>userInput;
    cin.getline(userInput,MAX_COMMAND_LENGTH);

    return string(userInput);
}


void GameManager::sendMoveToServer(string moveString) {

    char* moveToServer = new char[moveString.size()+1];
    copy(moveString.begin(),moveString.end(),moveToServer);
    moveToServer[moveString.size()]=0;

    cout<<"[GameManager::sendMoveToServer()] I am sending my move to the server: "<<moveToServer<<" with length "<<moveString.size()<<'\n';

    if (write (serverSocket, moveToServer, moveString.size()) <= 0)
        perror ("[Thread]Eroare la write() catre client.\n");
}

CommandData* GameManager::extractDataFromCommand(string command){

    string moveString = "move";
    string promotionString = "promote";
    string dropString = "drop";
    string checkString = "check";
    string checkMateString = "checkmate";
    string winString = "win";
    string loseString = "lose";
    string pieceNameAndPosition;
    string pieceName;
    string piecePosition;
    CommandData* result = new CommandData;

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
    if ( command.find(checkString) != std::string::npos )
        result->isCheck = true;
    if ( command.find(checkMateString) != std::string::npos )
        result->isCheckMate = true;
    if ( command.find(winString) != std::string::npos )
        result->isWin = true;
    if ( command.find(loseString) != std::string::npos )
        result->isLose = true;

    return result;
}

void GameManager::solveCommand(CommandData* command) {

    if (command->moveType == "move")
        {
            cout<<"[GameManager::solveCommand()] I am solving a move command.\n";
            gameLogic->movePiece(command->from,command->to,whoMovesNow);

            if (command->isPromotion)
                gameLogic->promotePiece(command->to);
        }

    if (command->moveType == "drop")
        {
         cout<<"[GameManager::solveCommand()] I am solving a drop command.\n";
         gameLogic->dropPiece(command->droppedPieceName,command->to,playerStatus,whoMovesNow);
        }

}

void GameManager::updateGameState(string command) {

    cout<<"[GameManager::updateGameState()] I am updating game state using command from server: "<<command<<'\n';
    CommandData* commandData = extractDataFromCommand(command);
    solveCommand(commandData);

    if ( commandData->isCheck )
        warnCheck();

    if ( commandData->isCheckMate )
        {
            gameStatus = DONE;
            if (commandData->isWin)
                gameResult = VICTORY;
                else
                gameResult = DEFEAT;

            return;
        }

    if ( commandData->isWin )
        {
        gameResult = VICTORY;
        gameStatus = DONE;
        }
    if ( commandData->isLose )
        {
        gameResult = DEFEAT;
        gameStatus = DONE;
        }
}

 void GameManager::warnCheck() {

 cout<<"You are in check.\n";
 }

 void GameManager::displayEndGameScreen(bool result) {

    if (result == VICTORY)
        cout<<"You have won the game!\n";
        else
        cout<<"You have lost the game!\n";
 }
