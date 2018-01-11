#include "GameGUI.h"
#include <iostream>

using namespace std;

GameGUI::GameGUI() {

    window.create(sf::VideoMode(1600,900),"Shogi Net Client");
    //this->window = &window;

    textFont.loadFromFile("rsc/fonts/arial.ttf");

    Button* exitButton = new Button("exit",1000,600,150,100,"EXIT");
    Button* playButton = new Button("play",1000,400,150,100,"PLAY" );

    Menu* mainMenu = new Menu("main",{exitButton,playButton},&window);

    Button* usernameField = new Button("username",1000,300,100,40,"");
    Button* passwordField = new Button("password",1000,400,100,40,"");
    Button* confirmPasswordField = new Button("confirmPass",1000,500,100,40,"");
    Button* loginButton = new Button("login",1000,600,100,80,"LOGIN");
    Button* registerLoginButton = new Button("registerL",1000,700,100,80,"REGISTER");
    Button* registerRegisterButton = new Button("registerR",1000,700,100,80,"REGISTER");
    Button* backButton = new Button("back",200,100,50,50,"Back");

    Menu* loginMenu = new Menu("login",{usernameField,passwordField,loginButton,registerLoginButton,backButton},&window);
    Menu* registerMenu = new Menu("register",{usernameField,passwordField,confirmPasswordField,registerRegisterButton},&window);

    Button* forfeitButton = new Button("forfeit",200,100,50,50,"FORFEIT");

    Menu* gameMenu = new Menu("game",{forfeitButton},&window);

    gameMenus = {mainMenu,loginMenu,registerMenu,gameMenu};
}

/*void GameGUI::setGameBoard(GameBoard* gameBoard) {

    this->gameBoard = gameBoard;
}*/

void GameGUI::drawMenu(string name) {

    //cout<<"[GameGUI::drawMenu] I am about to draw menu: "<<name<<'\n';

    for (auto it : gameMenus)
        if (it->getName() == name)
            {
            cout<<"[GameGUI::drawMenu] Game menu found!. I am drawing it\n";
            it->drawMenu();
            return;
            }
}
