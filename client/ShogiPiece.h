#include <vector>
#include <string>

#define UP 1
#define DOWN -1

#define PROMOTED true
#define UNPROMOTED false

#define CAPTURED {-1,-1}

using namespace std;

struct Position {

    short int lin;
    short int col;
};


class ShogiPiece {

    private:
    string pieceName;
    vector<Position> movement[2];
    vector<int> directionFlags;
    //vector<Position> promotedMovement;
    Position position;
    short orientation;  //piece orientation is given by the direction in which the piece head points to.
    bool promotionStatus;

    public:
    ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, short int lin, short int col, short int orientation);
    ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, vector<int> directionFlags, short int lin, short int col, short int orientation);
    ShogiPiece(string pieceName, vector<Position> movement, short int lin, short int col, short int orientation);
    ShogiPiece(string pieceName, vector<Position> movement, vector<Position> promotedMovement, Position place, short int orientation);
    void setCaptured();
    void moveTo(short int lin,short int col);
    void moveTo(Position place);

    vector<Position> getMovementRules(bool promotionStatus);
    vector<int> getDirectionFlags();
    Position getPosition();
    short int getOrientation();
    bool getPromotionStatus();
    string getName();
    void switchOrientation();

    void promote();

    bool isRangedPiece();
};
