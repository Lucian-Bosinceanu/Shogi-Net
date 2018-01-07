#include "ShogiPiece.h"
#include <map>

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
    void addPieceInHand(bool player,ShogiPiece* piece);
    void removePieceFromHand(bool player, string pieceName);
    bool isPositionOnBoard(Position position);
    ShogiPiece* getPieceAtPosition(int lin, int col);
    ShogiPiece* getPieceAtPosition(Position position);
};
