#include "GameGUI.h"
#include <iostream>

using namespace std;

GameGUI::GameGUI() {

    pieceOffsetX = 76;
    pieceOffsetY = 89;

    window.create(sf::VideoMode(1600,900),"Shogi Net Client");
    //this->window = &window;

    textFont.loadFromFile("rsc/fonts/arial.ttf");

    Button* exitButton = new Button("exit",1000,600,150,100,"EXIT");
    Button* playButton = new Button("play",1000,400,150,100,"PLAY" );

    Menu* mainMenu = new Menu("main",{exitButton,playButton},&window);

    Button* usernameField = new Button("username",1000,300,300,40,"Enter username");
    Button* passwordField = new Button("password",1000,400,300,40,"Enter password");
    Button* confirmPasswordField = new Button("confirmPass",1000,500,300,40,"Re-enter password");
    Button* loginButton = new Button("login",1000,600,150,100,"LOGIN");
    Button* registerLoginButton = new Button("registerL",1000,800,150,100,"REGISTER");
    Button* registerRegisterButton = new Button("registerR",1000,700,150,100,"REGISTER");
    Button* backButton = new Button("back",61,40,200,100,"Back");
    Button* warnings = new Button("warning",0,200,1600,30,"Please provide your credentials.");

    Menu* loginMenu = new Menu("login",{usernameField,passwordField,loginButton,registerLoginButton,backButton,warnings},&window);
    Menu* registerMenu = new Menu("register",{usernameField,passwordField,confirmPasswordField,registerRegisterButton,warnings,backButton},&window);

    Button* forfeitButton = new Button("forfeit",61,40,200,100,"FORFEIT");
    Button* statusButton = new Button("status",61,414,324,50,"");

    Menu* gameMenu = new Menu("game",{forfeitButton,statusButton},&window);


    Button* refreshButton = new Button("refresh",200,600,150,100,"REFRESH");
    Button* hostButton = new Button("host",400,600,150,100,"HOST");
    Button* joinButton = new Button("join",600,600,150,100,"JOIN");
    Button* playerSelectButton =  new Button("playerSelect",200,525,1200,50,"Type here the player name you wish to play against, then click JOIN.");
    Button* gameListButton = new Button("gameList",200,100,1200,400,"");
    Button* lobbyWarning = new Button("warning",0,800,1600,30,"");
    Button* backButtonGame = new Button("back",200,50,50,50,"RETURN TO MENU");


    Menu* lobbyMenu = new Menu("lobby",{backButtonGame,refreshButton,hostButton,playerSelectButton,joinButton,gameListButton},&window);

    gameMenus = {mainMenu,loginMenu,registerMenu,lobbyMenu,gameMenu};

    loadPiecesSprites();
    loadBoardSprites();
}

void GameGUI::loadPiecesSprites() {

    string path = "rsc/textures/piece/";

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

    kingSprite.setTexture(kingTexture); //kingSprite.setOrigin(458,50);
    unpromotedSprite.setTexture(unpromoted); //unpromotedSprite.setOrigin(458,50);
    kingGuestSprite.setTexture(kingGuestTexture); //kingGuestSprite.setOrigin(458,50);
    pawnSprite.setTexture(pawnTexture); //pawnSprite.setOrigin(458,50);
    pawnPromotedSprite.setTexture(pawnPromotedTexture); //pawnPromotedSprite.setOrigin(458,50);
    lanceSprite.setTexture(lanceTexture); //lanceSprite.setOrigin(458,50);
    lancePromotedSprite.setTexture(lancePromotedTexture); //lancePromotedSprite.setOrigin(458,50);
    knightSprite.setTexture(knightTexture); //knightSprite.setOrigin(458,50);
    knightPromotedSprite.setTexture(knightPromotedTexture); //knightPromotedSprite.setOrigin(458,50);
    silverSprite.setTexture(silverTexture); //silverSprite.setOrigin(458,50);
    silverPromotedSprite.setTexture(silverPromotedTexture); //silverPromotedSprite.setOrigin(458,50);
    goldenSprite.setTexture(goldenTexture); //goldenSprite.setOrigin(458,50);
    bishopSprite.setTexture(bishopTexture); //bishopSprite.setOrigin(458,50);
    bishopPromotedSprite.setTexture(bishopPromotedTexture);
    rookSprite.setTexture(rookTexture);
    rookPromotedSprite.setTexture(rookPromotedTexture);


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

    sf::Texture boardT;
    sf::Texture handT;
    sf::Texture teMarkerT;

    boardT.loadFromFile(path + "table.png");
    handT.loadFromFile(path + "tegoma.jpg");
    teMarkerT.loadFromFile(path + "te.jpg");

    board.setTexture(boardT);
    upHand.setTexture(handT);
    downHand.setTexture(handT);
    downHand.rotate(180);
    teMarker.setTexture(teMarkerT);


    board.setPosition(448,40);
    upHand.setPosition(61,486);
    downHand.setPosition(1214,40);

}

void GameGUI::drawBoard() {

    window.draw(board);
    window.draw(upHand);
    window.draw(downHand);
}

void GameGUI::drawMenu(string name) {

    //cout<<"[GameGUI::drawMenu] I am about to draw menu: "<<name<<'\n';

    for (auto it : gameMenus)
        if (it->getName() == name)
            {
            //cout<<"[GameGUI::drawMenu] Game menu found!. I am drawing it\n";
            it->drawMenu();
            return;
            }
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
    window.clear(sf::Color::White);
    menu->drawMenu();
    window.display();
    char letter;

while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::MouseMoved)
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