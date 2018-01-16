#include "GameGUI.h"
#include <iostream>

using namespace std;

GameGUI::GameGUI() {

    pieceOffsetX = 76;
    pieceOffsetY = 89;

    window.create(sf::VideoMode(1600,900),"Shogi Net Client");
    //this->window = &window;


    Button* exitButton = new Button("exit",700,700,200,100,"EXIT");
    Button* playButton = new Button("play",700,550,200,100,"PLAY" );

    Menu* mainMenu = new Menu("main",{exitButton,playButton},&window);

    Button* usernameField = new Button("username",575,300,450,50,"Enter username");
    Button* passwordField = new Button("password",575,400,450,50,"Enter password");
    Button* confirmPasswordField = new Button("confirmPass",575,500,450,50,"Re-enter password");
    Button* loginButton = new Button("login",575,600,200,100,"LOGIN");
    Button* registerLoginButton = new Button("registerL",825,600,200,100,"REGISTER");
    Button* registerRegisterButton = new Button("registerR",700,600,200,100,"REGISTER");
    Button* backButton = new Button("back",61,40,200,100,"BACK");
    Button* warnings = new Button("warning",0,200,1600,50,"Please provide your credentials.");

    Menu* loginMenu = new Menu("login",{usernameField,passwordField,loginButton,registerLoginButton,backButton,warnings},&window);
    Menu* registerMenu = new Menu("register",{usernameField,passwordField,confirmPasswordField,registerRegisterButton,warnings,backButton},&window);

    Button* forfeitButton = new Button("forfeit",61,40,200,100,"FORFEIT");
    Button* statusButton = new Button("status",61,414,324,50,"");

    Menu* gameMenu = new Menu("game",{forfeitButton,statusButton},&window);


    Button* refreshButton = new Button("refresh",1000,675,200,100,"REFRESH");
    Button* hostButton = new Button("host",400,675,200,100,"HOST");
    Button* joinButton = new Button("join",700,675,200,100,"JOIN");
    Button* playerSelectButton =  new Button("playerSelect",400,600,800,50,"Type here the player name you wish to play against, then click JOIN.");
    Button* gameListButton = new Button("gameList",61,175,1478,400,"");
    //Button* lobbyWarning = new Button("warning",0,800,1600,30,"");
    Button* backButtonGame = new Button("back",61,40,200,100,"BACK");


    Menu* lobbyMenu = new Menu("lobby",{backButtonGame,refreshButton,hostButton,playerSelectButton,joinButton,gameListButton, /*lobbyWarning*/},&window);

    gameMenus = {mainMenu,loginMenu,registerMenu,lobbyMenu,gameMenu};

    loadPiecesSprites();
    loadBoardSprites();
    loadTitle();
}

