#include "Button.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

Button::Button(string name,int x,int y,int width, int height, string text) {

    this->buttonName = name;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    textSf.setFont(textFont);
    textSf.setString(text);

    textFont.loadFromFile("rsc/fonts/arial.ttf");

    textSf.setFillColor(textButtonColor);

     buttonColor = sf::Color(255,0,0);
     buttonOutlineColor = sf::Color(0,0,0);
     textButtonColor = sf::Color(100,100,100);

}

bool Button::isClicked(int xClick,int yClick) {

    return ( xClick >= x && xClick <= (x+width) ) && ( yClick >= y && yClick <= (y+height) );
}

void Button::draw(sf::RenderWindow* windowWhereToDraw){

    //cout<<"[Button::draw()] I am drawing button: "<<this->getName()<<" in this window: "<<windowWhereToDraw<<'\n';

    sf::RectangleShape button;
    button.setPosition(x,y);
    button.setSize(sf::Vector2f(width,height));
    button.setFillColor(buttonColor);
    button.setOutlineThickness(5);
    button.setOutlineColor(buttonOutlineColor);

    centerTextInButtonBox();

    windowWhereToDraw->draw(button);
    windowWhereToDraw->draw(textSf);
}

void Button::centerTextInButtonBox() {

    int textWidth, textHeight;
    int textX, textY;
    textSf.setCharacterSize(height/3);

    textWidth = textSf.getGlobalBounds().width;
    textHeight = textSf.getGlobalBounds().height;

    textSf.setOrigin(0,0);
    textSf.setOrigin(textSf.getOrigin().x + textWidth/2, textSf.getOrigin().y + textSf.getCharacterSize()/2 );

    textX = x + width/2;
    textY = y + height/2;

    textSf.setPosition(textX,textY);

    cout<<"Button: "<<x<<' '<<y<<' '<<x+width<<' '<<y+height<<'\n';
    cout<<"Text center: "<<textX<<' '<<textY<<'\n';
    cout<<"Text origin: "<<textSf.getOrigin().x<<' '<<textSf.getOrigin().y<<'\n';
    cout<<"Text size: "<<textSf.getCharacterSize()<<' '<< textWidth<<' '<<textHeight<<'\n';
}

string Button::getName() {

    return buttonName;
}
