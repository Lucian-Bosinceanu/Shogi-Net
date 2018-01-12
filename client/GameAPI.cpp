#include "GameAPI.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

GameManager::GameManager(int serverSocket,GameGUI* gameGUI) {

    gameLogic = new GameLogic();
    gameStatus = PROGRESS;
    gameResult = DEFEAT;
    this->serverSocket = serverSocket;
    this->gameGUI = gameGUI;
}

bool GameManager::playGame(bool status) {

    string moveString, response;
    sf::RenderWindow* window = gameGUI->getWindow();

    window->clear(sf::Color::White);
    gameGUI->drawMenu("game");
    gameGUI->drawBoard();
    drawPieces();
    gameGUI->getWindow()->display();


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

    string result;
    //char userInput[MAX_COMMAND_LENGTH];
    //vector<string> piecesName;

    sf::RenderWindow* window = gameGUI->getWindow();
    sf::Event event;
    window->clear(sf::Color::White);
    gameGUI->drawMenu("game");
    gameGUI->drawBoard();
    drawPieces();
    gameGUI->getWindow()->display();
    int lin,col;
    int lin2,col2;
    int pos,i;
    string pieceName;
    GameBoard* gameBoard = getGameBoard();
    vector<Position*> highlightedPositions;
    vector<string> handPieces = gameLogic->getUpHandPieces();

    while (window->isOpen())
        {

            while (window->pollEvent(event))
            {

                if (event.mouseButton.button == sf::Mouse::Left)
                    {

                    if(gameGUI->getMenu("game")->isButtonPressed("forfeit",event.mouseButton.x,event.mouseButton.y))
                        return string("quit");


                    if (gameGUI->isBoardClicked(event.mouseButton.x,event.mouseButton.y))
                        {
                            //result = "move ";

                            col = gameGUI->getBoardClickedPositionX(event.mouseButton.x,event.mouseButton.y);
                            lin = gameGUI->getBoardClickedPositionY(event.mouseButton.x,event.mouseButton.y);

                            if (gameBoard->getPieceAtPosition(lin,col) == NUL)
                                continue;

                            if (gameBoard->getPieceAtPosition(lin,col)->getOrientation() == DOWN)
                                continue;

                            highlightPositions(BOARD,{ {lin,col} });
                            highlightedPositions = gameBoard->getAllPossibleMovementLocationsForPieceFrom(lin,col);
                            highlightPositions(BOARD,highlightedPositions);

                            while (window->pollEvent(event))
                                   {
                                        if (event.mouseButton.button == sf::Mouse::Left)
                                            {

                                                if (gameGUI->isBoardClicked(event.mouseButton.x,event.mouseButton.y))
                                                    {
                                                        col2 = gameGUI->getBoardClickedPositionX(event.mouseButton.x,event.mouseButton.y);
                                                        lin2 = gameGUI->getBoardClickedPositionY(event.mouseButton.x,event.mouseButton.y);

                                                        if (lin == lin2 && col == col2)
                                                            continue;

                                                        for (auto it : highlightedPositions)
                                                            if (it->lin == lin2 && it->col == col2)
                                                                {
                                                                    result = string("move ") + to_string(lin) + " " + to_string(col) + " " + to_string(lin2) + " " + to_string(col2);
                                                                    return result;
                                                                }
                                                    }

                                                break;
                                            }


                                   }

                            for (auto it : highlightedPositions)
                                delete *it;

                        }

                    if (gameGUI->isHandClicked(event.mouseButton.x,event.mouseButton.y))
                        {
                            pos = gameGUI->getHandClickedPiecePosition(event.mouseButton.x,event.mouseButton.y);

                            i = 0;
                            for (auto it : handPieces)
                                {
                                    if (i == pos)
                                        {
                                            pieceName = it;
                                            break;
                                        }

                                    i++;
                                }

                            highlightedPositions = gameBoard->getDropablePositions(pieceName,UP);
                            //highlightPositions(HAND,{ {lin,col} });
                            highlightPositions(BOARD,highlightedPositions);

                            while (window->pollEvent(event))
                                   {
                                        if (event.mouseButton.button == sf::Mouse::Left)
                                            {

                                                if (gameGUI->isBoardClicked(event.mouseButton.x,event.mouseButton.y))
                                                    {
                                                        col2 = gameGUI->getBoardClickedPositionX(event.mouseButton.x,event.mouseButton.y);
                                                        lin2 = gameGUI->getBoardClickedPositionY(event.mouseButton.x,event.mouseButton.y);

                                                        for (auto it : highlightedPositions)
                                                            if (it->lin == lin2 && it->col == col2)
                                                                {
                                                                    result = string("drop ") + pieceName + " " + to_string(lin2) + " " + to_string(col2);
                                                                    return result;
                                                                }
                                                    }

                                                break;
                                            }


                                   }

                            for (auto it : highlightedPositions)
                                delete *it;
                        }
                    }

            }
        }

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

// cout<<"You are in check.\n";
 gameGUI->getMenu("game")->getButtonByName("status")->setText("CHECK!");
 }

 void GameManager::displayEndGameScreen(bool result) {

    if (result == VICTORY)
        gameGUI->getMenu("game")->getButtonByName("status")->setText("WINNER!");
        //cout<<"You have won the game!\n";
        else
        gameGUI->getMenu("game")->getButtonByName("status")->setText("DEFEATED!");
        //cout<<"You have lost the game!\n";
 }


 GameBoard* GameManager::getGameBoard() {

    return gameLogic->getGameBoard();
 }


 void GameManager::drawPieces() {

    int i,j;
    int pieceOffsetX = gameGUI->getPieceOffsetX();
    int pieceOffsetY = gameGUI->getPieceOffsetY();
    int boardOriginX = gameGUI->getBoardPositionX();
    int boardOriginY = gameGUI->getBoardPositionY();

    ShogiPiece* piece;
    string pieceName;
    GameBoard* gameBoard = getGameBoard();
    vector<sf::Sprite> sprites;
    sf::Sprite spriteToDraw;
    map< string,vector<sf::Sprite> > piecesSprites = gameGUI->getPiecesSprites();

    for (i=1;i<=9;i++)
        for (j=1;j<=9;j++)
            {
                piece = gameBoard->getPieceAtPosition(i,j);
                if (piece!=NULL)
                    {
                        pieceName = piece->getName();
                        sprites = piecesSprites.find(pieceName)->second;

                        //for (auto it = piecesSprites.find(pieceName).second.begin() ; it <= piecesSprites.find(pieceName).second.end();i++)
                        //    sprites.push_back(it);

                        if (pieceName!="king")
                            spriteToDraw = sprites[piece->getPromotionStatus()];
                            else
                            {
                            if (piece->getOrientation() == UPPLAYER)
                                spriteToDraw = sprites[0];
                                else
                                spriteToDraw = sprites[1];
                            }

                        spriteToDraw.rotate( (piece->getOrientation() == UPPLAYER) ? 90 : 270 );
                        spriteToDraw.setPosition(boardOriginX + (j-1)*pieceOffsetX,boardOriginY+ (i-1)*pieceOffsetY);
                        gameGUI->getWindow()->draw(spriteToDraw);

                        sprites.clear();
                    }
            }

    drawPlayerHand(UP);
    drawPlayerHand(DOWN);

 }

 void GameManager::drawPlayerHand(short int orientation) {

    int angle,positions=0;
    int i=1,j=1;
    int pieceOffsetX = gameGUI->getPieceOffsetX();
    int pieceOffsetY = gameGUI->getPieceOffsetY();
    int handPositionX;
    int handPositionY;
    vector<string> pieces;
    vector<sf::Sprite> sprites;
    sf::Sprite spriteToDraw;
    map< string,vector<sf::Sprite> > piecesSprites = gameGUI->getPiecesSprites();

    if (orientation == UP)
        {
        angle = 90;
        pieces = gameLogic->getUpHandPieces();
        handPositionX = gameGUI->getUpHandPositionX();
        handPositionY = gameGUI->getUpHandPositionY();
        }
        else
        {
        angle = 270;
        pieces = gameLogic->getDownHandPieces();
        handPositionX = gameGUI->getDownHandPositionX();
        handPositionY = gameGUI->getDownHandPositionY();
        }

    for (auto it : pieces)
        {
            positions++;
            sprites = piecesSprites.find(it)->second;
            spriteToDraw = sprites[0];
            spriteToDraw.rotate(angle);
            spriteToDraw.setPosition(handPositionX + (j-1)*pieceOffsetX,handPositionY+ (i-1)*pieceOffsetY);
            gameGUI->getWindow()->draw(spriteToDraw);

            i++;
            if (i == 5)
                {
                    i = 1;
                    j++;
                }

            if (positions == 16)
                return;
        }

}

void GameManager::highlightPositions(int where,vector<Position*> positions) {

    sf::RectangleShape rectangle;
    rectangle.setFillColor(sf::Color(0,153,0,128));

    int pieceOffsetX = gameGUI->getPieceOffsetX();
    int pieceOffsetY = gameGUI->getPieceOffsetY();

    int originX;
    int originY;

    if (where == HAND)
        {
        originX = gameGUI->getUpHandPositionX();
        originY = gameGUI->getUpHandPositionY();
        }

    if (where == BOARD)
        {
        originX = gameGUI->getBoardPositionX();
        originY = gameGUI->getBoardPositionY();
        }

    for (auto it : positions)
        if ( getGameBoard()->getPieceAtPosition(*it)->getName() != "king" )
        {
            rectangle.setPosition( originX + (it.col-1)*pieceOffsetX, originY + (it.lin-1)*pieceOffsetY );
            gameGUI->getWindow()->draw(rectangle);
        }
}
