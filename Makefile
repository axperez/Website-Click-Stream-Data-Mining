all:	P2.hpp P2.cpp Client.hpp Client.cpp StreamFPTree.hpp StreamFPTree.cpp
		g++ -std=c++11 -o P2 P2.cpp Client.cpp StreamFPTree.cpp -pthread
clean:
		rm -rf *.o *~ P2 core
