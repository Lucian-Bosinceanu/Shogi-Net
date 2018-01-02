#include <string>
#include <vector>
#include "UserData.h"
#define COMMAND_MAX_SIZE 128

using namespace std;

class loginComponent {

    private:
    vector<UserData> userDatabase;

    void addNewUser(string username, string password);
    bool isUserInDatabase(string username, string password);
    bool isUserLoggedIn(UserData* user) {return user->getLoginStatus();}
    UserData* findUser(string username);

    void loginUser(UserData* user) { user->login();};
    void logoutUser(UserData* user) { user->logout();};

    public:
    loginComponent();
    ~loginComponent();

    string interpret(string command);
};
