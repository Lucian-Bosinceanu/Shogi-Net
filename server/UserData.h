#include <string>
#include <iostream>

using namespace std;

class UserData {

    private:
    string username;
    string password;
    int nrMatches;
    int nrVictories;
    bool userIsLoggedIn;

    public:
    UserData();
    UserData(string username, string password);

    void setUsername(string username) {this->username = username;};
    void setPassword(string password) {this->password = password;};
    void incrementMatches() {this->nrMatches++;};
    void incrementVictories() {this->nrVictories++;};

    int getNrMatches() {return nrMatches;};
    int getNrVictories() {return nrVictories;};
    string getUsername() {return username;};
    string getPassword() {return password;};
    bool getLoginStatus() {return userIsLoggedIn;};

    void login() {this->userIsLoggedIn=true;};
    void logout() {this->userIsLoggedIn=false;};

    //UserData& operator = (const UserData &userData);
    friend istream& operator >> (istream& in, UserData& userData);
    friend ostream& operator << (ostream& out, const UserData& userData);
};
