#include <vector>
#include "Button.h"

using namespace std;

class Menu {

    private:
    string name;
    vector<Button*> buttons;
    sf::RenderWindow* window;


    public:
    Button* getButtonByName(string buttonName);
    string getName() {return name;};
    Menu(string name, vector<Button*> buttons, sf::RenderWindow* window);
    void drawMenu();
    bool isButtonPressed(string name, int x, int y);
    void setButtonText(string buttonName, string text);
};
