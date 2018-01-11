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

    void centerTextInButtonBox();

    public:
    Button(string name, int x,int y,int width, int height, string text);
    bool isClicked(int x,int y);
    void draw(sf::RenderWindow* windowWhereToDraw);
    string getName();
};
