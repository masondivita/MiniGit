#include <iostream>
#include <fstream>
#include <string>
#include "git.hpp"

using namespace std;

int main() {
    int option;
    miniGit* kermmit = new miniGit;

    while(option != 5){

        cout << "--Menu--" << endl;
        cout << "1. Add file" << endl;
        cout << "2. Remove file" << endl;
        cout << "3. Commit" << endl;
        cout << "4. Checkout" << endl;
        cout << "5. Quit" << endl;
        cin >> option;

        switch(option){
            case 1: {
                kermmit->addFiles();
                break;
            }
            case 2: {
                cout << "Enter the file name2222: " << endl;

                string removeFileName;
                getline(cin, removeFileName);
            }

        }
    }
}