#include "GameBoard.h"

#define VALID_DROP false
#define INVALID_DROP true

class GameLogic {

    private:
    GameBoard* gameBoard;

    public:
    GameLogic();
    ~GameLogic();


    void dropPiece(string pieceName, Position dropPosition, bool playerStatus, bool whoMovesNow);
    void movePiece(Position from, Position to, bool playerStatus);
    void capturePiece(Position capturedPiece, bool playerStatus);
    void promotePiece(Position promotedPiece);

    unordered_set<Position*> getAllPossibleMovementLocationsForPieceFrom(Position piecePosition);
    vector<string> getUpHandPieces();
    vector<string> getDownHandPieces();

    GameBoard* getGameBoard();
};
