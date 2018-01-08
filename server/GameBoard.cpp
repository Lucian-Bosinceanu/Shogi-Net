#include "GameBoard.h"
#include <unordered_set>

using namespace std;

GameBoard::GameBoard() {
int i,j;
vector<Position> pawnMovement = { {0,-1}};
vector<Position> goldenGeneralMovement = { {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,0} };
vector<Position> lanceMovement = { {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0} };
vector<Position> knightMovement = { {-2,-1}, {-2,1} };
vector<Position> silverGeneralMovement = { {-1,-1}, {-1,0}, {-1,1}, {1,-1}, {1,1} };
vector<Position> kingMovement = { {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1} };

vector<Position> bishopMovement = { {-1,-1}, {-2,-2}, {-3,-3}, {-4,-4}, {-5,-5}, {-6,-6}, {-7,-7}, {-8,-8},
                            {1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}, {7,7}, {8,8},
                            {1,-1}, {2,-2}, {3,-3}, {4,-4}, {5,-5}, {6,-6}, {7,-7}, {8,-8},
                            {-1,1}, {-2,2}, {-3,3}, {-4,4}, {-5,5}, {-6,6}, {-7,7}, {-8,8} };

vector<Position> promotedBishopMovement = {  {-1,0}, {0,-1}, {0,1}, {1,0},
                                     {-1,-1}, {-2,-2}, {-3,-3}, {-4,-4}, {-5,-5}, {-6,-6}, {-7,-7}, {-8,-8},
                                     {1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}, {7,7}, {8,8},
                                     {1,-1}, {2,-2}, {3,-3}, {4,-4}, {5,-5}, {6,-6}, {7,-7}, {8,-8},
                                     {-1,1}, {-2,2}, {-3,3}, {-4,4}, {-5,5}, {-6,6}, {-7,7}, {-8,8} };

vector<Position> rookMovement = { {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0},
                          {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8},
                          {-1,0}, {-2,0}, {-3,0}, {-4,0}, {-5,0}, {-6,0}, {-7,0}, {-8,0},
                          {0,-1}, {0,-2}, {0,-3}, {0,-4}, {0,-5}, {0,-6}, {0,-7}, {0,-8} };

vector<Position> promotedRookMovement = { {-1,-1}, {-1,1}, {1,-1}, {1,1},
                                  {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0},
                                  {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8},
                                  {-1,0}, {-2,0}, {-3,0}, {-4,0}, {-5,0}, {-6,0}, {-7,0}, {-8,0},
                                  {0,-1}, {0,-2}, {0,-3}, {0,-4}, {0,-5}, {0,-6}, {0,-7}, {0,-8} };

for (i=1;i<=9;i++)
    for (j=1;j<=9;j++)
        board[i][j] = NULL;

for (i=1;i<=9;i++)
    {
        board[3][i] = new ShogiPiece("pawn",pawnMovement,goldenGeneralMovement,3,i,DOWN);
        board[7][i] = new ShogiPiece("pawn",pawnMovement,goldenGeneralMovement,7,i,UP);
    }

board[1][1] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,1,1,DOWN);
board[1][9] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,1,9,DOWN);
board[9][1] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,9,1,UP);
board[9][9] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,9,9,UP);

board[1][2] = new ShogiPiece("knight",knightMovement,goldenGeneralMovement,1,2,DOWN);
board[1][8] = new ShogiPiece("knight",knightMovement,goldenGeneralMovement,1,8,DOWN);
board[9][2] = new ShogiPiece("knight",knightMovement,goldenGeneralMovement,9,2,UP);
board[9][8] = new ShogiPiece("knight",knightMovement,goldenGeneralMovement,9,8,UP);

board[1][3] = new ShogiPiece("silver",silverGeneralMovement,goldenGeneralMovement,1,3,DOWN);
board[1][7] = new ShogiPiece("silver",silverGeneralMovement,goldenGeneralMovement,1,7,DOWN);
board[9][3] = new ShogiPiece("silver",silverGeneralMovement,goldenGeneralMovement,9,3,UP);
board[9][7] = new ShogiPiece("silver",silverGeneralMovement,goldenGeneralMovement,9,7,UP);

