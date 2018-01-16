# Shogi Net

---
## Packages

* SFML
    * sudo apt-get install libsfml-dev

---
## How to install

* For server, go to *server* folder and run *make* command;
* For client, go to *client* folder and run *make* command.

---
## How to use

0. Make sure that the server is running. Go to *server* folder and run *./server.bin* command. To start the client, go to *client* folder and run *./client.bin 127.0.0.1 2906* command.

### Main Menu

1. When the client is launched, a window of fixed 1600x900 resolution will appear on the screen, displaying the *Main Menu* of the *Shogi Net* Client. Two buttons are displayed: *Start* and *Exit*;
2. When the *Start* button is pressed, the client is redirected to the *Login* screen. If the server is not running, the game window closes, prompting a message in the console;

### Login && Register Menu

3. The client is promted to login, providing their username and password, then clicking the *Login* button. If he or she does not have an account, they will have to register first. 
4. If the client wish to register, then the *Register* button must be pressed. The *Register* screen will be displayed. The client is now prompted to provide a username of *maximum 16 characters*, with *only upper and/or lower letters and numbers*, and a password between *8-20 characters* (same character restrictions from above), followed by a confirm password field. If the provided credentials are correct, if the username was not being used by another account and if both password and confirm password are matched, the registration is considered successful. After registration, the client is redirected to the *Lobby* screen.
* Note 1: please make sure not to move the mouse while writing your credentials. Double click these fields before writing.
* Note 2: there are 2 users in the system that can be used for testing: (admin,1234) and (asd,123).

### Lobby Menu

5. After the user logs in, the *Lobby* screen is displayed. The client is now able to see a list of games in which can join, each game having displayed the host name, his or her number of wins and total matches. A *Host* button is also displayed, along with a *Refresh* button, *Join* button and a status bar that prompts messages from server.
6. By clicking the Refresh button, the list of available games in which the user can join is updated and displayed on the screen.
7. If the *Host* is pressed, the client enters in a wait hold state, awaiting for another player to join his or her game. The game is now displayed in the lobby and anyone can join. If someone joins the hosted game, the match will start. It is recommended not to close the client while waiting for someone to join.
8. If a user wishes to join the game hosted by another player, he or she can do so by typing that player name in the status bar, then pressing the *Join* button. This triggers the start of the match.

### Game Menu

9. When the match starts, the game screen is displayed. Both players can see whether is their turn or not, as a message will always be displayed on the screen above the player hand (a wooden rectangle at the left of the board) in a *status bar*. Both players will see the game table bottom-up, from their own perspective. A *Forfeit* button is also displayed in the upper-left corner.
10. If the current player right-clicks on a piece from the table or from the captured pieces zone, the corresponding squares in which the piece can be moved or placed will be highlighted in green. Afterwards if the player clicks a highlighted square.
11. If the movement has brought the piece in the promotion zone, the client will be requested to confirm promotion or not. Left-clicking the *status bar* confirms the promotion, while left-clicking anywhere on the board declines it. Wether the promotion occurs or not, a move will be registered and sent to the server.
12. After the move is sent to the server, the current player is now waiting for their opponent to make a move.
13. The opponent is waiting all this time until the server sends him or her a command to update the board state. The opponent is now able to make a move.
14. The game continues until an end game signal is received from the server. A game can end in several ways:
    * one of the players press the *Forfeit* button. That player is disconnected from the server and redirected to *Main Menu*. Its opponent is declared winner;
    * one of the players checkmate the other player. The first one is declared winner, while the other one is declared defeated;
    * one of the players is in check, but its current move keep the king in check. This player is declared defeated and its opponent wins the game.
15. When the game is over, an appropiate message will be displayed on the *status bar*. The *Forfeit* button becomes a *Back* button now. When pressed, the client is disconnected from the server and redirected to *Main Menu*.

---
## Known issues
* The server does not save information about new users or user progress if killed.


---
## Bibliography

* Server C code starting point. Retrieved from [here](https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerConcThread/servTcpConcTh2.c);
* Client C code starting point. Retrieved from [here](https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerConcThread/cliTcpNr.c);
* Shogi rules and terminology. Retrieved from [here](https://en.wikipedia.org/wiki/Shogi);
* LNCS format and document template. Retrieved from [here](http://www.springer.com/gp/computer-science/lncs/conference-proceedings-guidelines);
* Information about TCP. Retrieved from [here](https://profs.info.uaic.ro/~computernetworks/cursullaboratorul.php);
* A portion of the server diagram (slide 47). Retrieved from [here](https://profs.info.uaic.ro/~computernetworks/files/5rc_ProgramareaInReteaI_en.pdf);
* Multithreading tutorial. Retrieved from [here](https://solarianprogrammer.com/2011/12/16/cpp-11-thread-tutorial/);
* SFML tutorials. Retrieved from [here](https://www.sfml-dev.org/tutorials/2.4/);
* SFML documentation. Retrieved from [here](https://www.sfml-dev.org/documentation/2.4.2/);
* How to get dimensions of text in SFML. Retrieved from [here](https://stackoverflow.com/questions/13244928/getting-dimensions-of-text-in-sfml);
* How to center text into button in SFML. Inspired from [here](https://stackoverflow.com/questions/27806077/sfml-drawing-centered-text);
* C++ documentation pages for [String](http://www.cplusplus.com/reference/string/string/), [Vector](http://www.cplusplus.com/reference/vector/vector/), [Unordered Set](http://www.cplusplus.com/reference/unordered_set/unordered_set/), [Thread](http://www.cplusplus.com/reference/thread/thread/)
* Conversion from string to char array. Retrieved from [here](https://stackoverflow.com/questions/13294067/how-to-convert-string-to-char-array-in-c);
* Conversion from int to string. Retrived from [here](https://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c);
* How to use C++ ternary operator ?. Retrieved from [here](http://www.cplusplus.com/forum/articles/14631/);
* How to install SFML with CodeBlocks. Revrieved from [here](https://www.youtube.com/watch?v=ZkSFRPNLZO0);
* Wooden textures for board, buttons and captured pieces zones. Retrieved from [here](https://jammurch.deviantart.com/art/45-Wood-Textures-11874972);
* Merge font. Retrieved from [here](http://www.1001fonts.com/merge-font.html#styles);
* Mistuki font used to write Main menu title. Retrieved from [here](http://www.fontspace.com/m%C3%A5ns-greb%C3%A4ck/mistuki-1-personal-use);
* How to create arrowheads in Illustrator. Retrieved from [here](https://luanneseymour.wordpress.com/2012/10/30/creating-arrows-and-arrowheads-in-illustrator-cs6/);