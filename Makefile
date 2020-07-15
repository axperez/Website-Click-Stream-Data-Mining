all:	P2.cpp P2.hpp
		g++ -std=c++11 -o P2 P2.cpp
debug:	P2.cpp P2.hpp
		g++ -g -std=c++11 -o P2 P2.cpp
clean:
		rm -rf *.o *~ P2 core