board[1][4] = new ShogiPiece("golden",goldenGeneralMovement,1,4,DOWN);
board[1][6] = new ShogiPiece("golden",goldenGeneralMovement,1,6,DOWN);
board[9][4] = new ShogiPiece("golden",goldenGeneralMovement,9,4,UP);
board[9][6] = new ShogiPiece("golden",goldenGeneralMovement,9,6,UP);

board[1][5] = new ShogiPiece("king",kingMovement,1,5,DOWN);
board[9][5] = new ShogiPiece("king",kingMovement,9,5,UP);

board[2][2] = new ShogiPiece("rook",rookMovement,promotedRookMovement,2,2,DOWN);
board[8][8] = new ShogiPiece("rook",rookMovement,promotedRookMovement,8,8,UP);

board[2][8] = new ShogiPiece("bishop",bishopMovement,promotedBishopMovement,2,8,DOWN);
board[8][2] = new ShogiPiece("bishop",bishopMovement,promotedBishopMovement,8,2,UP);
}

GameBoard::~GameBoard() {

int i,j;

for (i=1;i<=9;i++)
    for (j=1;j<=9;j++)
         delete board[i][j];
}

void GameBoard::addPieceInHand(bool player,ShogiPiece* piece) {

if (player==UPPLAYER)
    upHandPieces.insert(pair<string,ShogiPiece*>(piece->getName(), piece));
    else
    downHandPieces.insert(pair<string,ShogiPiece*>(piece->getName(), piece));

}

ShogiPiece* GameBoard::removePieceFromHand(bool player, string pieceName) {

ShogiPiece* result;
multimap<string,ShogiPiece*>::iterator piece;

if (player == UPPLAYER)
    {
    piece = upHandPieces.find(pieceName);
    result = piece->second;
    upHandPieces.erase(piece);
    }
    else
    {
    piece = downHandPieces.find(pieceName);
    result = piece->second;
    downHandPieces.erase(piece);
    }

return result;
}

bool GameBoard::isPositionOnBoard(Position position){

return (position.lin >=1 && position.lin <=9 ) && (position.col >=1 && position.col <=9);

}

ShogiPiece* GameBoard::getPieceAtPosition(int lin, int col) {

    return board[lin][col];
}

ShogiPiece* GameBoard::getPieceAtPosition(Position position) {

    return board[position.lin][position.col];
}

bool GameBoard::isPromotionZone(Position position, short int side){

if (side == UP)
    return position.col <= 3;
    else
    return position.col >= 7;
}

void GameBoard::movePiece(Position from, Position to) {

    board[to.lin][to.col] =  board[from.lin][from.col];
    board[from.lin][from.col] = NULL;
}

 void GameBoard::dropPiece(ShogiPiece* piece, Position dropPosition) {

    board[dropPosition.lin][dropPosition.col] = piece;
 }

 unordered_set<ShogiPiece*> GameBoard::getPiecesOfPlayer(short int orientation) {

    unordered_set<ShogiPiece*> result;
    int i,j;
    for (i=1;i<=9;i++)
        for (j=1;j<=9;j++)
            if (board[i][j]->getOrientation() == orientation )
                result.insert(board[i][j]);
    return result;
 }

 unordered_set<Position*> GameBoard::getAllPossibleMovementLocationsForPieceFrom(Position piecePosition) {

    unordered_set<Position*> result;
    ShogiPiece* piece = board[piecePosition.lin][piecePosition.col];
    Position possiblePosition;
    short int pieceOrientation = piece->getOrientation();
    bool promotionStatus = piece->getPromotionStatus();
    vector<Position> movementRules = piece->getMovementRules(promotionStatus);
    int i;

    if (!piece->isRangedPiece())
        {
            for (i = 0; i<= movementRules.size();i++)
                {
                    possiblePosition.lin = piecePosition.lin + pieceOrientation*movementRules[i].lin;
                    possiblePosition.col = piecePosition.col + pieceOrientation*movementRules[i].col;
                    if (isPositionOnBoard(possiblePosition))
                        if (getPieceAtPosition(possiblePosition) == NULL ||
                           ( (getPieceAtPosition(possiblePosition) != NULL) && (getPieceAtPosition(possiblePosition)->getOrientation() != pieceOrientation) )
                           )
                            result.insert(&possiblePosition);
                }
        }

    return result;
 }