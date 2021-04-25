#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include "git.hpp"

namespace fs = std::filesystem;
using namespace std;


miniGit::~miniGit () {
    
}

void miniGit::removeFiles () {
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

int convertToASCII (string name) {
    int total = 0;
    for (int i = 0; i < name.length(); i++)
    {
        total += name[i];
    }
    return total;
}

int fileVersionKey (string name) {
    return stoi((to_string(name[2] % 48) + to_string(name[3] % 48)));
}

int hashFunction (string fileName) {
    return ((convertToASCII(fileName) - 814) % 48);
}

string newFileVersion (string fileName, string oldfileVersion) {
    string newFileVersion = "";
    if (oldfileVersion == "") {
        newFileVersion = "__00__" + fileName;
    }
    else {
        int fileASCII = fileVersionKey(oldfileVersion); // receives the version number as an int
        if (fileASCII < 9) {
            newFileVersion = "__0" + to_string(fileASCII + 1) + "__" + fileName; // gets the next version name
        }
        else {
            newFileVersion = "__" + to_string(fileASCII + 1) + "__" + fileName; // gets the next version name
        }
        
    }

    ofstream newVersion(".minigit/" + newFileVersion); // creates and opens the new version file


    ifstream toCommit(fileName); // opens the file that is being commited
    string line;

    while(getline(toCommit, line)) { // copies line by line
        newVersion << line << endl;
    }

    newVersion.close(); // closes the files
    toCommit.close();

    return newFileVersion;
}

Ht_item* newHTI (string fileName, string fileVersion) {
    Ht_item* nHTI = new Ht_item;
    nHTI->key = hashFunction(fileName);
    nHTI->fileVersion = newFileVersion(fileName, fileVersion);
    nHTI->next = NULL;
    
    return nHTI;
}

bool isNewVerion (string currFileName, string oldFileName) {
    ifstream currFile(currFileName); // opens the file that is being commited
    ifstream oldFile(oldFileName); // opens old file
    string line;
    string oldLine;

    while(!currFile.eof() && !oldFile.eof()) { // copies line by line
        currFile >> line;
        oldFile >> oldLine;
        if (line != oldLine) {
            cout << "true" << endl;
            currFile.close();
            oldFile.close();
            return true;
        }
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

        fs::remove_all(".minigit");
        fs::create_directory(".minigit");
        

        while (curr != NULL) { // creates new versions of the files
            HT[hashFunction(curr->fileName)] = *newHTI(curr->fileName, curr->fileVersion); // adds this file to the hash table at index file#

            curr->fileVersion = HT[hashFunction(curr->fileName)].fileVersion;
            curr = curr->next;
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
        nn->next = NULL;
        head = nn;
        tail = nn;
    }
    else {
        singlyNode* currSingle = currHead;

        while (currSingle != NULL) { // goes through all the files that will be commited to see if they are new versions
            if (&HT[hashFunction(currSingle->fileName)] == NULL) { // checks if this is the first version of this file
                HT[hashFunction(currSingle->fileName)] = *newHTI(currSingle->fileName, currSingle->fileVersion); // adds it as new to the hash table
                currSingle->fileVersion = HT[hashFunction(currSingle->fileName)].fileVersion;
            } else {

                Ht_item* currHTI = NULL; // grabs the first file version from the hash table that coresponds to the current file
                Ht_item* test = &HT[hashFunction(currSingle->fileName)]; // test just fixes a seg fault error, it is only used for the following while loop

            
                while (test != NULL) { // goes to the last file version from the hash table
                    currHTI = test;
                    test = test->next;
                }

                string oldFileVerion = currHTI->fileVersion;

                if (isNewVerion(currSingle->fileName, oldFileVerion)) { // checks if the file being commited is a new verion, if it isnt, it wont be commited
                    Ht_item* nHTI = newHTI(currSingle->fileName, oldFileVerion);
                
                    currHTI->next = nHTI;
                
                }
            }
            
            currSingle = currSingle->next;
        }

        doublyNode* currDouble = head;
        doublyNode* prevDouble = NULL;

        while (currDouble != NULL) { // traverses to the last commit
            prevDouble = currDouble;
            currDouble = currDouble->next;
        }

        doublyNode* nn = new doublyNode; // new commit

        currSingle = currHead;
        singlyNode* prevSingle = NULL;
        int j = 0;

        while (currSingle != NULL) { // copies the single linked list and saves it in the commit
            singlyNode* nsn = new singlyNode;
            if (j == 0) {
             nn->head = nsn; // sets the first new node as the head
             j++;   
            }
            nsn->fileName = currSingle->fileName;
            nsn->fileVersion = currSingle->fileVersion;
            
            if (prevSingle != NULL) {
                prevSingle->next = nsn;
            }

            prevSingle = nsn;
            currSingle = currSingle->next;
        }

        nn->next = NULL;
        nn->previous = prevDouble;
        prevDouble->next = nn;
        nn->commitNumber = prevDouble->commitNumber + 1;
        tail = nn;
    }
}