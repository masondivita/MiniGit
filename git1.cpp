#include <iostream>
#include <fstream>
#include <sstream>
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

void hashKey (string fileName, string fileVersion) {
    
}

void miniGit::commit () {
    Ht_item* curr = HThead;

    while(curr != NULL) {
        if (HThead->key == )
    }
    
    ifstream iFile();
    string line;


    while(getline(iFile, line)) {
        stringstream ss(line);
}