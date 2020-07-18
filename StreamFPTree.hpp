// ============================================================
//      StreamFPTree.hpp
// ------------------------------------------------------------
// Author :     Axel Perez
// Date :       7/12/20
// ============================================================

// include files
#include <string>
#include <unordered_map>
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

        void freeTree(StreamNode * head) {
            for (std::pair<std::string, StreamNode *> element : head->children)
            {
                freeTree(element.second);
            }

            delete head;
        }

        ~StreamFPTree() {
            for (std::pair<std::string, StreamNode *> element : streamHT)
            {
                freeTree(element.second);
            }
        }
};
/*
int main(void) {
    StreamFPTree tree1;
    std::vector<std::string> url_path1 {"test.com/"};
    std::vector<std::string> url_path2 {"test.com/", "test.com/home/"};
    std::vector<std::string> url_path3 {"test.com/", "test.com/home/", "test.com/home/about_us"};

    tree1.insert(url_path1);
    tree1.insert(url_path2);
    tree1.insert(url_path3);
    tree1.insert(url_path1);

    
    return 0;
}
*/