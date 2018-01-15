#include "GameLogic.h"
#include <iostream>

using namespace std;

GameLogic::GameLogic() {

    gameBoard = new GameBoard();
    upKingPosition = {9,5};
    downKingPosition = {1,5};

}

GameLogic::~GameLogic() {

    gameBoard->~GameBoard();
}



void GameLogic::dropPiece(string pieceName, Position dropPosition, bool playerStatus, bool whoMovesNow) {

    ShogiPiece* pieceToDrop = gameBoard->removePieceFromHand(playerStatus,whoMovesNow,pieceName);
    gameBoard->dropPiece(pieceToDrop,dropPosition);

}


void GameLogic::movePiece(Position from, Position to, bool playerStatus) {

cout<<"[GameLogic::movePiece()] I am trying to move the piece from "<<from.lin<<' '<<from.col<<" to "<<to.lin<<' '<<to.col<<'\n';

//short int side = playerOrientation;

cout<<"[GameLogic::movePiece()] I am about to move a "<<gameBoard->getPieceAtPosition(from)->getName()<<" with orientation "<<playerStatus<<'\n';


    if (gameBoard->getPieceAtPosition(to) == NULL)
        gameBoard->movePiece(from,to);
        else
        if ( gameBoard->getPieceAtPosition(from)->getOrientation() != gameBoard->getPieceAtPosition(to)->getOrientation() )
            {
      //      cout<<"[GameLogic::movePiece] I am going to capture the piece from "<<to.lin<<' '<<to.col<<'\n';
            capturePiece(to, playerStatus);
            gameBoard->movePiece(from,to);

            }

    if (gameBoard->getPieceAtPosition(to)->getName() == "king")
    {
        if (gameBoard->getPieceAtPosition(to)->getOrientation() == UP)
            {
            upKingPosition = to;
            cout<<"[GameLogic::movePiece] I am setting the Up king position to: "<<to.lin<<' '<<to.col<<'\n';
            }
            else
            {
            downKingPosition = to;
            cout<<"[GameLogic::movePiece] I am setting the Down king position to: "<<to.lin<<' '<<to.col<<'\n';
            }
    }

//cout<<"[GameLogic::movePiece()] Done.\n";

}

void GameLogic::capturePiece(Position capturedPiece, bool playerStatus) {

    //cout<<"[GameLogic::capturePiece] I am about to capture the piece from "<<capturedPiece.lin<<' '<<capturedPiece.col<<'\n';
    ShogiPiece* capturedPiecePointer = gameBoard->getPieceAtPosition(capturedPiece);

    //cout<<"[GameLogic::capturePiece] It's a "<<capturedPiecePointer->getName()<<" with orientation "<<capturedPiecePointer->getOrientation()<<'\n';
    //cout<<"[GameLogic::capturePiece] I am adding this piece to this player hand: "<<-1*capturedPiecePointer->getOrientation()<<'\n';
    gameBoard->addPieceInHand(playerStatus,capturedPiecePointer);

    capturedPiecePointer->setCaptured();
}

void GameLogic::promotePiece(Position promotedPiece) {

    gameBoard->getPieceAtPosition(promotedPiece)->promote();
}

vector<string> GameLogic::getUpHandPieces() {

    return gameBoard->getHandPiecesOfPlayer(UP);
}

vector<string> GameLogic::getDownHandPieces() {

    return gameBoard->getHandPiecesOfPlayer(DOWN);
}

GameBoard* GameLogic::getGameBoard() {

    return gameBoard;
}

bool GameLogic::isCheckFromPiece(ShogiPiece* attackingPiece, Position kingPosition) {

    //cout<<"[GameLogic::isCheckFromPiece] I am checking if the piece from "<<attackingPiece->getPosition().lin<<' '<<attackingPiece->getPosition().col<<" is checking the king.\n";

    //cout<<"[GameLogic::isCheckFromPiece] I am getting opposing king orientation.\n";
    short int opposingKingOrientation = -1*attackingPiece->getOrientation();
    //cout<<"[GameLogic::isCheckFromPiece] Opposing king orientation is "<<opposingKingOrientation<<'\n';

    //cout<<"[GameLogic::isCheckFromPiece] I am getting opposing king position \n";
    Position opposingKingPosition = kingPosition;
    //cout<<"[GameLogic::isCheckFromPiece] Opposing king is at: "<<opposingKingPosition.lin<<' '<<opposingKingPosition.col<<'\n';

    //cout<<"[GameLogic::isCheckFromPiece] I am getting the next possible moves of attacking piece.\n";
    unordered_set<Position*> possibleFutureMovements = gameBoard->getAllPossibleMovementLocationsForPieceFrom(attackingPiece->getPosition());

    /*cout<<"[GameLogic::isCheckFromPiece] The attacking piece can move at: ";
    for (auto it : possibleFutureMovements)
        cout<<it->lin<<' '<<it->col<<'\n';*/

    for (auto it : possibleFutureMovements)
        if (it->lin == opposingKingPosition.lin && it->col == opposingKingPosition.col)
            return true;

    //cout<<"[GameLogic::IsCheckFromPiece] The king is not checked by this piece.\n";
    return false;

    /*if ( possibleFutureMovements.find(&opposingKingPosition) != possibleFutureMovements.end() )
        cout<<"[GameLogic::isCheckFromPiece] The "<<attackingPiece->getName()<<" from position "<<attackingPiece->getPosition().lin<<' '<<attackingPiece->getPosition().col<<
        " has checked the opposing King!\n";
    */
    //return possibleFutureMovements.find(&opposingKingPosition) != possibleFutureMovements.end();

}

