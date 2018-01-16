#include <SFML/Graphics.hpp>
#include "Menu.h"
#include <map>

#define VISIBLE 1
#define HIDDEN 2

using namespace std;

class GameGUI {

    private:

    sf::RenderWindow window;

    //GameBoard* gameBoard;
    vector<Menu*> gameMenus;
    map< string,vector<sf::Sprite> > piecesSprites;

    sf::Sprite board;
    sf::Sprite upHand;
    sf::Sprite downHand;
    sf::Sprite title;
    sf::Sprite background;


    sf::Texture unpromoted;
    sf::Texture kingTexture;
    sf::Texture kingGuestTexture;
    sf::Texture pawnTexture;
    sf::Texture pawnPromotedTexture;
    sf::Texture lanceTexture;
    sf::Texture lancePromotedTexture;
    sf::Texture knightTexture;
    sf::Texture knightPromotedTexture;
    sf::Texture silverTexture;
    sf::Texture silverPromotedTexture;
    sf::Texture goldenTexture;
    sf::Texture bishopTexture;
    sf::Texture bishopPromotedTexture;
    sf::Texture rookTexture;
    sf::Texture rookPromotedTexture;

    sf::Texture boardT;
    sf::Texture handT;
    sf::Texture teMarkerT;
    sf::Texture backgroundT;

    sf::Texture titleT;

    int pieceOffsetX;
    int pieceOffsetY;

    void loadPiecesSprites();
    void loadBoardSprites();
    void loadTitle();

    public:
    GameGUI();
    //void setGameBoard(GameBoard* gameBoard);
    void drawMenu(string name);
    sf::RenderWindow* getWindow() {return &window;};
    Menu* getMenu(string menuName);
    string getInputFromClient(Menu* menu,Button* field,int type);

    void clearEventQueue();

    void drawBoard();
    void drawTitle();
    void drawGameScreen();


    map< string,vector<sf::Sprite> > getPiecesSprites() { return piecesSprites;};

    int getPieceOffsetX() {return pieceOffsetX;};
    int getPieceOffsetY() {return pieceOffsetY;};

    int getBoardPositionX() {return 458;/*board.getPosition().x + 10;*/};
    int getBoardPositionY() {return 50;/*board.getPosition().y + 10;*/};

    int getUpHandPositionX() {return upHand.getPosition().x + 20;};
    int getUpHandPositionY() {return upHand.getPosition().y + 20;};

    int getDownHandPositionX() {return downHand.getPosition().x + 20;};
    int getDownHandPositionY() {return downHand.getPosition().y + 20;};

    bool isBoardClicked(int x, int y);
    bool isHandClicked(int x, int y);

    int getHandClickedPiecePosition(int x, int y);
    int getBoardClickedPositionX(int x, int y);
    int getBoardClickedPositionY(int x,int y);
};

