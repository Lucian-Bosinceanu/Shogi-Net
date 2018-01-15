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


    bool isKingInCheck(Position kingPosition);
    bool isCheckFromPiece(ShogiPiece* attackingPiece, Position kingPosition);
    bool isCheckFromPiece(Position attackingPiecePosition);
    void dropPiece(string pieceName, Position dropPosition, bool playerStatus, bool whoMovesNow);
    void movePiece(Position from, Position to, bool playerStatus);
    void capturePiece(Position capturedPiece, bool playerStatus);
    void promotePiece(Position promotedPiece);

    unordered_set<Position*> getAllPossibleMovementLocationsForPieceFrom(Position piecePosition);
    vector<string> getUpHandPieces();
    vector<string> getDownHandPieces();

    Position getKingPosition(short int orientation);

    GameBoard* getGameBoard();
};
