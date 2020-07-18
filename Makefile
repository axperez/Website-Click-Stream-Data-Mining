all:	P2.cpp P2.hpp Client.hpp
		g++ -std=c++11 -o P2 P2.cpp -pthread
debug:	P2.cpp P2.hpp Client.hpp Client.cpp
		g++ -g -std=c++11 -o P2 P2.cpp
clean:
		rm -rf *.o *~ P2 core
