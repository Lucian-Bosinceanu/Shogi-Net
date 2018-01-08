#include "GameLogic.h"

using namespace std;

GameLogic::GameLogic() {

    gameBoard = new GameBoard();
    upKingPosition = { 9,5 };
    downKingPosition = { 1,5 };
}

GameLogic::~GameLogic() {

    gameBoard->~GameBoard();
}

Position GameLogic::getUpKingPosition() {

    return upKingPosition;
}

Position GameLogic::getDownKingPosition() {

    return downKingPosition;
}

Position GameLogic::getKingPositionWithOrientation(short int orientation) {

    if (orientation == UP)
        return upKingPosition;
        else
        return downKingPosition;
}

bool GameLogic::columnHasPawn(int column, short int orientation) {

int i;
for (i = 1; i <= 9; i++)
    if (gameBoard->getPieceAtPosition(i,column)->getName() == "pawn" &&
        gameBoard->getPieceAtPosition(i,column)->getOrientation() == orientation)
        return true;

return false;
}

bool GameLogic::dropPiece(string pieceName, Position dropPosition, short int playerOrientation) {

    ShogiPiece* pieceToDrop = gameBoard->removePieceFromHand(playerOrientation,pieceName);
    gameBoard->dropPiece(pieceToDrop,dropPosition);
    if (pieceName == "Pawn")
        return columnHasPawn(dropPosition.col,playerOrientation) || isKingCheckMated(-1*playerOrientation);

    return false;
}

bool GameLogic::isCheckFromPiece(ShogiPiece* attackingPiece) {

    short int opposingKingOrientation = -1*attackingPiece->getOrientation();
    Position opposingKingPosition = getKingPositionWithOrientation(opposingKingOrientation);
    unordered_set<Position*> possibleFutureMovements = gameBoard->getAllPossibleMovementLocationsForPieceFrom(attackingPiece->getPosition());

    return possibleFutureMovements.find(&opposingKingPosition) != possibleFutureMovements.end();

}

bool GameLogic::isCheckFromPiece(Position attackingPiecePosition) {

    ShogiPiece* attackingPiece = gameBoard->getPieceAtPosition(attackingPiecePosition);
    return isCheckFromPiece(attackingPiece);
}

bool GameLogic::isKingCheckMated(short int kingOrientation) {

    unordered_set<ShogiPiece*> opposingArmy = gameBoard->getPiecesOfPlayer(-1*kingOrientation);
    Position initialKingPosition = getKingPositionWithOrientation(kingOrientation);
    unordered_set<Position*> kingMovementOptions = gameBoard->getAllPossibleMovementLocationsForPieceFrom(initialKingPosition);
    ShogiPiece* possibleCapture;
    string possibleCapturedPieceName;
    bool reverseMove = false;
    bool kingChecked;

    for (auto it : kingMovementOptions)
        {
            possibleCapture = gameBoard->getPieceAtPosition(*it);
            reverseMove = false;
            kingChecked = false;

            if (possibleCapture != NULL)
                {
                possibleCapturedPieceName= possibleCapture->getName();
                reverseMove = true;
                opposingArmy.erase(possibleCapture);
                }

            gameBoard->movePiece(initialKingPosition,*it);

            for (auto piece : opposingArmy)
                if ( isCheckFromPiece(piece) )
                    {
                     kingChecked = true;
                     break;
                    }

            if (reverseMove)
                {
                    gameBoard->movePiece(*it,initialKingPosition);
                    dropPiece(possibleCapturedPieceName,*it,kingOrientation);
                    possibleCapture->switchOrientation();
                    opposingArmy.insert(possibleCapture);
                }

            if (!kingChecked)
                return false;

        }

    return true;
}

Position GameLogic::getMirroredPosition(Position position) {

    Position result;
    result.lin = 10 - position.lin;
    result.col = 10 - position.col;

    return result;
}

void GameLogic::movePiece(Position from, Position to) {

short int side = gameBoard->getPieceAtPosition(from)->getOrientation();

if (gameBoard->getPieceAtPosition(to) == NULL)
    gameBoard->movePiece(from,to);
    else
    if ( side != gameBoard->getPieceAtPosition(to)->getOrientation() )
        capturePiece(to);

}

void GameLogic::capturePiece(Position capturedPiece) {

    ShogiPiece* capturedPiecePointer = gameBoard->getPieceAtPosition(capturedPiece);

    gameBoard->addPieceInHand(capturedPiecePointer->getOrientation(),capturedPiecePointer);
    capturedPiecePointer->setCaptured();
}

void GameLogic::promotePiece(Position promotedPiece) {

    gameBoard->getPieceAtPosition(promotedPiece)->promote();
}
