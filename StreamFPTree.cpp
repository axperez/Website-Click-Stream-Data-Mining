// =========================================================
//      StreamFPTree.cpp
// 
// ---------------------------------------------------------
// Author:      Axel Perez
// Date:        7/12/20
// =========================================================

#include "StreamFPTree.hpp"
using namespace std;

StreamNode::StreamNode(string inputUrl) {
    url = inputUrl;
}

void StreamFPTree::insert(vector<string> url_path) {
    int i, j;
    StreamNode * sp;
    string lastUrl = url_path[url_path.size() - 1];

    if (streamHT.count(lastUrl) == 0) {
        streamHT[lastUrl] = new StreamNode(lastUrl);
        streamHT[lastUrl]->count++;
    }
    else {
        streamHT[lastUrl]->count++;
    }

    for (i = url_path.size() - 2; i >= 0; i--) {
        sp = streamHT[url_path[i]];
        for (j = i + 1; j < url_path.size(); j++) {
            if (sp->children.count(url_path[j]) == 0) {
                sp->children[url_path[j]] = new StreamNode(url_path[j]);
            }
            sp = sp->children[url_path[j]];
        }
        sp->count++;
    }
}

map<int, vector<vector<string>>> StreamFPTree::getMFRs(int suppThresh) {
    vector<string> path;
    map<int, vector<vector<string>>> returnMFRs;

    for(auto &urlHead: streamHT) {
        dfsHelper(urlHead.second, suppThresh, path);
    }

    returnMFRs = mfrs;
    mfrs.clear();

    return returnMFRs;
}

void StreamFPTree::freeTree(StreamNode * head) {
    for (pair<string, StreamNode *> element : head->children) {
        freeTree(element.second);
    }

    delete head;
}

StreamFPTree::~StreamFPTree() {
    for (pair<string, StreamNode *> element : streamHT) {
        freeTree(element.second);
    }
}

void StreamFPTree::dfsHelper(StreamNode *head, int suppThresh, vector<string> path) {
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