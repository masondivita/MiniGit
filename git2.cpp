#include <iostream>
#include <fstream>
#include <string>
#include "git.hpp"

using namespace std;

bool search(string name, singlyNode* head){
    
    singlyNode* ptr = head;
    
    while (ptr != NULL){ // this was throwing errors so I cut it down a bit
        if(ptr->fileName == name){
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

/*int getMostRecentCommitNumber(doublyNode* head){
        
    doublyNode *node = head;
        
    while(node){
        node = node->next;
    }
    return node->commitNumber;   
}
bool isMostRecentCommit(int version, doublyNode* head){
    if(getMostRecentCommitNumber(head) == version){
            
        return true;
    } 
    return false;
}*/

void miniGit::addFiles(){
    
    string file;
    cout << "Enter the file name: " << endl;
    cin >> file;
    
    ifstream in_file(file);

    string fileVersion = ""; // this works better with my code, just becaue when that aren't commited, they don't have a version number

    if (in_file.fail()){
        cout << "File not found. Please enter valid filename." << endl << endl;
        return;
    }

    if(search(file, currHead)){
        cout << "File already exists in directory." << endl;
    }
    else if(currHead == NULL){
        currHead = new singlyNode;
        currHead->fileName = file;
        currHead->next = NULL;
        currHead->fileVersion = fileVersion;
    } else {
        singlyNode* last = currHead;
        
        while(last->next){
            
            last = last->next;
        }
        singlyNode* insert = new singlyNode; // there needed to be a new node created here to add more than one file
        insert->fileName = file;
        insert->fileVersion = fileVersion;
        last->next = insert;
    }
    in_file.close();
}

void checkoutFile(string fileName, string fileVersion){
    ofstream currFile(fileName);
    ifstream oldFile(".minigit/" + fileVersion);
    string line;

    while(getline(oldFile, line)) {
        currFile << line << endl;
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
        
        cout << "Please enter valid commit number!" << endl;
        return true;
    } else if(head == NULL){

        cout << "No previous file version availible." << endl;
        return true;
    }
        
    while(node){

        if(node->commitNumber == version){

            singlyNode *curr = node->head; // grabs the head of the current node rather than what hasn't been committed yet

            while(curr){

                checkoutFile(curr->fileName, curr->fileVersion);
                curr = curr->next;
            }
        }
        node = node->next;
    }

    if(version == tail->commitNumber){
        return true;
    } else {
        cout << "Current directory is not most recent directory." << endl;
        cout << "Please checkout most recent directory to access other menu options." << endl << endl;
        return false;  
    }
}