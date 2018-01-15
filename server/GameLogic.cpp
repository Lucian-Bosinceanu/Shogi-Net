#include "GameLogic.h"
#include <iostream>

using namespace std;

GameLogic::GameLogic() {

    gameBoard = new GameBoard();
    upKingPosition = { 9,5 };
    downKingPosition = { 1,5 };

    cout<<"[GameLogic()] GameLogic instance created!\n";
}

GameLogic::~GameLogic() {

    gameBoard->~GameBoard();
    cout<<"[~GameLogic()] GameLogic instance deleted!\n";
}

Position GameLogic::getUpKingPosition() {

    return upKingPosition;
}

Position GameLogic::getDownKingPosition() {

    return downKingPosition;
}

Position GameLogic::getKingPositionWithOrientation(short int orientation) {

    if (orientation == UP)
        return upKingPosition;
        else
        return downKingPosition;
}

bool GameLogic::columnHasPawn(int column, short int orientation) {

    int i;
    for (i = 1; i <= 9; i++)
        if (gameBoard->getPieceAtPosition(i,column) != NULL)
            if (gameBoard->getPieceAtPosition(i,column)->getName() == "pawn" &&
                gameBoard->getPieceAtPosition(i,column)->getOrientation() == orientation)
                {
                cout<<"[GameLogic::columnHasPawn()] Pawn found at line "<<i<<" and column "<<column<<'\n';
                return true;
                }

    cout<<"[GameLogic::columnHasPawn()] Column "<<column<<" does not have any pawns.\n";
    return false;
}

bool GameLogic::dropPiece(string pieceName, Position dropPosition, short int playerOrientation) {

    ShogiPiece* pieceToDrop = gameBoard->removePieceFromHand(playerOrientation,pieceName);

    if (pieceName == "pawn")
        {
        cout<<"[GameLogic::dropPiece] Because a pawn is dropped, I am checking to see if it's a valid pawn drop\n";
        if (columnHasPawn(dropPosition.lin,dropPosition.col))
            {
                gameBoard->dropPiece(pieceToDrop,dropPosition);
                return INVALID_DROP;
            }

        gameBoard->dropPiece(pieceToDrop,dropPosition);

        if (isKingCheckMated(-1*playerOrientation))
            return INVALID_DROP;
            else
            return VALID_DROP;

        //return columnHasPawn(dropPosition.col,playerOrientation) || isKingCheckMated(-1*playerOrientation);
        //cout<<"[GameLogic::dropPiece] It was a valid pawn drop\n";
        }

    gameBoard->dropPiece(pieceToDrop,dropPosition);

    cout<<"[GameLogic::dropPiece] I am dropping a "<<pieceName<<" at position"<<dropPosition.lin<<' '<<dropPosition.col<<'\n';

    cout<<"[GameLogic::dropPiece] Valid drop\n";
    return VALID_DROP;
}

bool GameLogic::isCheckFromPiece(ShogiPiece* attackingPiece) {

    cout<<"[GameLogic::isCheckFromPiece] I am checking if the piece from "<<attackingPiece->getPosition().lin<<' '<<attackingPiece->getPosition().col<<" is checking the king.\n";

    cout<<"[GameLogic::isCheckFromPiece] I am getting opposing king orientation.\n";
    short int opposingKingOrientation = -1*attackingPiece->getOrientation();
    cout<<"[GameLogic::isCheckFromPiece] Opposing king orientation is "<<opposingKingOrientation<<'\n';

    cout<<"[GameLogic::isCheckFromPiece] I am getting opposing king position \n";
    Position opposingKingPosition = getKingPositionWithOrientation(opposingKingOrientation);
    cout<<"[GameLogic::isCheckFromPiece] Opposing king is at: "<<opposingKingPosition.lin<<' '<<opposingKingPosition.col<<'\n';

    cout<<"[GameLogic::isCheckFromPiece] I am getting the next possible moves of attacking piece.\n";
    unordered_set<Position*> possibleFutureMovements = gameBoard->getAllPossibleMovementLocationsForPieceFrom(attackingPiece->getPosition());

    cout<<"[GameLogic::isCheckFromPiece] The attacking piece can move at ";
    for (auto it : possibleFutureMovements)
        cout<<it->lin<<' '<<it->col<<'\n';

    for (auto it : possibleFutureMovements)
        if (it->lin == opposingKingPosition.lin && it->col == opposingKingPosition.col)
            return true;

    return false;

    /*if ( possibleFutureMovements.find(&opposingKingPosition) != possibleFutureMovements.end() )
        cout<<"[GameLogic::isCheckFromPiece] The "<<attackingPiece->getName()<<" from position "<<attackingPiece->getPosition().lin<<' '<<attackingPiece->getPosition().col<<
        " has checked the opposing King!\n";
    */
    //return possibleFutureMovements.find(&opposingKingPosition) != possibleFutureMovements.end();

}

