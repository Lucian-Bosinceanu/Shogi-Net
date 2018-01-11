#include <vector>
#include "Button.h"

using namespace std;

class Menu {

    private:
    string name;
    vector<Button*> buttons;
    sf::RenderWindow* window;

    Button* getButtonByName(string buttonName);

    public:
    string getName() {return name;};
    Menu(string name, vector<Button*> buttons, sf::RenderWindow* window);
    void drawMenu();
    bool isButtonPressed(string name, int x, int y);
};
