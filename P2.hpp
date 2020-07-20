// ============================================================
//      P2.hpp
// ------------------------------------------------------------
// Author :     Axel Perez
// Date :       7/12/20
// ============================================================

// include files
#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <regex>
#include <atomic>
#include "Client.hpp"
#include "StreamFPTree.hpp"

// constant declarations

//global declarations
StreamFPTree globalFPTree;
std::atomic<bool> exitThread(false);
std::mutex treeMutex;
std::mutex coutMutex;

// function prototype
void streamThread(std::string hostname_or_ip, int portno);
void handleInput(std::vector<std::thread> &myThreads);
void printMFRs(std::map<int, std::vector<std::vector<std::string>>> mfrs, int suppThresh);