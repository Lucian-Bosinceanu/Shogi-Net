#include "GameBoard.h"
#include <unordered_set>
#include <iostream>

using namespace std;

GameBoard::GameBoard() {

    int i,j;
    vector<Position> pawnMovement = { {-1,0}};
    vector<Position> goldenGeneralMovement = { {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,0} };
    vector<Position> lanceMovement = { {-1,0}, {-2,0}, {-3,0}, {-4,0}, {-5,0}, {-6,0}, {-7,0}, {-8,0} };
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

    board[1][1] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,{3,0,7},1,1,DOWN);
    board[1][9] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,{3,0,7},1,9,DOWN);
    board[9][1] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,{3,0,7},9,1,UP);
    board[9][9] = new ShogiPiece("lance",lanceMovement,goldenGeneralMovement,{3,0,7},9,9,UP);

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

    board[2][2] = new ShogiPiece("rook",rookMovement,promotedRookMovement,{6,0,8,16,24,31,4,12,20,28,35},2,2,DOWN);
    board[8][8] = new ShogiPiece("rook",rookMovement,promotedRookMovement,{6,0,8,16,24,31,4,12,20,28,35},8,8,UP);

    board[2][8] = new ShogiPiece("bishop",bishopMovement,promotedBishopMovement,{6,0,8,16,24,31,4,12,20,28,35},2,8,DOWN);
    board[8][2] = new ShogiPiece("bishop",bishopMovement,promotedBishopMovement,{6,0,8,16,24,31,4,12,20,28,35},8,2,UP);

    cout<<"[Game] GameBoard instance created!\n";
}

GameBoard::~GameBoard() {

    int i,j;

    for (i=1;i<=9;i++)
        for (j=1;j<=9;j++)
             delete board[i][j];

    cout<<"[Game] GameBoard instance deleted!\n";
}

void GameBoard::addPieceInHand(short int player,ShogiPiece* piece) {

    if (player==UPPLAYER)
        {
        cout<<"[GameBoard::addPieceInHand()] The "<<piece->getName()<<" at position "<<piece->getPosition().lin<<' '<<piece->getPosition().col<<" has been added to the hand of the HOST player.\n";
        upHandPieces.insert(pair<string,ShogiPiece*>(piece->getName(), piece));

        }
        else
        {
        cout<<"The "<<piece->getName()<<" at position "<<piece->getPosition().lin<<' '<<piece->getPosition().col<<" has been added to the hand of the GUEST player.\n";
        downHandPieces.insert(pair<string,ShogiPiece*>(piece->getName(), piece));
        }


    cout<<"[GameBoard::addPieceInHand] The HOST hand has: ";
    for (auto it : upHandPieces )
            cout<<it.first<<' ';
    cout<<'\n';

    cout<<"[GameBoard::addPieceInHand] The GUEST hand has: ";
    for (auto it : downHandPieces )
        cout<<it.first<<' ';
    cout<<'\n';
}

ShogiPiece* GameBoard::removePieceFromHand(short int player, string pieceName) {

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
    return position.lin <= 3;
    else
    return position.lin >= 7;
}