bool GameLogic::isCheckFromPiece(Position attackingPiecePosition) {

    cout<<"[GameLogic::isCheckFromPiece] I am checking if the piece from "<<attackingPiecePosition.lin<<' '<<attackingPiecePosition.col<<" is checking the king.\n";
    ShogiPiece* attackingPiece = gameBoard->getPieceAtPosition(attackingPiecePosition);
    return isCheckFromPiece(attackingPiece);
}

bool GameLogic::isKingCheckMated(short int kingOrientation) {

    cout<<"[GameLogic::isKingCheckMated] I am getting all the pieces of the player with orientation: "<<-1*kingOrientation<<'\n';
    unordered_set<ShogiPiece*> opposingArmy = gameBoard->getPiecesOfPlayer(-1*kingOrientation);
    Position initialKingPosition = getKingPositionWithOrientation(kingOrientation);
    cout<<"[GameLogic::isKingCheckMated] Kin position identified at "<<initialKingPosition.lin<<' '<<initialKingPosition.col<<'\n';
    unordered_set<Position*> kingMovementOptions = gameBoard->getAllPossibleMovementLocationsForPieceFrom(initialKingPosition);

    cout<<"[GameLogic::isKingCheckMated] The king is able to move to this positions: \n";

    for (auto it : kingMovementOptions)
        cout<<it->lin<<' '<<it->col<<'\n';


    ShogiPiece* possibleCapture;
    string possibleCapturedPieceName;
    bool reverseMove = false;
    bool kingChecked;
    bool isPromoted;

    for (auto it : kingMovementOptions)
        {
            if (it->lin == initialKingPosition.lin && it->col == initialKingPosition.col)
                continue;

            possibleCapture = gameBoard->getPieceAtPosition(*it);
            reverseMove = false;
            kingChecked = false;
            isPromoted = false;

            cout<<"[GameLogic::isKingCheckMated] Analysing position "<<it->lin<<' '<<it->col<<'\n';
            if (possibleCapture != NULL && possibleCapture->getOrientation() != kingOrientation)
                {
                    possibleCapturedPieceName= possibleCapture->getName();
                    reverseMove = true;
                    if (possibleCapture->getPromotionStatus() == PROMOTED)
                        isPromoted = true;
                    opposingArmy.erase(possibleCapture);
                    cout<<"[GameLogic::isKingCheckMated] There is an enemy "<<possibleCapturedPieceName<<" there.\n";
                }

            //cout<<"[GameLogic::isKingCheckMated] I am moving the king to the position from above.\n";
            movePiece(initialKingPosition,*it,kingOrientation);

            for (auto piece : opposingArmy)
                if ( isCheckFromPiece(piece) )
                    {
                         kingChecked = true;
                         break;
                    }

            movePiece(*it,initialKingPosition,kingOrientation);

            if (reverseMove)
                {
                    dropPiece(possibleCapturedPieceName,*it,kingOrientation);
                    possibleCapture->switchOrientation();
                    opposingArmy.insert(possibleCapture);
                    if (isPromoted)
                        possibleCapture->promote();
                }

            if (!kingChecked)
                {
                    cout<<"[GameLogic::isKingCheckMated] The king is not checkmated. It can move to "<<it->lin<<' '<<it->col<<'\n';
                    opposingArmy.clear(); kingMovementOptions.clear();
                    return false;
                }

        }

    unordered_set<Position*> whereCurrentFriendlyPieceCanMove;
    unordered_set<Position*> whereFriendsCanMove;
    unordered_set<ShogiPiece*> friendPieces = gameBoard->getPiecesOfPlayer(kingOrientation);

    friendPieces.erase( friendPieces.find(gameBoard->getPieceAtPosition(initialKingPosition)) );

    for (auto it : friendPieces)
        {
            whereCurrentFriendlyPieceCanMove = gameBoard->getAllPossibleMovementLocationsForPieceFrom(it->getPosition());
            whereFriendsCanMove.insert( whereCurrentFriendlyPieceCanMove.begin(), whereCurrentFriendlyPieceCanMove.end() );
            whereCurrentFriendlyPieceCanMove.clear();
        }

    cout<<"[GameLogic::isKingCheckMated] Friendly pieces can attack at:\n";
    for (auto it : whereFriendsCanMove)
        cout<<it->lin<<' '<<it->col<<'\n';

    for (auto piece : opposingArmy)
        if ( isCheckFromPiece(piece) )
            {
                 kingChecked = true;
                 cout<<"[GameLogic::isKingCheckMated] King is checked by a "<<piece->getName()<<" from position "<<piece->getPosition().lin<<' '<<piece->getPosition().col<<'\n';

                 for (auto it : whereFriendsCanMove)
                    if ( it->lin == piece->getPosition().lin && it->col == piece->getPosition().col )
                        {
                            cout<<"[GameLogic::isKingCheckMated] However, there is a friendly piece that can capture it.\n";
                            return false;
                        }

                if (kingChecked)
                    break;
            }

    whereCurrentFriendlyPieceCanMove.clear(); whereFriendsCanMove.clear(); friendPieces.clear(); opposingArmy.clear(); kingMovementOptions.clear();
    cout<<"[GameLogic::isKingCheckMated] The king is checkmated.\n";
    return true;
}

