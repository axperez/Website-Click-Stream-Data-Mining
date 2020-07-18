all:	P2.cpp P2.hpp Client.hpp Client.cpp
		g++ -std=c++11 -o P2 P2.cpp
debug:	P2.cpp P2.hpp Client.hpp Client.cpp
		g++ -g -std=c++11 -o P2 P2.cpp
client:	Client.hpp Client.cpp
		g++ -g -std=c++11 -o Client Client.cpp
clean:
		rm -rf *.o *~ P2 Client core
