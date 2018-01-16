#include "GameAPI.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>

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

    drawGameScreen();

    if (status == HOST)
        {
            //cout<<"[GameManager::playGame()] The game has started. I am the host. I move first.\n";
            playerStatus = HOST;
            gameGUI->getMenu("game")->setButtonText("status","Move");
        }
        else
        {
            playerStatus= GUEST;
            //cout<<"[GameManager::playGame()] The game has started. I am the host. I move second.\n";
            gameGUI->getMenu("game")->setButtonText("status","Wait");
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
            gameGUI->getMenu("game")->setButtonText("status","Wait");
            drawGameScreen();

            response = getResponseFromServer();

            if (gameGUI->getMenu("game")->getButtonByName("status")->getText() == "Wait" ||
                gameGUI->getMenu("game")->getButtonByName("status")->getText() == "Click here to promote or right click not to.")
                gameGUI->getMenu("game")->setButtonText("status","Move");

            drawGameScreen();

            whoMovesNow = !playerStatus;

            updateGameState(response);

            if ( gameStatus == DONE )
                break;

            moveString = getMoveFromUser();
            whoMovesNow = playerStatus;
            updateGameState(moveString);

            if ( gameLogic->isKingInCheck(gameLogic->getKingPosition(UP)) )
                {
                    //cout<<"[GameManager::getMoveFromUser()] King was in check and not moved. I am sending a lose command.\n";
                    gameStatus = DONE;
                    gameResult = DEFEAT;
                    moveString.append(" lose ");
                }

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
              exit(1);
              //return errno;
            }

    response[responseLength] = 0;

    result = string(response);

    //cout<<"[GameManager::getResponseFromServer()] I have received this response from server: "<<result<<" with length "<<responseLength<<'\n';

    return result;
}