void GameBoard::movePiece(Position from, Position to) {


    board[to.lin][to.col] =  board[from.lin][from.col];
    board[to.lin][to.col]->moveTo(to);
    board[from.lin][from.col] = NULL;
}

 void GameBoard::dropPiece(ShogiPiece* piece, Position dropPosition) {

    board[dropPosition.lin][dropPosition.col] = piece;
    board[dropPosition.lin][dropPosition.col]->moveTo(dropPosition);
 }

 unordered_set<ShogiPiece*> GameBoard::getPiecesOfPlayer(short int orientation) {

    cout<<"[GameBoard::getPiecesOfPlayer] I am trying to find all pieces of the player with orientation "<<orientation<<'\n';
    unordered_set<ShogiPiece*> result;
    int i,j;
    for (i=1;i<=9;i++)
        for (j=1;j<=9;j++)
            if (board[i][j] != NULL)
                if (board[i][j]->getOrientation() == orientation )
                    result.insert(board[i][j]);

    cout<<"[GameBoard::getPiecesOfPlayer] I am done.\n";
    return result;
 }

 unordered_set<Position*> GameBoard::getAllPossibleMovementLocationsForPieceFrom(Position piecePosition) {

    //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am here.\n";
    unordered_set<Position*> result;
    ShogiPiece* piece = board[piecePosition.lin][piecePosition.col];

    //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am calculating for piece from "<<piecePosition.lin<<' '<<piecePosition.col<<", which is a "
    //piece->getName()<<" oriented "<<piece->getOrientation()<<'\n';

    Position possiblePosition;
    Position* elementToInsert;
    short int pieceOrientation = piece->getOrientation();
    bool promotionStatus = piece->getPromotionStatus();

    //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am loading movement rules. \n";
    vector<Position> movementRules = piece->getMovementRules(promotionStatus);
    //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] Movement rules loaded. \n";

    int i,dir;
    int flagLowerBound = 0, flagUpperBound = 0;

    elementToInsert = new Position{piecePosition.lin,piecePosition.col};
    result.insert(elementToInsert);

    if (!piece->isRangedPiece() || (piece->getName() == "lance" && piece->getPromotionStatus() == PROMOTED))
        {
            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am calculating for a non-ranged piece or for a promoted lance. \n";

            for (i = 0; i < movementRules.size();i++)
                {
                    possiblePosition.lin = piecePosition.lin + pieceOrientation*movementRules[i].lin;
                    possiblePosition.col = piecePosition.col + pieceOrientation*movementRules[i].col;
                    if (isPositionOnBoard(possiblePosition))
                        if (getPieceAtPosition(possiblePosition) == NULL ||
                           ( (getPieceAtPosition(possiblePosition) != NULL) && (getPieceAtPosition(possiblePosition)->getOrientation() != pieceOrientation) )
                           )
                            {
                            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am inserting this position to results: "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
                            elementToInsert = new Position{possiblePosition.lin,possiblePosition.col};
                            result.insert(elementToInsert);
                            }
                }

            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am done. \n";
            return result;
        }

    //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am calculating for a rook or a bishop. \n";

    vector<int> directions = piece->getDirectionFlags();

    if ( (piece->getName() == "rook" || piece->getName() == "bishop") && piece->getPromotionStatus() == PROMOTED )
        {
            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] Which is also promoted. \n";
            for (i = 0; i<= 4;i++)
                    {
                        possiblePosition.lin = piecePosition.lin + pieceOrientation*movementRules[i].lin;
                        possiblePosition.col = piecePosition.col + pieceOrientation*movementRules[i].col;
                        if (isPositionOnBoard(possiblePosition))
                            if (getPieceAtPosition(possiblePosition) == NULL ||
                               ( (getPieceAtPosition(possiblePosition) != NULL) && (getPieceAtPosition(possiblePosition)->getOrientation() != pieceOrientation) )
                               )
                                {
                            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am inserting this position to results: "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
                            elementToInsert = new Position{possiblePosition.lin,possiblePosition.col};
                            result.insert(elementToInsert);
                            }
                    }

            flagLowerBound = directions[0];
            flagUpperBound = directions.size()-1;
        }

    if ( !flagLowerBound && !flagUpperBound )
        {
        flagLowerBound = 1;
        flagUpperBound = directions[0]-1;
        }

    for (dir=flagLowerBound; dir<flagUpperBound;dir++)
        for (i= directions[dir]; i<= directions[dir+1]; i++)
            {
                possiblePosition.lin = piecePosition.lin + pieceOrientation*movementRules[i].lin;
                possiblePosition.col = piecePosition.col + pieceOrientation*movementRules[i].col;
                if (isPositionOnBoard(possiblePosition))
                    {
                    if (getPieceAtPosition(possiblePosition) == NULL)
                        {
                            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am inserting this position to results: "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
                            elementToInsert = new Position{possiblePosition.lin,possiblePosition.col};
                            result.insert(elementToInsert);
                        }
                        else
                        {if (getPieceAtPosition(possiblePosition)->getOrientation() != pieceOrientation)
                            {
                            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am inserting this position to results: "<<possiblePosition.lin<<' '<<possiblePosition.col<<'\n';
                            //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] at this position there is a "<<getPieceAtPosition(possiblePosition)->getName()<<" with orientation "<<getPieceAtPosition(possiblePosition)->getOrientation()<<'\n';
                            elementToInsert = new Position{possiblePosition.lin,possiblePosition.col};
                            result.insert(elementToInsert);
                            break;
                            }
                            else
                            break;
                        }
                    }
            }

    //cout<<"[GameBoard::getAllPossibleMovementLocationsForPieceFrom] I am done. \n";
    return result;
 }
