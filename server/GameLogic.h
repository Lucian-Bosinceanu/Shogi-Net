#include "GameBoard.h"

#define VALID_DROP false
#define INVALID_DROP true

class GameLogic {

    private:
    GameBoard* gameBoard;
    Position upKingPosition;
    Position downKingPosition;

    public:
    GameLogic();
    ~GameLogic();
    Position getUpKingPosition();
    Position getDownKingPosition();
    Position getKingPositionWithOrientation(short int orientation);

    bool columnHasPawn(int column, short int orientation);
    bool dropPiece(string pieceName, Position dropPosition, short int playerOrientation);

    bool isCheckFromPiece(ShogiPiece* attackingPiece);
    bool isCheckFromPiece(Position attackingPiecePosition);
    bool isKingCheckMated(short int kingOrientation);

    Position getMirroredPosition(Position position);
    void movePiece(Position from, Position to);
    void capturePiece(Position capturedPiece);
    void promotePiece(Position promotedPiece);
};
