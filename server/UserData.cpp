#include "UserData.h"
#include <iostream>

UserData::UserData(string username, string password){

this->username = username;
this->password = password;
nrVictories = 0;
nrMatches = 0;
}

UserData::UserData(){

    this->username= "";
    this->password = "";
    this->nrMatches = 0;
    this->nrVictories = 0;
}

istream& operator >> (istream& in, UserData& userData)
{
    in>>userData.username>>userData.password>>userData.nrMatches>>userData.nrVictories;
    return in;
};

ostream& operator << (ostream& out, const UserData& userData)
{
    out<<userData.username<<' '<<userData.password<<' '<<userData.nrMatches<<' '<<userData.nrVictories;
    return out;
};

/*UserData& UserData::operator= (const UserData& userData)
{
    username = userData->getUsername();
    this->password = userData.getPassword();
    this->nrMatches = userData->getNrMatches();
    this->nrVictories = userData->getNrVictories();
    this->userIsLoggedIn = userData->getLoginStatus();
    return *this;
}*/