Position GameLogic::getMirroredPosition(Position position) {

    Position result;
    result.lin = 10 - position.lin;
    result.col = 10 - position.col;

    return result;
}

void GameLogic::movePiece(Position from, Position to, short int playerOrientation) {

cout<<"[GameLogic::movePiece()] I am trying to move the piece from "<<from.lin<<' '<<from.col<<" to "<<to.lin<<' '<<to.col<<'\n';

short int side = playerOrientation;

cout<<"[GameLogic::movePiece()] I am about to move a "<<gameBoard->getPieceAtPosition(from)->getName()<<" with orientation "<<side<<'\n';

if (gameBoard->getPieceAtPosition(to) == NULL)
    gameBoard->movePiece(from,to);
    else
    if ( side != gameBoard->getPieceAtPosition(to)->getOrientation() )
        {
        cout<<"[GameLogic::movePiece] I am going to capture the piece from "<<to.lin<<' '<<to.col<<'\n';
        capturePiece(to,playerOrientation);
        gameBoard->movePiece(from,to);
        }

if (gameBoard->getPieceAtPosition(to)->getName() == "king")
    if (gameBoard->getPieceAtPosition(to)->getOrientation() == UP)
        upKingPosition = to;
        else
        downKingPosition = to;

cout<<"[GameLogic::movePiece()] Done.\n";

}

void GameLogic::capturePiece(Position capturedPiece, short int playerOrientation) {

    cout<<"[GameLogic::capturePiece] I am about to capture the piece from "<<capturedPiece.lin<<' '<<capturedPiece.col<<'\n';
    ShogiPiece* capturedPiecePointer = gameBoard->getPieceAtPosition(capturedPiece);

    cout<<"[GameLogic::capturePiece] It's a "<<capturedPiecePointer->getName()<<" with orientation "<<capturedPiecePointer->getOrientation()<<'\n';
    cout<<"[GameLogic::capturePiece] I am adding this piece to this player hand: "<<-1*capturedPiecePointer->getOrientation()<<'\n';
    gameBoard->addPieceInHand(playerOrientation,capturedPiecePointer);

    capturedPiecePointer->setCaptured();
}

void GameLogic::promotePiece(Position promotedPiece) {

    gameBoard->getPieceAtPosition(promotedPiece)->promote();
}
