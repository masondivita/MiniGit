#include <iostream>
#include <fstream>
#include <string>
#include "git.hpp"

using namespace std;

bool search(string name){
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

    if(search(fileName)){
        cout << "File already exists in directory." << endl;
    }

}