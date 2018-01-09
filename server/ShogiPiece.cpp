#include "ShogiPiece.h"
#include <iostream>

using namespace std;

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, short int lin, short int col, short int orientation) {

    this->pieceName = pieceName;
    this->movement[0] = movement;
    this->movement[1] = promotedMovement;
    this->position.lin = lin;
    this->position.col = col;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;

    cout<<"I have created a "<<pieceName<<" at position "<<position.lin<<' '<<position.col<<" with orientation "<<orientation<<'\n';
}

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, short int lin, short int col, short int orientation) {

    this->pieceName = pieceName;
    this->movement[0] = movement;
    this->position.lin = lin;
    this->position.col = col;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;

    cout<<"I have created a "<<pieceName<<" at position "<<position.lin<<' '<<position.col<<" with orientation "<<orientation<<'\n';
}

ShogiPiece::ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, Position place, short int orientation) {

    this->pieceName = pieceName;
    this->movement[0] = movement;
    this->movement[1] = promotedMovement;
    this->position = place;
    this->orientation = orientation;
    this->promotionStatus = UNPROMOTED;

    cout<<"I have created a "<<pieceName<<" at position "<<position.lin<<' '<<position.col<<" with orientation "<<orientation<<'\n';
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

    cout<<"I have created a "<<pieceName<<" at position "<<position.lin<<' '<<position.col<<" with orientation "<<orientation<<'\n';
}

void ShogiPiece::setCaptured() {

    cout<<"[ShogiPiece::setCaptured()] The "<<pieceName<< " from "<<position.lin<<' '<<position.col<<" with orientation "<<orientation<<" has been captured.\n";
    this->position = CAPTURED;
    this->promotionStatus = UNPROMOTED;
    this->orientation = -1*orientation;
    cout<<"[ShogiPiece::setCaptured()] It is now at position "<<position.lin<<' '<<position.col<<", in the hand of the player: "<<orientation<<'\n';
}


void ShogiPiece::moveTo(short int lin,short int col) {

    this->position.lin = lin;
    this->position.col = col;
}

void ShogiPiece::moveTo(Position place) {

    this->position = place;
}

Position ShogiPiece::getPosition() {

    cout<<"[ShogiPiece::getPosition()] Position "<<position.lin<<' '<<position.col<<" sent.\n";
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
