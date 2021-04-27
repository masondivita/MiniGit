#include <iostream>
#include <fstream>
#include <string>
#include "git.hpp"

using namespace std;

bool search(string name, singlyNode* head){
    
    singlyNode* ptr = head;
    
    while (ptr != NULL && ptr->fileName != name){
        
        ptr = ptr->next;
        
        if(ptr->fileName == name){
            return true;
        }
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

    string fileVersion = "__00__" + file;

    if (in_file.fail()){
        cout << "File not found. Please enter valid filename." << endl << endl;
    }

        if(search(file, currHead)){
            cout << "File already exists in directory." << endl;
        }

        if(currHead == NULL){
            currHead = new singlyNode;
            currHead->fileName = file;
            currHead->next = NULL;
            currHead->fileVersion = fileVersion;

        } else {
            singlyNode* last = currHead;
            
            while(last->next){
                
                last = last->next;
            }
            
            last->fileName = file;
            last->fileVersion = fileVersion;
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


bool miniGit::checkout(){

    int version = 0;
    doublyNode *node = head;

    cout << "Warning: Files in current directory will be overwritten" << endl;
    cout << "Enter commit number: " << endl << endl;

    cin >> version;

    if(head == NULL){

        cout << "No previous file version availible." << endl;
        return true;
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

    if(version == tail->commitNumber){
        return true;
    } else {
        cout << "Current directory is not most recent directory." << endl;
        cout << "Please checkout most recent directory to access other menu options." << endl << endl;
        return false;  
    }
}
