#include "loginComponent.h"
#include <stdlib.h>
#include <fstream>

using namespace std;

ifstream fin("userDatabase.txt");

loginComponent::loginComponent(){

int i, userCount;
UserData* user;

fin>>userCount;

for (i=1;i<=userCount;i++)
    {
    user = new UserData();
    fin>>*user;
    userDatabase.push_back(*user);
    }


fin.close();
}

string loginComponent::interpret(string command){

string credentials, username, password, confirmPassword;
string loginCommand("login ");
string registerCommand("register ");

if (command.find(loginCommand) != std::string::npos){

    credentials = command.substr( loginCommand.length() );
    username = credentials.substr(0,credentials.find(" ")-1);
    password = credentials.substr(credentials.find(" ")+1);

    if ( isUserInDatabase(username,password) )
        {
            if (!isUserLoggedIn(findUser(username)))
                return "ok";
                else
                return "logged";
        }
        else
        return "inexistent";
    }

}


loginComponent::~loginComponent(){
userDatabase.clear();
}

void loginComponent::addNewUser(string username, string password){

UserData* user = new UserData(username,password);
userDatabase.push_back(*user);
}

UserData* loginComponent::findUser(string username){

for (auto it : userDatabase)
    if ( username.compare(it.getUsername()) == 0 )
        return &it;
return NULL;
}


bool loginComponent::isUserInDatabase(string username,string password){

for (auto it : userDatabase)
    if ( (username.compare(it.getUsername()) == 0) && ( password.compare(it.getPassword()) == 0) )
        return true;

return false;

}
