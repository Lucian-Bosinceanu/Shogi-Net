#include "Menu.h"
#include <iostream>

using namespace std;


Menu::Menu(string name,vector<Button*> buttons,sf::RenderWindow* window) {

this->name = name;
this->buttons = buttons;
this->window = window;
}

void Menu::drawMenu() {

    //cout<<"[Menu::drawMenu] I am drawing menu: "<<this->getName()<<" in this window: "<<window<<'\n';

    for ( auto it : buttons )
        it->draw(window);
}

bool Menu::isButtonPressed(string name, int x, int y) {

    Button * targetButton = getButtonByName(name);
    return targetButton->isClicked(x,y);

}

Button* Menu::getButtonByName(string buttonName) {

    for (auto it : buttons)
        if (it->getName() == buttonName)
            return it;

    return NULL;
}


void Menu::setButtonText(string buttonName, string text) {

    for (auto it : buttons)
        if (it->getName() == buttonName)
            {it->setText(text);
            return;
            }


}
