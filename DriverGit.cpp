#include <iostream>
#include <fstream>
#include <string>
#include "git.hpp"
using namespace std;

int main() {
    int option;

    while(option != 5){

        cout << "--Menu--" << endl;
        cout << "1. Add file" << endl;
        cout << "2. Remove file" << endl;
        cout << "3. Commit" << endl;
        cout << "4. Checkout" << endl;
        cout << "5. Quit" << endl;
        cin >> option;

        switch(option){
            case 1:
                cout << "Enter the file name: " << endl;

                string file;
                getline(cin, file);
                ifstream in_file(file);

        }
    }
}