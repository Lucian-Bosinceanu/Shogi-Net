#include "ShogiPiece.h"

using namespace std;

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, short int lin, short int col, short int orientation) {

    this->pieceName = pieceName;
    this->movement = movement;
    this->promotedMovement = promotedMovement;
    this->position.lin = lin;
    this->position.col = col;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;
}

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, short int lin, short int col, short int orientation) {

    this->pieceName = pieceName;
    this->movement = movement;
    this->position.lin = lin;
    this->position.col = col;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;
}

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, Position place, short int orientation) {

    this->pieceName = pieceName;
    this->movement = movement;
    this->promotedMovement = promotedMovement;
    this->position = place;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;
}


void ShogiPiece::setCaptured() {

    this->position = CAPTURED;
    this->promotionStatus = UNPROMOTED;
    this->orientation = -1*orientation;
}


void ShogiPiece::moveTo(short int lin,short int col) {

    this->position.lin = lin;
    this->position.col = col;
}

void ShogiPiece::moveTo(Position place) {

    this->position = place;
}

Position ShogiPiece::getPosition() {

    return position;
}

short int ShogiPiece::getOrientation() {

    return orientation;
}

string ShogiPiece::getName(){

    return pieceName;
}

bool ShogiPiece::getPromotionStatus() {

    return promotionStatus;
}

void ShogiPiece::promote() {

    this->promotionStatus = PROMOTED;
}