void GameGUI::loadPiecesSprites() {

    string path = "rsc/textures/piece/";

    unpromoted.loadFromFile(path + "no_promo.png");
    kingTexture.loadFromFile(path + "king.png");
    kingGuestTexture.loadFromFile(path + "guest_king.png");
    pawnTexture.loadFromFile(path + "pawn.png");
    pawnPromotedTexture.loadFromFile(path + "pawn_promo.png");
    lanceTexture.loadFromFile(path + "lance.png");
    lancePromotedTexture.loadFromFile(path+ "lance_promo.png");
    knightTexture.loadFromFile(path + "knight.png");
    knightPromotedTexture.loadFromFile(path + "knight_promo.png");
    silverTexture.loadFromFile(path + "silver.png");
    silverPromotedTexture.loadFromFile(path + "silver_promo.png");
    goldenTexture.loadFromFile(path + "golden.png");
    bishopPromotedTexture.loadFromFile(path + "bishop_promo.png");
    bishopTexture.loadFromFile(path + "bishop.png");
    rookTexture.loadFromFile(path + "rook.png");
    rookPromotedTexture.loadFromFile(path + "rook_promo.png");

    sf::Sprite unpromotedSprite;
    sf::Sprite kingSprite;
    sf::Sprite kingGuestSprite;
    sf::Sprite pawnSprite;
    sf::Sprite pawnPromotedSprite;
    sf::Sprite lanceSprite;
    sf::Sprite lancePromotedSprite;
    sf::Sprite knightSprite;
    sf::Sprite knightPromotedSprite;
    sf::Sprite silverSprite;
    sf::Sprite silverPromotedSprite;
    sf::Sprite goldenSprite;
    sf::Sprite bishopSprite;
    sf::Sprite bishopPromotedSprite;
    sf::Sprite rookSprite;
    sf::Sprite rookPromotedSprite;

    kingSprite.setTexture(kingTexture); kingSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    unpromotedSprite.setTexture(unpromoted); unpromotedSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    kingGuestSprite.setTexture(kingGuestTexture); kingGuestSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    pawnSprite.setTexture(pawnTexture); pawnSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    pawnPromotedSprite.setTexture(pawnPromotedTexture); pawnPromotedSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    lanceSprite.setTexture(lanceTexture); lanceSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    lancePromotedSprite.setTexture(lancePromotedTexture); lancePromotedSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    knightSprite.setTexture(knightTexture);  knightSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    knightPromotedSprite.setTexture(knightPromotedTexture);  knightPromotedSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    silverSprite.setTexture(silverTexture);  silverSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    silverPromotedSprite.setTexture(silverPromotedTexture);  silverPromotedSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    goldenSprite.setTexture(goldenTexture);  goldenSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    bishopSprite.setTexture(bishopTexture);  bishopSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    bishopPromotedSprite.setTexture(bishopPromotedTexture); bishopPromotedSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    rookSprite.setTexture(rookTexture); rookSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);
    rookPromotedSprite.setTexture(rookPromotedTexture); rookPromotedSprite.setOrigin(pieceOffsetY/2,pieceOffsetX/2);


    piecesSprites.insert(pair<string,vector<sf::Sprite>>("king",{kingSprite,kingGuestSprite}));
    piecesSprites.insert(pair<string,vector<sf::Sprite>>("pawn",{pawnSprite,pawnPromotedSprite}));
    piecesSprites.insert(pair<string,vector<sf::Sprite>>("lance",{lanceSprite,lancePromotedSprite}));
    piecesSprites.insert(pair<string,vector<sf::Sprite>>("knight",{knightSprite,knightPromotedSprite}));
    piecesSprites.insert(pair<string,vector<sf::Sprite>>("silver",{silverSprite,silverPromotedSprite}));
    piecesSprites.insert(pair<string,vector<sf::Sprite>>("golden",{goldenSprite,unpromotedSprite}));
    piecesSprites.insert(pair<string,vector<sf::Sprite>>("rook",{rookSprite,rookPromotedSprite}));
    piecesSprites.insert(pair<string,vector<sf::Sprite>>("bishop",{bishopSprite,bishopPromotedSprite}));

}


void GameGUI::loadBoardSprites() {

    string path = "rsc/textures/board/";

    boardT.loadFromFile(path + "table.png");
    handT.loadFromFile(path + "tegoma.png");
    backgroundT.loadFromFile(path + "background.png");

    board.setTexture(boardT);
    upHand.setTexture(handT);
    downHand.setTexture(handT);
    background.setTexture(backgroundT);

    background.setPosition(0,0);


    board.setPosition(0,0);
    upHand.setPosition(47,475);
    downHand.setPosition(1199,30);

}

void GameGUI::drawBoard() {

    //cout<<"[GameGUI::drawBoard()] I am drawing the board.\n";
    //window.draw(background);
    window.draw(board);
    window.draw(upHand);
    window.draw(downHand);
}

void GameGUI::drawMenu(string name) {

    //cout<<"[GameGUI::drawMenu] I am about to draw menu: "<<name<<'\n';

    window.clear(sf::Color::White);
    window.draw(background);

    for (auto it : gameMenus)
        if (it->getName() == name)
            {
            //cout<<"[GameGUI::drawMenu] Game menu found!. I am drawing it\n";
            it->drawMenu();
            return;
            }

    window.display();
}

