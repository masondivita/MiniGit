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
void miniGit::addFiles(){
    cout << "Enter the file name: " << endl;

    string file;
    cin >> file;


    singlyNode* nn = new singlyNode;
    nn->fileName = file;
    nn->fileVersion = "";
    currHead = nn;

    ifstream in_file(file);

    if (in_file.fail()){
        cout << "File not found. Please enter valid filename." << endl;
    }

    if(search(file, currHead)){
        cout << "File already exists in directory." << endl;
    }
    
}