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
void miniGit::addFiles(string fileName, string fileVersion){
    cout << "Enter the file name: " << endl;

    string file;
    getline(cin, file);
    ifstream in_file(file);

    if (in_file.fail()){
        cout << "File not found. Please enter valid filename." << endl;
    }

    if(search(fileName, currHead)){
        cout << "File already exists in directory." << endl;
    }

     if(currHead == NULL){
        currHead = new singlyNode;
        currHead->fileName = fileName;
        currHead->next = NULL;
        currHead->fileVersion = fileVersion;
    } else {
        singlyNode* last = currHead;
        while(last->next){
            last = last->next;
        }
        last->fileName = fileName;
        last->fileVersion = fileVersion;
    }

    string checkoutFile(string fileName, string fileVersion){
        ofstream fileName(fileName);
        ifstream fileVersion(fileVersion);
        string line;

        while(getline(fileVersion, line)){
            fileName << line << endl;
        }

        fileVersion.close();
        fileName.close();

        return fileName;


    }

    void miniGit::checkout(int version){
        doublyNode *node = head;

        cout << "Warning: Files in current directtory will be overwritten" << endl;
        cout << "Enter commit number: " << endl;
        cin >> version;

        if(head == NULL){
            cout << "No previous file version availible." << endl;
            return;
        }
        while(node){
            node = node->next;
            if(node->commitNumber == version){
                singlyNode *curr = currHead;
                while(curr){
                    checkoutFile(curr->fileName, curr->fileVersion);
                    curr = curr->next;
                }
            }
        }
    }
}