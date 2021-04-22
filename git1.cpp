#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "git.hpp"

using namespace std;


void miniGit::removeFiles() {
    cout << "Enter the file name: " << endl;
    string fileName;
    cin >> fileName;

    singlyNode* curr = currHead;
    singlyNode* prev = NULL;

    while(curr != NULL) {
        if (curr->fileName == fileName) {
            if (prev == NULL) {
                currHead = NULL;   
            }
            else {
                prev->next = curr->next;
            }
            delete curr;
            curr = NULL;
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

string newFileVersion (string fileName, string oldfileVersion) {
    string newFileVersion = "";
    if (oldfileVersion == "") {
        newFileVersion = "__00__" + fileName;
    }
    else {
        int fileASCII = convertToASCII(oldfileVersion); // receives the version number as an int
        // if (fileASCII < 10) "file0" + ...
        newFileVersion = "__" + to_string((fileASCII - 1194) % 96) + "__" + fileName; // gets the next version name
    }

    ofstream newVersion(newFileVersion); // creates and opens the new version file

    ifstream toCommit(fileName); // opens the file that is being commited
    string line;

    while(getline(toCommit, line)) { // copies line by line
        newVersion << line << endl;
    }

    newVersion.close(); // closes the files
    toCommit.close();

    return newFileVersion;
}

Ht_item* hashKey (string fileName, string fileVersion) {
    Ht_item* nHTI = new Ht_item;
    int fileASCII = convertToASCII(fileName);
    nHTI->key = (fileASCII - 814) % 48;
    nHTI->fileVersion = newFileVersion(fileName, fileVersion);
    
    return nHTI;
}

bool isNewVerion (string currFileName, string oldFileName) {
    ifstream currFile(currFileName); // opens the file that is being commited
    ifstream oldFile(oldFileName); // opens old file
    string line;
    string oldLine;

    while(!currFile.eof() || !oldFile.eof()) { // copies line by line
        currFile >> line;
        oldFile >> oldLine;
        if (line != oldLine) {
            cout << "true" << endl;
            currFile.close();
            oldFile.close();
            return true;
        }
    }
    if (!(currFile.eof() && oldFile.eof())) {
        cout << "true" << endl;
        currFile.close();
        oldFile.close();
        return true;
    }

    currFile.close();
    oldFile.close();
    cout << "false" << endl;
    return false;
}

void miniGit::commit () {
    // array of HT, with newst version at the end of linked list for each 'fileName'
    // compares, line by line, the inside of the files for the new commits and latest version
    // if not the same, add a new node and add the file to .git dir
    if (head == NULL) {
        singlyNode* curr = currHead;
        int i = 0;

        while (curr != NULL) { // creates a new version of the file
            HT[i] = *hashKey(curr->fileName, curr->fileVersion); // adds this file to the hash table at index file#
            curr = curr->next;
            i++;
        }

        doublyNode* nn = new doublyNode;
        nn->commitNumber = 0;

        curr = currHead;
        int j = 0;

        while (curr != NULL) {
            singlyNode* nsn = new singlyNode;
            if (j == 0) {
             nn->head = nsn;
             j++;   
            }
            nsn->fileName = curr->fileName;
            nsn->fileVersion = curr->fileVersion;
            nsn->next = curr->next;

            curr = curr->next;
        }

        nn->previous = NULL;
        head = nn;
        tail = nn;
    }
    else {
        singlyNode* currSingle = currHead;

        while (currSingle != NULL) {
            Ht_item* currHTI = &HT[((convertToASCII(currSingle->fileName) - 814) % 48)];
            
            while (currHTI->next != NULL) {
                currHTI = currHTI->next;
            }

            string oldFileVerion = currHTI->fileVersion;
            cout << oldFileVerion << endl;
            if (isNewVerion(currSingle->fileName, oldFileVerion)) {
                Ht_item* nHTI = hashKey(currSingle->fileName, oldFileVerion);
                
                currHTI->next = nHTI;
                
            }
            currSingle = currSingle->next;
        }
    }
}