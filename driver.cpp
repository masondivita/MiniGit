#include <iostream>
#include <fstream>
#include <string>
#include "miniGit.hpp"

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
                cout << "--------------------" << endl;
                break;
            }
            case 2: {
                kermmit->removeFiles();
                cout << "--------------------" << endl;
                break;
            }
            case 3: {
                kermmit->commit();
                cout << "--------------------" << endl;
                break;
            }
            case 4: {
                bool currCommit = false;
                while(!currCommit){
                    currCommit = kermmit->checkout();
                }
                cout << "--------------------" << endl;
                break;
            }

        }
    }
    delete kermmit;
}