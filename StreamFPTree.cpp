// =========================================================
//      StreamFPTree.cpp
// 
// ---------------------------------------------------------
// Author:      Axel Perez
// Date:        7/19/20
// =========================================================

#include "StreamFPTree.hpp"
using namespace std;

// UrlNode Class Constructor:
//      Sets input URL string to member variable of object.
UrlNode::UrlNode(string inputUrl) {
    url = inputUrl;
}

// StreamFPTree Insert Member Function:
//      Inserts URL path and subpaths into tree and either sets 
//      or increments count for those respective paths.
void StreamFPTree::insert(vector<string> url_path) {
    int i, j;
    UrlNode * sp;
    string lastUrl = url_path[url_path.size() - 1];

    // If last URL in path has never been seen, add to header table 
    // and create head node for it (previous URLS have already been
    // added in previous inserts)
    if (streamHT.count(lastUrl) == 0) {
        streamHT[lastUrl] = new UrlNode(lastUrl);
        streamHT[lastUrl]->count++;
    }
    else {
        streamHT[lastUrl]->count++;
    }
    // Create paths with nodes or increment count for full URL path and new subpaths
    for (i = url_path.size() - 2; i >= 0; i--) {    // start at second to last URL and go to start
        sp = streamHT[url_path[i]];
        for (j = i + 1; j < url_path.size(); j++) { // create path from i to the end of the URL path
            if (sp->children.count(url_path[j]) == 0) {
                sp->children[url_path[j]] = new UrlNode(url_path[j]);
            }
            sp = sp->children[url_path[j]];
        }
        sp->count++;
    }
}

// StreamFPTree Get Maximum Forward References Member Function:
//      Iterate over URL header table and do a depth first search on the
//      head node of the tree. dfsHelper will add to the member variable
//      mfrs which will then be returned.
map<int, vector<vector<string>>> StreamFPTree::getMFRs(int suppThresh) {
    vector<string> path;
    mfrs.clear();

    for(auto &urlHead: streamHT) {
        dfsHelper(urlHead.second, suppThresh, path);
    }

    return mfrs;
}

// StreamFPTree Free Tree Member Function:
//      Recursive function to free all of the heap memory from a tree
//      of UrlNodes.
void StreamFPTree::freeTree(UrlNode * head) {
    for (pair<string, UrlNode *> element : head->children) {
        freeTree(element.second);
    }

    delete head;
}

// StreamFPTree Destructor Function:
//      Iterates over URL header table and frees heap memory of each tree.
StreamFPTree::~StreamFPTree() {
    for (pair<string, UrlNode *> element : streamHT) {
        freeTree(element.second);
    }
}

// StreamFPTree Depth First Search Helper Member Function:
//      Recursive depth first search that collects URL paths with a count of at
//      least the support threshold. It adds these paths to the member variable
//      mfrs which is later returned by the getMFRS member function.
void StreamFPTree::dfsHelper(UrlNode *head, int suppThresh, vector<string> path) {
    if (head->count < suppThresh) {
        return;
    }

    path.push_back(head->url);
    
    if (mfrs.count(path.size()) == 0) {
        mfrs[path.size()] = vector<vector<string>> {};
    }

    mfrs[path.size()].push_back(path);

    for (auto &child: head->children) {
        dfsHelper(child.second, suppThresh, path);
    }
    path.pop_back();
}