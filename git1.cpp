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

Ht_item* hashKey (string fileName, string fileVersion) {
    Ht_item* nHTI = new Ht_item;
    int fileASCII = convertToASCII(fileName);
    nHTI->key = fileASCII % 416;
    nHTI->fileName = fileName;
    nHTI->fileVersion = fileVersion;
    
    return nHTI;
}

int convertToASCII(string name)
{
    int total;
    for (int i = 0; i < name.length(); i++)
    {
        total += name[i];
    }
    return total;
}

void miniGit::commit () {
    // array of HT, with newst version at the end of linked list for each 'fileName'
    // compares, line by line, the inside of the files for the new commits and latest version
    // if not the same, add a new node and add the file to .git dir
    if (HT[0] == NULL) {
        singlyNode* curr = currHead;
        int i = 0;

        while (curr != NULL) {
            HT[0] = hashKey(curr->fileName, curr->fileVersion);
            curr = curr->next;
        }
    }

    
    
    ifstream iFile();
    string line;


    while(getline(iFile, line)) {
        stringstream ss(line);
}