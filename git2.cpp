#include <iostream>
#include <fstream>
#include <string>
#include "git.hpp"

using namespace std;

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
