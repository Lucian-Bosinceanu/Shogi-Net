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
    int clientDescriptor;

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
    int getClientDescriptor() {return clientDescriptor;};

    void login(int clientDescriptor) {this->userIsLoggedIn=true; this->clientDescriptor = clientDescriptor;};
    void logout() {this->userIsLoggedIn=false; this->clientDescriptor = -1; };

    //UserData& operator = (const UserData &userData);
    friend istream& operator >> (istream& in, UserData& userData);
    friend ostream& operator << (ostream& out, const UserData& userData);
};
