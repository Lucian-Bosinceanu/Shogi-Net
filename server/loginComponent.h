#include <string>
#include <vector>
#include "UserData.h"
#define COMMAND_MAX_SIZE 128
#define MAX_LENGTH_USERNAME 16
#define MIN_LENGTH_PASSWORD 8
#define MAX_LENGTH_PASSWORD 20


using namespace std;

class loginComponent {

    private:
    vector<UserData*> userDatabase;

    void addNewUser(string username, string password);
    bool isUserInDatabase(string username, string password);

    bool isUserLoggedIn(UserData* user)
        {
        cout<<"User login status checked\n";
        return user->getLoginStatus();
        };

    bool areValidCredentials(string username, string password, string confirmPassword);
    UserData* findUser(string username);
    //void loginUser(UserData* user) { user->login();};
    //void logoutUser(UserData* user) { user->logout();};

    string interpret(string command, int clientDescriptor);

    public:
    loginComponent();
    ~loginComponent();

    UserData* findUserByClient(int clientDescriptor);
    bool authenticateUser(int clientDescriptor);
    void logoutUser(int clientDescriptor) { findUserByClient(clientDescriptor)->logout();};
    void updateDatabase();
};
