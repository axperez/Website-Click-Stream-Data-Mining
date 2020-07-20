// ============================================================
//      StreamFPTree.hpp
// ------------------------------------------------------------
// Author :     Axel Perez
// Date :       7/12/20
// ============================================================

// include files
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>

// constants


// Class definitions
class StreamNode {
    public:
        std::string url;
        unsigned int count = 0;
        std::unordered_map<std::string, StreamNode *> children;

        StreamNode(std::string inputUrl);
};

class StreamFPTree {
    public:
        std::unordered_map<std::string, StreamNode *> streamHT;
        std::map<int, std::vector<std::vector<std::string>>> mfrs;

        void insert(std::vector<std::string> url_path);
        std::map<int, std::vector<std::vector<std::string>>> getMFRs(int suppThresh);
        void freeTree(StreamNode * head);
        ~StreamFPTree();
    private:
        void dfsHelper(StreamNode *head, int suppThresh, std::vector<std::string> path);
};