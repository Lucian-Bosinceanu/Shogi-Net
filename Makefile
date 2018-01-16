SERVERFLAGS = -O -std=c++14 -pthread
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
CLIENTFLAGS = -O

all:
	 g++ $(SERVERFLAGS) server/*.cpp -o server/server.bin
	 g++ $(CLIENTFLAGS) $(SFMLFLAGS) client/*.cpp -o client/client.bin

clean:
	rm -f core server/*.o
	rm -f core client/*.o