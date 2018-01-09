#include "ShogiPiece.h"
#include <map>
#include <vector>
#include <unordered_set>

#define UPPLAYER true  //player orientation is given by piece orientation. The Up player is actually the one at the bottom of the board.
#define DOWNPLAYER false

using namespace std;

class GameBoard {

    private:
    ShogiPiece* board[10][10];
    multimap<string,ShogiPiece*> upHandPieces;
    multimap<string,ShogiPiece*> downHandPieces;

    public:
    GameBoard();
    ~GameBoard();
    void addPieceInHand(short int player,ShogiPiece* piece);
    ShogiPiece* removePieceFromHand(short int player, string pieceName);

    bool isPositionOnBoard(Position position);
    bool isPromotionZone(Position position, short int side);

    void movePiece(Position from, Position to);
    ShogiPiece* getPieceAtPosition(int lin, int col);
    ShogiPiece* getPieceAtPosition(Position position);
    void dropPiece(ShogiPiece* piece, Position dropPosition);

    unordered_set<Position*> getAllPossibleMovementLocationsForPieceFrom(Position piecePosition);
    unordered_set<ShogiPiece*> getPiecesOfPlayer(short int orientation);
};
