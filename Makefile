all:	P2.hpp P2.cpp Client.hpp Client.cpp StreamFPTree.hpp StreamFPTree.cpp
		g++ -std=c++11 -o P2 P2.cpp Client.cpp StreamFPTree.cpp -pthread
debug:	P2.cpp P2.hpp Client.hpp Client.cpp
		g++ -g -std=c++11 -o P2 P2.cpp
clean:
		rm -rf *.o *~ P2 core