bool GameLogic::isCheckFromPiece(Position attackingPiecePosition) {

    /*cout<<"[GameLogic::isCheckFromPiece] I am checking if the piece from "<<attackingPiecePosition.lin<<' '<<attackingPiecePosition.col<<" is checking the king.\n";
    ShogiPiece* attackingPiece = gameBoard->getPieceAtPosition(attackingPiecePosition);
    return isCheckFromPiece(attackingPiece);*/
}

unordered_set<Position*> GameLogic::getAllPossibleMovementLocationsForPieceFrom(Position piecePosition){

    if (gameBoard->getPieceAtPosition(piecePosition)->getName() != "king")
        return gameBoard->getAllPossibleMovementLocationsForPieceFrom(piecePosition);

    unordered_set<Position*> result;
    ShogiPiece* piece = gameBoard->getPieceAtPosition(piecePosition);
    Position possiblePosition;
    short int pieceOrientation = piece->getOrientation();
    vector<Position> movementRules = piece->getMovementRules(piece->getPromotionStatus());
    Position* elementToInsert;
    Position initialKingPosition = piecePosition;
    short int kingOrientation = piece->getOrientation();
    bool isValid = false;
    bool reverseMove = false;
    bool isPromoted = false;
    bool kingChecked;
    ShogiPiece* possibleCapture;
    string possibleCapturedPieceName;
    int i;

    unordered_set<ShogiPiece*> enemyPieces = gameBoard->getPiecesOfPlayer(-1*kingOrientation);

    if (piece->getName() == "king")
        {
            for (i = 0; i < movementRules.size();i++)
                {
                    possiblePosition.lin = piecePosition.lin + pieceOrientation*movementRules[i].lin;
                    possiblePosition.col = piecePosition.col + pieceOrientation*movementRules[i].col;

                    if (!gameBoard->isPositionOnBoard(possiblePosition))
                        continue;

                    possibleCapture = gameBoard->getPieceAtPosition(possiblePosition);

                    if (possibleCapture != NULL && possibleCapture->getOrientation() == kingOrientation)
                        continue;

                    reverseMove = false;
                    kingChecked = false;
                    isPromoted = false;

                    cout<<"[GameLogic::getAllPossibleMovementLocationsForPieceFrom] Analysing position "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
                    if (possibleCapture != NULL && possibleCapture->getOrientation() != kingOrientation)
                        {
                        if (possibleCapture->getPromotionStatus() == PROMOTED)
                            isPromoted = true;
                        possibleCapturedPieceName= possibleCapture->getName();
                        reverseMove = true;
                        enemyPieces.erase(possibleCapture);
                        cout<<"[GameLogic::getAllPossibleMovementLocationsForPieceFrom] There is an enemy "<<possibleCapturedPieceName<<" there.\n";
                        }

                    cout<<"[GameLogic::getAllPossibleMovementLocationsForPieceFrom] I am moving the king to the position from above.\n";
                    movePiece(initialKingPosition,possiblePosition,kingOrientation);

                    for (auto piece : enemyPieces)
                        if ( isCheckFromPiece(piece,possiblePosition) )
                            {
                             cout<<"[GameLogic::getAllPossibleMovementLocationsForPieceFrom] The king is checked here by a "<<piece->getName()<<" from position "<<piece->getPosition().lin<<' '<<piece->getPosition().col<<'\n';
                             cout<<"[GameLogic::getAllPossibleMovementLocationsForPieceFrom] The king is at position "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
                             kingChecked = true;
                             break;
                            }

                    movePiece(possiblePosition,initialKingPosition,kingOrientation);

                    if (reverseMove)
                        {
                            dropPiece(possibleCapturedPieceName,possiblePosition,kingOrientation, kingOrientation);
                            possibleCapture->switchOrientation();
                            enemyPieces.insert(possibleCapture);
                            if (isPromoted)
                                possibleCapture->promote();
                        }

                    if (!kingChecked)
                        {
                            elementToInsert = new Position{possiblePosition.lin,possiblePosition.col};
                            result.insert(elementToInsert);
                        }

                }
        }

    return result;
}


Position GameLogic::getKingPosition(short int orientation) {

    cout<<"[GameLogic::getKingPosition] I am getting king position with orientation: "<<orientation<<'\n';

    if (orientation == UP)
        {
            cout<<"[GameLogic::getKingPosition] I am returning position: "<<upKingPosition.lin<<' '<<upKingPosition.col<<'\n';
            return upKingPosition;
        }
        else
        {
            cout<<"[GameLogic::getKingPosition] I am returning position: "<<downKingPosition.lin<<' '<<downKingPosition.col<<'\n';
            return downKingPosition;
        }
}

