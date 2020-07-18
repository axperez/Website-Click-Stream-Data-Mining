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
#include <unordered_set>
#include "Client.hpp"
#include "StreamFPTree.hpp"

// constant declarations

//global declarations
StreamFPTree globalFPTree;
std::mutex treeMutex;
std::mutex coutMutex;

// function prototype
