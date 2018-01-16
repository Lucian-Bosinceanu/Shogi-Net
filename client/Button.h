#include <SFML/Graphics.hpp>
#include <string>
#include "ColorScheme.h"

using namespace std;

class Button {

    private:
    string buttonName;
    int x;
    int y;
    int width;
    int height;
    sf::Text textSf;

    sf::Font textFont;
    sf::Color buttonColor;
    sf::Color buttonOutlineColor;
    sf::Color textButtonColor;

    sf::Texture buttonT;

    void centerTextInButtonBox();
    void findOptimalTextSize();

    public:
    Button(string name, int x,int y,int width, int height, string text);
    bool isClicked(int x,int y);
    void draw(sf::RenderWindow* windowWhereToDraw);
    string getName();
    void setText(string text);
    string getText() { return textSf.getString();};
};
