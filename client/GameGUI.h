#include <SFML/Graphics.hpp>
#include "Menu.h"
#include <map>

#define VISIBLE 1
#define HIDDEN 2

using namespace std;

class GameGUI {

    private:

    sf::RenderWindow window;
    sf::Font textFont;

    //GameBoard* gameBoard;
    vector<Menu*> gameMenus;
    map< string,vector<sf::Sprite> > piecesSprites;

    sf::Sprite board;
    sf::Sprite upHand;
    sf::Sprite downHand;
    sf::Sprite teMarker;

    int pieceOffsetX;
    int pieceOffsetY;

    void loadPiecesSprites();
    void loadBoardSprites();

    public:
    GameGUI();
    //void setGameBoard(GameBoard* gameBoard);
    void drawMenu(string name);
    sf::RenderWindow* getWindow() {return &window;};
    Menu* getMenu(string menuName);
    string getInputFromClient(Menu* menu,Button* field,int type);
    void drawBoard();
    map< string,vector<sf::Sprite> > getPiecesSprites() { return piecesSprites;};

    int getPieceOffsetX() {return pieceOffsetX;};
    int getPieceOffsetY() {return pieceOffsetY;};

    int getBoardPositionX() {return board.getPosition().x + 10;};
    int getBoardPositionY() {return board.getPosition().y + 10;};

    int getUpHandPositionX() {return upHand.getPosition().x + 10;};
    int getUpHandPositionY() {return upHand.getPosition().y + 10;};

    int getDownHandPositionX() {return downHand.getPosition().x + 10;};
    int getDownHandPositionY() {return downHand.getPosition().y + 10;};

    bool isBoardClicked(int x, int y);
    bool isHandClicked(int x, int y);

    int getHandClickedPiecePosition(int x, int y);
    int getBoardClickedPositionX(int x, int y);
    int getBoardClickedPositionY(int x,int y);
};

