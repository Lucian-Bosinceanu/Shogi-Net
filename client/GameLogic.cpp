#include "GameLogic.h"

using namespace std;

GameLogic::GameLogic() {

    gameBoard = new GameBoard();

}

GameLogic::~GameLogic() {

    gameBoard->~GameBoard();
}



void GameLogic::dropPiece(string pieceName, Position dropPosition, bool playerStatus, bool whoMovesNow) {

    ShogiPiece* pieceToDrop = gameBoard->removePieceFromHand(playerStatus,whoMovesNow,pieceName);
    gameBoard->dropPiece(pieceToDrop,dropPosition);

}


void GameLogic::movePiece(Position from, Position to, bool playerStatus) {

//cout<<"[GameLogic::movePiece()] I am trying to move the piece from "<<from.lin<<' '<<from.col<<" to "<<to.lin<<' '<<to.col<<'\n';

//short int side = playerOrientation;

//cout<<"[GameLogic::movePiece()] I am about to move a "<<gameBoard->getPieceAtPosition(from)->getName()<<" with orientation "<<side<<'\n';

if (gameBoard->getPieceAtPosition(to) == NULL)
    gameBoard->movePiece(from,to);
    else
    if ( gameBoard->getPieceAtPosition(from)->getOrientation() != gameBoard->getPieceAtPosition(to)->getOrientation() )
        {
  //      cout<<"[GameLogic::movePiece] I am going to capture the piece from "<<to.lin<<' '<<to.col<<'\n';
        capturePiece(to, playerStatus);
        gameBoard->movePiece(from,to);
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

unordered_set<Position*> GameLogic::getAllPossibleMovementLocationsForPieceFrom(Position piecePosition){

    return gameBoard->getAllPossibleMovementLocationsForPieceFrom(piecePosition);
}

vector<string> GameLogic::getUpHandPieces() {

    return gameBoard->getHandPiecesOfPlayer(UP);
}

vector<string> GameLogic::getDownHandPieces() {

    return gameBoard->getHandPiecesOfPlayer(DOWN);
}
