#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include "miniGit.hpp"

namespace fs = std::filesystem;
using namespace std;


miniGit::~miniGit () {
    singlyNode* curr = currHead;
    singlyNode* prev = NULL;

    while (curr != NULL) { // clears the current files linked list
        prev = curr;
        curr = curr->next;
        delete prev;
    }

    doublyNode* currDouble = head;
    doublyNode* prevDouble = NULL;

    while (currDouble != NULL) {// clears all the commits
        prevDouble = currDouble;
        curr = prevDouble->head;
        while (curr != NULL) { // clears the old commit linked lists
            prev = curr;
            curr = curr->next;
            delete prev;
        }
        currDouble = currDouble->next;
        delete prevDouble;
    }

    for (int i = 0; i < 101; i++) { // clears the linked lists of hash table items (the fileversions)
        Ht_item* currHTI = HT[i];
        Ht_item* prevHTI = NULL;

        while (currHTI != NULL) {
            prevHTI = currHTI;
            currHTI = currHTI->next;
            delete prevHTI;
        }
    }

    delete[] HT;
    fs::remove_all(".minigit");
}

bool search(string name, singlyNode* head){
    
    singlyNode* ptr = head;
    
    while (ptr != NULL){ //traverses linked list until filename is found
        if(ptr->fileName == name){
            return true;
        }
        ptr = ptr->next;
    }
    return false; //returns false if filename isn't found
}

void miniGit::addFiles(){
    
    string file;
    cout << "Enter the file name: " << endl; //prompts user for filename
    cin >> file;
    
    ifstream in_file(file);

    string fileVersion = ""; 

    if (in_file.fail()){
        cout << "File not found. Please enter valid filename." << endl << endl;
        return;
    }

    if(search(file, currHead)){
        cout << "File already exists in directory." << endl;
    }
    else if(currHead == NULL){ //creates new node if linked list is empty
        currHead = new singlyNode;
        currHead->fileName = file;
        currHead->next = NULL;
        currHead->fileVersion = fileVersion;
    } else {
        singlyNode* last = currHead;
        
        while(last->next){
            
            last = last->next; //traverses linked list to get last node
        }
        singlyNode* node = new singlyNode; //creates a new node and adds it after the last node in the linked list
        node->fileName = file;
        node->fileVersion = fileVersion;
        last->next = node;
    }
    in_file.close();
}

void miniGit::removeFiles () {
    cout << "Enter the file name: " << endl;
    string fileName;
    cin >> fileName;

    singlyNode* curr = currHead;
    singlyNode* prev = NULL;

    while(curr != NULL) { // traverses through the linked list looking for the file
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

int convertToASCII (string name) { // converts some string to the sum of its ascii character values
    int total = 0;
    for (int i = 0; i < name.length(); i++)
    {
        total += name[i];
    }
    return total;
}

int fileVersionKey (string name) { // extracts the version number
    return stoi((to_string(name[2] % 48) + to_string(name[3] % 48)));
}

int hashFunction (string fileName) { // extracts the file number
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

Ht_item* newHTI (string fileName, string fileVersion) { // creates a new ash tabel item
    Ht_item* nHTI = new Ht_item;
    nHTI->key = hashFunction(fileName);
    nHTI->fileVersion = newFileVersion(fileName, fileVersion);
    nHTI->next = NULL;
    
    return nHTI;
}

bool isNewVerion (string currFileName, string oldFileName) {
    ifstream currFile(currFileName); // opens the file that is being commited
    ifstream oldFile(".minigit/" + oldFileName); // opens old file
    string line;
    string oldLine;

    while(!currFile.eof() & !oldFile.eof()) { // goes through both files at the same time

        currFile >> line;
        oldFile >> oldLine;
        
        if (line != oldLine) { // if the there is a difference in the lines, it returns true
            currFile.close();
            oldFile.close();
            return true;
        }
    }
    currFile.close();
    oldFile.close();
    return false;
}

void miniGit::commit () {
    if (head == NULL) {
        singlyNode* curr = currHead;
        
        if (currHead == NULL) {
            cout << "No files to commit!" << endl; 
            return;
        }

        fs::remove_all(".minigit");
        fs::create_directory(".minigit");
        

        while (curr != NULL) { // creates new versions of the files
            HT[hashFunction(curr->fileName)] = newHTI(curr->fileName, ""); // adds this file to the hash table at index file#

            curr->fileVersion = HT[hashFunction(curr->fileName)]->fileVersion;
            curr = curr->next;
        }

        doublyNode* nn = new doublyNode;
        nn->commitNumber = 0;

        curr = currHead;
        int j = 0;

        while (curr != NULL) { // copies the single linked list and saves it in the commit
            singlyNode* nsn = new singlyNode;
            if (j == 0) {
             nn->head = nsn; // sets the first new node as the head
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
            if (HT[hashFunction(currSingle->fileName)] == NULL) { // checks if this is the first version of this file
                HT[hashFunction(currSingle->fileName)] = newHTI(currSingle->fileName, ""); // adds it as new to the hash table
                currSingle->fileVersion = HT[hashFunction(currSingle->fileName)]->fileVersion;
            } else {

                Ht_item* currHTI = NULL; // grabs the first file version from the hash table that coresponds to the current file
                Ht_item* test = HT[hashFunction(currSingle->fileName)]; // test just fixes a seg fault error, it is only used for the following while loop

            
                while (test != NULL) { // goes to the last file version from the hash table
                    currHTI = test;
                    test = test->next;
                }

                string oldFileVerion = currHTI->fileVersion; // gets the last updated file version

                if (isNewVerion(currSingle->fileName, oldFileVerion)) { // checks if the file being commited is a new verion, if it isnt, a new version wont be created
                    Ht_item* nHTI = newHTI(currSingle->fileName, oldFileVerion);
                    currSingle->fileVersion = nHTI->fileVersion;
                
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

void checkoutFile(string fileName, string fileVersion){
    ofstream currFile(fileName); //opens file to overwrite with file in specified commit
    ifstream oldFile(".minigit/" + fileVersion); //opens file in specified commit
    string line;

    while(getline(oldFile, line)) {
        currFile << line << endl; //copies oldfile into currFile
    }

    oldFile.close();
    currFile.close();
}


bool miniGit::checkout(){

    int version = -1;
    doublyNode *node = head;

    cout << "Warning: Files in current directory will be overwritten" << endl;
    cout << "Enter commit number: " << endl;

    cin >> version;

    if (version < 0) {
        
        cout << "Please enter valid commit number." << endl;
        return true;
    }
    
    if(head == NULL){

        cout << "No previous file version availible." << endl;
        return true;
    }
        
    while(node){

        if(node->commitNumber == version){

            singlyNode *curr = node->head; // grabs the head of the current node 

            while(curr){

                checkoutFile(curr->fileName, curr->fileVersion); //updates current directory with specified commit
                curr = curr->next;
            }
        }
        node = node->next; //traverses until specified commit number is found
    }

    if(version == tail->commitNumber){ //compares current commit with most recent commit
        return true;
    } else {
        cout << "Current directory is not most recent directory." << endl;
        cout << "Please checkout most recent directory to access other menu options." << endl << endl;
        return false;  //prevents menu options when current commit is not most recent
    }
}