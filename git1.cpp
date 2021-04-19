#include <iostream>
#include "git.hpp"

using namespace std;

void miniGit::removeFiles(string fileName) {
    singlyNode* curr = currHead;
    singlyNode* prev = NULL;

    while(curr != NULL) {
        if (curr->fileName == fileName) {
            prev->next = curr->next;
            delete curr;
            curr = 0;
            return;
        }
    }
    cout << "No file found!" << endl;
}