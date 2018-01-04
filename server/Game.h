#define RUNNING 3
#define READY 2
#define WAITING 1
#define UNDEFINED 0

using namespace std;

class Game {

    private:
    int client1;
    int client2;
    int status;

    public:
    Game();

    void host(int hostDescriptor) {client1 = hostDescriptor; status = WAITING;};
    void join(int joinerDescriptor) { client2 = joinerDescriptor; setReady(); };

    void setReady() { status = READY;};
    void setRunning() { status = RUNNING; };

    int getGameStatus() {return status;};
    int getHost() {return client1;};
    int getJoiner() {return client2;};

};

