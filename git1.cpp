#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
        curr = curr->next;
    }
    cout << "No file found!" << endl;
}

int convertToASCII(string name)
{
    int total = 0;
    for (int i = 0; i < name.length(); i++)
    {
        total += name[i];
    }
    return total;
}

Ht_item hashKey (string fileName, string fileVersion) {
    Ht_item* nHTI = new Ht_item;
    int fileASCII = convertToASCII(fileName);
    nHTI->key = fileASCII % 416;
    nHTI->fileVersion = fileVersion;
    
    return *nHTI;
}

string newFileVersion (string fileName, string oldfileVersion) {
    int fileASCII = convertToASCII(oldfileVersion); // receives the version number as an int
    // if (fileASCII < 10) "file0" + ...
    string newFileVersion = "file" + to_string((fileASCII % 416) + 1) + ".cpp"; // gets the next version name
    
    ofstream newVersion(newFileVersion); // creates and opens the new version file

    ifstream toCommit(fileName + ".cpp"); // opens the file that is being commited
    string line;

    while(getline(toCommit, line)) { // copies line by line
        newVersion << line << endl;
    }

    newVersion.close(); // closes the files
    toCommit.close();

    return newFileVersion;
}

bool isNewVerion (string currFileName, string oldFileName) {
    ifstream currFile(currFileName + ".cpp"); // opens the file that is being commited
    ifstream oldFile(oldFileName + ".cpp"); // opens old file
    string line;
    string oldLine;

    while(getline(currFile, line) && getline(oldFile, oldLine)) { // copies line by line
        if (line != oldLine) {
            return true;
        }
    }
    return false;
}

void miniGit::commit () {
    // array of HT, with newst version at the end of linked list for each 'fileName'
    // compares, line by line, the inside of the files for the new commits and latest version
    // if not the same, add a new node and add the file to .git dir
    if (head == NULL) {
        singlyNode* curr = currHead;
        int i = 0;

        while (curr != NULL) {
            HT[0] = hashKey(curr->fileName, curr->fileVersion);
            curr = curr->next;
        }
        doublyNode* nn = new doublyNode;
        nn->commitNumber = 0;
        nn->head = currHead;
        currHead = NULL;
        nn->previous = NULL;
        head = nn;
        tail = nn;
    }
    else {
        singlyNode* currSingle = tail->head;

        while (currSingle != NULL) {
            if (isNewVerion(currHead->fileName, HT[convertToASCII(currSingle->fileName)].fileVersion)) {
                Ht_item* nHTI = new Ht_item;

                nHTI->fileVersion = newFileVersion(currSingle->fileName, currSingle->fileVersion);
                HT[convertToASCII(currSingle->fileName)].next = nHTI;
            }
        }

    }

    
    
    
}