string GameManager::getMoveFromUser() {

    //cout<<"[GameManager::getMoveFromUser()] I am about to receive a move from the player.\n";

    string result;
    //char userInput[MAX_COMMAND_LENGTH];
    //vector<string> piecesName;

    sf::RenderWindow* window = gameGUI->getWindow();
    int lin,col;
    int lin2,col2;
    int pos,i;
    bool isPieceClicked = false;
    bool shouldHighlight = false;
    bool isHandPieceClicked = false;
    bool shouldPromptPromotion = false;
    string pieceName;
    GameBoard* gameBoard = getGameBoard();
    unordered_set<Position*> highlightedPositions;
    vector<string> handPieces = gameLogic->getUpHandPieces();

    gameGUI->clearEventQueue();

    drawGameScreen();
    //cout<<"[GameManager::getMoveFromUser()] Am ajuns aici.\n";

    while (window->isOpen())
        {
            sf::Event event;
            while (window->pollEvent(event))
            {

                if (event.type == sf::Event::Closed)
                            {
                                gameGUI->getWindow()->close();
                                return "quit";
                            }

                if (event.mouseButton.button == sf::Mouse::Right && (gameGUI->getMenu("game")->getButtonByName("status")->getText() != "Wait"))
                    {

                        //cout<<"[GameManager::getMoveFromUser()] First click detected.\n";

                        if (shouldPromptPromotion)
                            {
                                //result.append(" promote");
                                return result;
                            }


                        if (gameGUI->isBoardClicked(event.mouseButton.x,event.mouseButton.y) /*&& !isPieceClicked*/)
                            {
                                //result = "move ";
                                col = gameGUI->getBoardClickedPositionX(event.mouseButton.x,event.mouseButton.y);
                                lin = gameGUI->getBoardClickedPositionY(event.mouseButton.x,event.mouseButton.y);
                                //cout<<"[GameManager::getMoveFromUser()] Board is clicked at this position "<<lin<<' '<<col<<'\n';

                                if (
                                        gameBoard->getPieceAtPosition(lin,col) == NULL /*||
                                        (
                                            gameBoard->getPieceAtPosition(lin,col) != NULL &&
                                            gameGUI->getMenu("game")->getButtonByName("status")->getText() == "CHECKED!" &&
                                            gameBoard->getPieceAtPosition(lin,col)->getName() != "king"
                                        )*/
                                    )
                                    {
                                        isPieceClicked = false;
                                        shouldHighlight = false;
                                        isHandPieceClicked = false;
                                        continue;
                                    }

                                if (gameBoard->getPieceAtPosition(lin,col)->getOrientation() == DOWN)
                                    {
                                        isPieceClicked = false;
                                        shouldHighlight = false;
                                        isHandPieceClicked = false;
                                        continue;
                                    }

                                isPieceClicked = true;
                                //highlightPositions(BOARD,{ {lin,col} });
                                //cout<<"[GameManager::getMoveFromUser()] I am about to highlight positions.\n";
                                highlightedPositions = gameLogic->getAllPossibleMovementLocationsForPieceFrom({lin,col});
                                //highlightedPositions.insert(&selectedPiecePosition);
                                highlightPositions(BOARD,highlightedPositions);
                                shouldHighlight = true;

                                //cout<<"[GameManager::getMoveFromUser()] I am done highlighting positions.\n";

                                //cout<<"[GameManager::getMoveFromUser()] I am done treating the second click.\n";

                                //for (auto it : highlightedPositions)
                                 //   delete it;

                            }



                    if (gameGUI->isHandClicked(event.mouseButton.x,event.mouseButton.y))
                        {
                            pos = gameGUI->getHandClickedPiecePosition(event.mouseButton.x,event.mouseButton.y);
                            pieceName = "not";

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

                            if (pieceName == "not")
                                {
                                    isHandPieceClicked = false;
                                    isPieceClicked = false;
                                    shouldHighlight = false;
                                    continue;
                                }

                            highlightedPositions = gameBoard->getDropablePositions(pieceName,UP);
                            highlightPositions(BOARD,highlightedPositions);
                            shouldHighlight = true;
                            isHandPieceClicked = true;

                            //for (auto it : highlightedPositions)
                             //   delete it;
                        }
                    }

                if (event.mouseButton.button == sf::Mouse::Left && (gameGUI->getMenu("game")->getButtonByName("status")->getText() != "Wait"))
                    {

                        if(gameGUI->getMenu("game")->isButtonPressed("forfeit",event.mouseButton.x,event.mouseButton.y))
                            return string("quit");

                        if (gameGUI->getMenu("game")->isButtonPressed("status",event.mouseButton.x,event.mouseButton.y) && shouldPromptPromotion)
                            {
                                result.append(" promote");
                                return result;
                            }

                        if ( gameGUI->isBoardClicked(event.mouseButton.x,event.mouseButton.y) && isPieceClicked  )
                            {

                                //cout<<"[GameManager::getMoveFromUser()] Second click detected.\n";
                                if (gameGUI->isBoardClicked(event.mouseButton.x,event.mouseButton.y))
                                    {
                                        //cout<<"[GameManager::getMoveFromUser()] Board was clicked a second time.\n";
                                        col2 = gameGUI->getBoardClickedPositionX(event.mouseButton.x,event.mouseButton.y);
                                        lin2 = gameGUI->getBoardClickedPositionY(event.mouseButton.x,event.mouseButton.y);

                                         //cout<<"[GameManager::getMoveFromUser()] Board is clicked at this position "<<lin2<<' '<<col2<<'\n';

                                        if (lin == lin2 && col == col2) /*||
                                             (getGameBoard()->getPieceAtPosition(lin2,col2) != NULL && getGameBoard()->getPieceAtPosition(lin2,col2)->getName() == "king"*/
                                            {
                                            isPieceClicked = false;
                                            shouldHighlight = false;
                                            continue;
                                            }

                                        /*if ( gameLogic->isKingInCheck(gameLogic->getKingPosition(UP)) )
                                            {
                                                cout<<"[GameManager::getMoveFromUser()] King was in check and not moved. I am sending a lose command.\n";
                                                gameStatus = DONE;
                                                gameResult = DEFEAT;
                                                result = string("move ") + to_string(lin) + " " + to_string(col) + " " + to_string(lin2) + " " + to_string(col2);
                                                result.append(" lose ");
                                                return result;
                                                break;
                                            }*/

                                        for (auto it : highlightedPositions)
                                            if (it->lin == lin2 && it->col == col2)
                                                {
                                                    result = string("move ") + to_string(lin) + " " + to_string(col) + " " + to_string(lin2) + " " + to_string(col2);

                                                    //cout<<"[GameManager::getMoveFromUser()] I am returning this result: "<<result<<'\n';
                                                    if (gameLogic->getGameBoard()->isPromotionZone({lin2,col2},gameLogic->getGameBoard()->getPieceAtPosition(lin,col)->getOrientation()) &&
                                                        gameLogic->getGameBoard()->getPieceAtPosition(lin,col)->getPromotionStatus() == UNPROMOTED
                                                        )
                                                        {
                                                            gameGUI->getMenu("game")->setButtonText("status","Click here to promote or right click not to.");
                                                            shouldPromptPromotion = true;
                                                            drawGameScreen();
                                                            continue;
                                                        }
                                                        else
                                                        {
                                                            drawGameScreen();
                                                            return result;
                                                        }

                                                }
                                    }

                            }

                        if (gameGUI->isBoardClicked(event.mouseButton.x,event.mouseButton.y) && isHandPieceClicked && !shouldPromptPromotion)
                            {
                                col2 = gameGUI->getBoardClickedPositionX(event.mouseButton.x,event.mouseButton.y);
                                lin2 = gameGUI->getBoardClickedPositionY(event.mouseButton.x,event.mouseButton.y);

                                for (auto it : highlightedPositions)
                                    if (it->lin == lin2 && it->col == col2)
                                        {
                                            result = string("drop ") + pieceName + " " + to_string(lin2) + " " + to_string(col2);
                                            //cout<<"[GameManager::getMoveFromUser()] I am returning this result: "<<result<<'\n';
                                            return result;
                                        }
                            }

                        isPieceClicked = false;
                        isHandPieceClicked = false;
                        shouldHighlight = false;

                    }

            }

            gameGUI->drawGameScreen();
            drawPieces();
            if (shouldHighlight)
                highlightPositions(BOARD,highlightedPositions);
            gameGUI->getWindow()->display();
        }

}


