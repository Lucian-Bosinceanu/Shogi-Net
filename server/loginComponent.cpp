#include "loginComponent.h"
#include <unistd.h>
#include <fstream>
#include <cstring>

using namespace std;

ifstream fin("userDatabase.txt");
ofstream fout("userDatabase1.txt");

loginComponent::loginComponent(){

cout<<"New loginComponent instance!\n";

int i, userCount;
UserData* user;

fin>>userCount;

for (i=1;i<=userCount;i++)
    {
    user = new UserData();
    fin>>*user;
    userDatabase.push_back(user);
    }

//fin.close();
}


bool loginComponent::areValidCredentials(string username, string password, string confirmPassword){

if (username.length() > MAX_LENGTH_USERNAME || username.length() <=0)
    return false;

if (password.compare(confirmPassword) != 0)
    return false;

if (password.length() < MIN_LENGTH_PASSWORD || password.length() > MAX_LENGTH_PASSWORD)
    return false;

if (findUser(username) != NULL)
    return false;

return true;

}


loginComponent::~loginComponent(){

fout<<userDatabase.size()<<'\n';

for (auto it : userDatabase)
    fout<<it<<'\n';

userDatabase.clear();
}

void loginComponent::addNewUser(string username, string password){

UserData* user = new UserData(username,password);
userDatabase.push_back(user);
}

UserData* loginComponent::findUser(string username){

for (auto it : userDatabase)
    if ( username.compare(it->getUsername()) == 0 )
        {
        cout<<"User has been found!\n";
        return it;
        }
return NULL;
}

UserData* loginComponent::findUserByClient(int clientDescriptor){

cout<<"I'm trying to find the client with client descriptor = "<<clientDescriptor<<'\n';
cout<<"Number of clients = "<<userDatabase.size()<<'\n';
for (auto it : userDatabase)
    {
    cout<<"Client "<<it->getClientDescriptor()<<" has been found."<<'\n';
    if ( clientDescriptor == it->getClientDescriptor() )
        {
        cout<<"User has been found by Client Descriptor!\n";
        return it;
        }
    }
return NULL;

}

bool loginComponent::isUserInDatabase(string username,string password){


for (auto it : userDatabase)
    {
    if ( (username.compare(it->getUsername()) == 0) && ( password.compare(it->getPassword()) == 0) )
        {
        cout<<"User exists in the database\n";
        return true;
        }
    }

return false;

}

string loginComponent::interpret(string command, int clientDescriptor){

string credentials, username, password, confirmPassword, passwords;
string loginCommand("login ");
string registerCommand("register ");
string backCommand("back");

if (command.find(loginCommand) != std::string::npos){

    credentials = command.substr( loginCommand.length() );
    username = credentials.substr(0,credentials.find(" "));
    password = credentials.substr(credentials.find(" ")+1);

    if ( isUserInDatabase(username,password) )
        {
            if (!isUserLoggedIn(findUser(username)))
                {
                findUser(username)->login(clientDescriptor);
                cout<<"When I login current user, the received CD is "<<clientDescriptor<<" and his UserData CD is "<<findUser(username)->getClientDescriptor()<<'\n';
                return "ok";
                }
                else
                return "logged";
        }
        else
        return "inexistent";
    }

if (command.find(registerCommand) != std::string::npos) {

    credentials = command.substr(registerCommand.length() );
    username = credentials.substr(0,credentials.find(" "));
    passwords = credentials.substr(credentials.find(" ")+1);
    password = passwords.substr(passwords.find(" ")+1);
    confirmPassword = passwords.substr(passwords.find(" ")+1);

    cout<<username<<'|'<<password<<' '<<confirmPassword<<'\n';

    if ( areValidCredentials(username,password,confirmPassword) )
        {
            addNewUser(username,password);
            findUser(username)->login(clientDescriptor);
            return "ok";
        }
        else
        return "invalid";
    }

if (command.find(backCommand) !=std::string::npos)
    return "back";

return "unexpected";
}

bool loginComponent::authenticateUser(int clientDescriptor){

char command[COMMAND_MAX_SIZE];
char logged[]="User already logged in!";
char invalid[]="Invalid credentials. Username must have at most 16 characters. Password must have between 8-20 characters. Password and Confirm Password must match.";
char inexistent[]="Invalid credentials or the user does not exist in the database.";
char unexpected[]="Unexpected error or unknown command.";
char successV[]="Success!";
char backM[]="Returning to main menu.";
string response;

while (1)
    {
    if (read (clientDescriptor, &command,COMMAND_MAX_SIZE) <= 0)
       perror ("Eroare la read() de la client la auntentificare.\n");

    response = interpret(string(command),clientDescriptor);
    cout<<"[Server: Authentication] Proccessing command "<<command<<'\n';
    cout<<response<<'\n';
    //cout<<"Am ajuns aici";
    if (!response.compare("back"))
        {
            if ( write (clientDescriptor, &backM, sizeof(backM)) <= 0)
                    perror ("[Thread]Eroare la write() catre client.\n");
            return false;
        }

    if (!response.compare("ok"))
        {
            if ( write (clientDescriptor, &successV, sizeof(successV)) <= 0)
                perror ("[Thread]Eroare la write() catre client.\n");
            return true;
        }

    if (!response.compare("logged"))
        {
            if (write (clientDescriptor, &logged, sizeof(logged)) <= 0)
                perror ("[Thread]Eroare la write() catre client.\n");
        }

    if (!response.compare("invalid"))
        {
            if (write (clientDescriptor, &invalid, sizeof(invalid)) <= 0)
                perror ("[Thread]Eroare la write() catre client.\n");
        }

    if (!response.compare("inexistent"))
        {
            if (write (clientDescriptor, &inexistent, sizeof(inexistent)) <= 0)
                perror ("[Thread]Eroare la write() catre client.\n");
        }
        else
        {
            if (write (clientDescriptor, &unexpected, sizeof(unexpected)) <= 0)
                perror ("[Thread]Eroare la write() catre client.\n");
        }
    }

}
