#include <SFML/Graphics.hpp>
#include "Menu.h"

using namespace std;

class GameGUI {

    private:
    sf::RenderWindow window;
    sf::Font textFont;

    //GameBoard* gameBoard;
    vector<Menu*> gameMenus;

    public:
    GameGUI();
    //void setGameBoard(GameBoard* gameBoard);
    void drawMenu(string name);
    sf::RenderWindow* getWindow() {return &window;};
};

