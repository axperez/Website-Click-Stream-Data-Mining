// ============================================================
//      StreamFPTree.hpp
// ------------------------------------------------------------
// Author :     Axel Perez
// Date :       7/19/20
// ============================================================

// include files
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>

// URL Node Class:
//      Node class for each url that includes its url, frequency count, 
//      and pointers to its children. This class is meant to be used as 
//      a node for the StreamFPTree class.
class UrlNode {
    public:
        std::string url;
        unsigned int count = 0;
        std::unordered_map<std::string, UrlNode *> children;

        UrlNode(std::string inputUrl);
};

// Stream Frequency Path Traversal Patterns Tree Class:
//      Header table of URLs that point to the head node
//      of a Frequency Path Traversal Patterns Tree, which
//      can keep frequency counts of forward URL paths. 
class StreamFPTree {
    public:
        std::unordered_map<std::string, UrlNode *> streamHT;        // URL header table
        std::map<int, std::vector<std::vector<std::string>>> mfrs;  // Maximal Forward References

        void insert(std::vector<std::string> url_path);
        std::map<int, std::vector<std::vector<std::string>>> getMFRs(int suppThresh);
        void freeTree(UrlNode * head);
        ~StreamFPTree();
    private:
        void dfsHelper(UrlNode *head, int suppThresh, std::vector<std::string> path);
};