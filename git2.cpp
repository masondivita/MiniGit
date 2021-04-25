#include <iostream>
#include <fstream>
#include <string>
#include "git.hpp"

using namespace std;

bool search(string name, singlyNode* head){
    singlyNode* ptr = head;
    while (ptr != NULL){
        ptr = ptr->next;
        if(ptr->fileName == name){
            return true;
        }
    }
    return false;
}

void miniGit::addFiles(){
    cout << "Enter the file name: " << endl;

    string fileName;
    string fileVersion = "";
    cin >> fileName;
    ifstream in_file(fileName);

    if (in_file.fail()){
        cout << "File not found. Please enter valid filename." << endl;
    }

    in_file.close();

    // if(search(fileName, currHead)){
    //     cout << "File already exists in directory." << endl;
    // }

     if(currHead == NULL){
        singlyNode* nn = new singlyNode;
        currHead = nn;
        nn->fileName = fileName;
        nn->next = NULL;
        nn->fileVersion = fileVersion;
    } else {
        singlyNode* last = currHead;
        while(last->next){
            last = last->next;
        }
        singlyNode* nn = new singlyNode;
        nn->fileName = fileName;
        nn->next = NULL;
        nn->fileVersion = fileVersion;
        last->next = nn;
    }
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

void miniGit::checkout(){
    doublyNode *node = head;
    int version;
    cout << "Warning: Files in current directtory will be overwritten" << endl;
    cout << "Enter commit number: " << endl;
    cin >> version;
    if(head == NULL){
        cout << "No previous file version availible." << endl;
        return;
    }
    while(node){
        if(node->commitNumber == version){
            singlyNode *curr = currHead;
            while(curr){
                checkoutFile(curr->fileName, curr->fileVersion);
                curr = curr->next;
            }
        }
        node = node->next;
    }
}