bool GameLogic::isKingInCheck(Position kingPosition) {

    Position possiblePosition;

    Position initialKingPosition = kingPosition;
    short int kingOrientation = gameBoard->getPieceAtPosition(kingPosition)->getOrientation();
    short int pieceOrientation = kingOrientation;
    bool reverseMove = false;
    bool isPromoted = false;
    bool kingChecked;
    Position piecePosition = kingPosition;

    ShogiPiece* possibleCapture;
    string possibleCapturedPieceName;
    int i;

    vector<Position> movementRules = gameBoard->getPieceAtPosition(kingPosition)->getMovementRules(UNPROMOTED);
    unordered_set<ShogiPiece*> enemyPieces = gameBoard->getPiecesOfPlayer(-1*kingOrientation);;
    unordered_set<Position*> attackedPositions;
    unordered_set<Position*> currentPieceAttackingPositions;

    unordered_set<Position*> whereCurrentFriendlyPieceCanMove;
    unordered_set<Position*> whereFriendsCanMove;
    unordered_set<ShogiPiece*> friendPieces = gameBoard->getPiecesOfPlayer(kingOrientation);

    friendPieces.erase( friendPieces.find(gameBoard->getPieceAtPosition(initialKingPosition)) );

    for (auto it : friendPieces)
        {
            whereCurrentFriendlyPieceCanMove = getAllPossibleMovementLocationsForPieceFrom(it->getPosition());
            whereFriendsCanMove.insert( whereCurrentFriendlyPieceCanMove.begin(), whereCurrentFriendlyPieceCanMove.end() );
            whereCurrentFriendlyPieceCanMove.clear();
        }

for (i = 0; i < movementRules.size();i++)
    {
        possiblePosition.lin = piecePosition.lin + pieceOrientation*movementRules[i].lin;
        possiblePosition.col = piecePosition.col + pieceOrientation*movementRules[i].col;

        if (!gameBoard->isPositionOnBoard(possiblePosition))
            continue;

        possibleCapture = gameBoard->getPieceAtPosition(possiblePosition);

        if (possibleCapture != NULL && possibleCapture->getOrientation() == kingOrientation)
            continue;

        reverseMove = false;
        kingChecked = false;
        isPromoted = false;

        cout<<"[GameLogic::isKingInCheck] Analysing position "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
        if (possibleCapture != NULL && possibleCapture->getOrientation() != kingOrientation)
            {
            if (possibleCapture->getPromotionStatus() == PROMOTED)
                isPromoted = true;
            possibleCapturedPieceName= possibleCapture->getName();
            reverseMove = true;
            enemyPieces.erase(possibleCapture);
            cout<<"[GameLogic::isKingCheckMated] There is an enemy "<<possibleCapturedPieceName<<" there.\n";
            }

        cout<<"[GameLogic::isKingInCheck] I am moving the king to the position from above.\n";
        movePiece(initialKingPosition,possiblePosition,kingOrientation);

        for (auto piece : enemyPieces)
            if ( isCheckFromPiece(piece,possiblePosition) )
                {
                 kingChecked = true;
                 break;
                }

        cout<<"[GameLogic::isKingInCheck] Am ajuns aici.\n";
        movePiece(possiblePosition,initialKingPosition,kingOrientation);

        if (reverseMove)
            {
                dropPiece(possibleCapturedPieceName,possiblePosition,kingOrientation, kingOrientation);
                possibleCapture->switchOrientation();
                enemyPieces.insert(possibleCapture);
                if (isPromoted)
                    possibleCapture->promote();
            }

        if (!kingChecked)
            {
                currentPieceAttackingPositions.clear(); attackedPositions.clear(); whereCurrentFriendlyPieceCanMove.clear(); whereFriendsCanMove.clear(); friendPieces.clear(); enemyPieces.clear(); movementRules.clear();
                return false;
            }

    }

    for (auto piece : enemyPieces)
        if ( isCheckFromPiece(piece,possiblePosition) )
            {
             cout<<"[GameLogic::isKingInCheck] The king is checked here by a "<<piece->getName()<<" from position "<<piece->getPosition().lin<<' '<<piece->getPosition().col<<'\n';
             cout<<"[GameLogic::isKingCheck] The king is at position "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
             kingChecked = true;

             for (auto it : whereFriendsCanMove)
                if ( it->lin == piece->getPosition().lin && it->col == piece->getPosition().col )
                    {
                        return false;
                    }

            if (kingChecked)
                return true;
            }
    currentPieceAttackingPositions.clear(); attackedPositions.clear(); whereCurrentFriendlyPieceCanMove.clear(); whereFriendsCanMove.clear(); friendPieces.clear(); enemyPieces.clear(); movementRules.clear();
    return false;

}