Menu* GameGUI::getMenu(string menuName){

   for (auto it : gameMenus)
        if (it->getName() == menuName)
            return it;

    return NULL;
}

string GameGUI::getInputFromClient(Menu* menu,Button* field,int type) {

    string input = "";
    string hiddenInput;
    //window.clear(sf::Color::White);
    //menu->drawMenu();
    //window.display();
    char letter;

    clearEventQueue();

    while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {

                if (event.type == sf::Event::MouseMoved)
                    if (!field->isClicked(sf::Mouse::getPosition().x,sf::Mouse::getPosition().y))
                        return input;

                if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode < 128 && event.text.unicode > 32)
                            {
                                letter = static_cast<char>(event.text.unicode);
                                input.push_back(letter);
                                hiddenInput.push_back('*');

                                if (type == VISIBLE)
                                    field->setText(input);
                                    else
                                    field->setText(hiddenInput);

                            }

                        if (event.text.unicode == '\b' && input.size()>0) //backspace
                            {
                                input.pop_back();
                                hiddenInput.pop_back();

                                if (type == VISIBLE)
                                    field->setText(input);
                                    else
                                    field->setText(hiddenInput);

                            }
                    }

            window.clear(sf::Color::White);
            window.draw(background);
            menu->drawMenu();
            window.display();
            }

        }

}

bool GameGUI::isBoardClicked(int x, int y) {

    int boardWidth = board.getGlobalBounds().width;
    int boardHeight = board.getGlobalBounds().height;
    int boardX = getBoardPositionX();
    int boardY = getBoardPositionY();

    return ( (x>= boardX) && (x<= boardX + boardWidth) ) && ( (y>=boardY) && (y<= boardY + boardHeight) );
}

bool GameGUI::isHandClicked(int x, int y) {

    int upHandWidth = upHand.getGlobalBounds().width;
    int upHandHeight = upHand.getGlobalBounds().height;
    int upHandX = getUpHandPositionX();
    int upHandY = getUpHandPositionY();

    return ( (x>= upHandX) && (x<= upHandX + upHandWidth) ) && ( (y>=upHandY) && (y<= upHandY + upHandHeight) );
}

int GameGUI::getHandClickedPiecePosition(int x, int y) {

    int upHandOriginX = getUpHandPositionX();
    int pieceOffsetX = getPieceOffsetX();
    int j = 1, i = 1;
    int upHandOriginY = getUpHandPositionY();
    int pieceOffsetY = getPieceOffsetY();
    int result;


    while ( upHandOriginX + j*pieceOffsetX < x)
        j++;

    while ( upHandOriginY + i*pieceOffsetY < y)
        i++;

    return (i-1)*4 + j - 1;
}

int GameGUI::getBoardClickedPositionX(int x, int y) {

    int boardOriginX = getBoardPositionX();
    int pieceOffsetX = getPieceOffsetX();
    int result = 1;

    while ( boardOriginX + result*pieceOffsetX < x)
            result++;

    return result;
}

int GameGUI::getBoardClickedPositionY(int x,int y) {

    int boardOriginY = getBoardPositionY();
    int pieceOffsetY = getPieceOffsetY();
    int result = 1;

    while ( boardOriginY + result*pieceOffsetY < y)
            result++;

    return result;
}

void GameGUI::loadTitle() {

    string path = "rsc/textures/title.png";
    titleT.loadFromFile(path);
    title.setTexture(titleT);
}

void GameGUI::drawTitle() {

    window.draw(title);
    window.display();
}

void GameGUI::drawGameScreen() {

    window.clear(sf::Color::White);
    drawMenu("game");
    drawBoard();
    //drawPieces();
}

void GameGUI::clearEventQueue() {

    sf::Event eventConsumer;

    while (window.pollEvent(eventConsumer));
}

