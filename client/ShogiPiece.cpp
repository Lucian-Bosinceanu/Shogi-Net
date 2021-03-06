#include "ShogiPiece.h"

using namespace std;

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, short int lin, short int col, short int orientation) {

    this->pieceName = pieceName;
    this->movement[0] = movement;
    this->movement[1] = promotedMovement;
    this->position.lin = lin;
    this->position.col = col;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;
}

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, short int lin, short int col, short int orientation) {

    this->pieceName = pieceName;
    this->movement[0] = movement;
    this->position.lin = lin;
    this->position.col = col;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;
}

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, Position place, short int orientation) {

    this->pieceName = pieceName;
    this->movement[0] = movement;
    this->movement[1] = promotedMovement;
    this->position = place;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;
}

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, vector<int> directionFlags, short int lin, short int col, short int orientation) {

    this->pieceName = pieceName;
    this->movement[0] = movement;
    this->movement[1] = promotedMovement;
    this->position.lin = lin;
    this->position.col = col;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;
    this->directionFlags = directionFlags;
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

vector<Position> ShogiPiece::getMovementRules(bool promotionStatus) {

    return movement[promotionStatus];
}

void ShogiPiece::promote() {

    this->promotionStatus = PROMOTED;
}

 bool ShogiPiece::isRangedPiece() {

    if (pieceName == "bishop" || pieceName == "rook" || pieceName == "lance")
        return true;
    return false;
}

void ShogiPiece::switchOrientation(){

    orientation = -1*orientation;
}

vector<int> ShogiPiece::getDirectionFlags() {

    return directionFlags;
}