void GameManager::sendMoveToServer(string moveString) {

    char* moveToServer = new char[moveString.size()+1];
    copy(moveString.begin(),moveString.end(),moveToServer);
    moveToServer[moveString.size()]=0;

    //cout<<"[GameManager::sendMoveToServer()] I am sending my move to the server: "<<moveToServer<<" with length "<<moveString.size()<<'\n';

    if (write (serverSocket, moveToServer, moveString.size()) <= 0)
        {
        perror ("[Thread]Eroare la write() catre server.\n");
        exit(1);
        }
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
            //cout<<"[GameManager::solveCommand()] I am solving a move command.\n";
            gameLogic->movePiece(command->from,command->to,whoMovesNow);

            if (command->isPromotion)
                gameLogic->promotePiece(command->to);
        }

    if (command->moveType == "drop")
        {
         //cout<<"[GameManager::solveCommand()] I am solving a drop command.\n";
         gameLogic->dropPiece(command->droppedPieceName,command->to,playerStatus,whoMovesNow);
        }

}

void GameManager::updateGameState(string command) {

    //cout<<"[GameManager::updateGameState()] I am updating game state using command from server: "<<command<<'\n';
    CommandData* commandData = extractDataFromCommand(command);
    solveCommand(commandData);

    if ( commandData->isCheck )
        {
        warnCheck();
        //isKingInCheck = true;
        }
        //else
        //isKingInCheck = false;

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
     drawGameScreen();
 }

 void GameManager::displayEndGameScreen(bool result) {

    if (result == VICTORY)
        gameGUI->getMenu("game")->getButtonByName("status")->setText("Winner!");
        //cout<<"You have won the game!\n";
        else
        gameGUI->getMenu("game")->getButtonByName("status")->setText("Defeated!");
        //cout<<"You have lost the game!\n";

    gameGUI->getMenu("game")->setButtonText("forfeit","BACK");
    drawGameScreen();
 }


 GameBoard* GameManager::getGameBoard() {

    return gameLogic->getGameBoard();
 }


 void GameManager::drawPieces() {

    //cout<<"[GameManager::drawPieces()] I am board drawing pieces.\n";

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
                        spriteToDraw.setPosition(boardOriginX + (j-1)*pieceOffsetX + pieceOffsetX/2,boardOriginY+ (i-1)*pieceOffsetY + pieceOffsetY/2);
                        gameGUI->getWindow()->draw(spriteToDraw);

                        sprites.clear();
                    }
            }

    drawPlayerHand(UP);
    drawPlayerHand(DOWN);

 }

 void GameManager::drawPlayerHand(short int orientation) {

    //cout<<"[GameManager::drawPieces()] I am drawing pieces in hand of player with orientation: "<<orientation<<'\n';

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
            spriteToDraw.setPosition(handPositionX + (i-1)*pieceOffsetX + pieceOffsetX/2,handPositionY+ (j-1)*pieceOffsetY + pieceOffsetY/2);
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

void GameManager::highlightPositions(int where,unordered_set<Position*> positions) {

    //cout<<"[GameManager::highlightPositions] I am highlighting positions: \n";

    int pieceOffsetX = gameGUI->getPieceOffsetX();
    int pieceOffsetY = gameGUI->getPieceOffsetY();

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(pieceOffsetX,pieceOffsetY));
    rectangle.setFillColor(sf::Color(0,153,0,128));
    vector<Position*> positionsToRemove;

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
        if ( (getGameBoard()->getPieceAtPosition(*it) == NULL) ||
            ( getGameBoard()->getPieceAtPosition(*it) != NULL && getGameBoard()->getPieceAtPosition(*it)->getName() != "king") )
        {
            rectangle.setPosition( originX + (it->col-1)*pieceOffsetX, originY + (it->lin-1)*pieceOffsetY );
            gameGUI->getWindow()->draw(rectangle);
        }
        /*else
        if ( getGameBoard()->getPieceAtPosition(*it) != NULL && getGameBoard()->getPieceAtPosition(*it)->getName() == "king")
            {
                cout<<"[GameManager::highlightPositions] I am deleting this position because there is a king there: "<<it->lin<<' '<<it->col<<'\n';
                //positions.erase( positions.find(it) );
                positionsToRemove.push_back(it);
            }*/


    //gameGUI->getWindow()->display();
}


void GameManager::drawGameScreen() {

    gameGUI->drawGameScreen();
    drawPieces();
    gameGUI->getWindow()->display();
}
