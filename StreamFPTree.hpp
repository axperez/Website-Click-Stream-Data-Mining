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

        StreamNode(std::string inputUrl) {
            url = inputUrl;
        }
};

class StreamFPTree {
    public:
        std::unordered_map<std::string, StreamNode *> streamHT;
        std::map<int, std::vector<std::vector<std::string>>> mfrs;

        void insert(std::vector<std::string> url_path) {
            int i, j;
            StreamNode * sp;
            std::string lastUrl = url_path[url_path.size() - 1];

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

        std::map<int, std::vector<std::vector<std::string>>> getMFRs(int suppThresh) {
            std::vector<std::string> path;
            std::map<int, std::vector<std::vector<std::string>>> returnMFRs;

            for(auto &urlHead: streamHT) {
                dfsHelper(urlHead.second, suppThresh, path);
            }

            returnMFRs = mfrs;
            mfrs.clear();

            return returnMFRs;
        }

        void freeTree(StreamNode * head) {
            for (std::pair<std::string, StreamNode *> element : head->children) {
                freeTree(element.second);
            }

            delete head;
        }

        ~StreamFPTree() {
            for (std::pair<std::string, StreamNode *> element : streamHT) {
                freeTree(element.second);
            }
        }
    private:
        void dfsHelper(StreamNode *head, int suppThresh, std::vector<std::string> path) {
            if (head->count < suppThresh) {
                return;
            }

            path.push_back(head->url);
            
            if (mfrs.count(path.size()) == 0) {
                mfrs[path.size()] = std::vector<std::vector<std::string>> {};
            }

            mfrs[path.size()].push_back(path);

            for (auto &child: head->children) {
                dfsHelper(child.second, suppThresh, path);
            }
            path.pop_back();
        }
};