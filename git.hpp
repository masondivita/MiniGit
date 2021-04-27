#ifndef GIT_HPP
#define GIT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;


struct singlyNode{
    string fileName; // Name of local file
    string fileVersion; // Name of file in .minigit folder
    singlyNode* next;
};

struct doublyNode{
    int commitNumber;
    singlyNode* head;
    doublyNode* previous;
    doublyNode* next;
};

struct Ht_item {
    int key;
    string fileVersion;
    Ht_item* next;
};


class miniGit {
  private:
    doublyNode* head;
    doublyNode* tail;
    singlyNode* currHead;
    Ht_item** HT;

  public:
    miniGit(){
    	head = NULL;
      tail = NULL;
      currHead = NULL;
      HT = new Ht_item*[100];
    }
    ~miniGit();
    void addFiles();
    void removeFiles();
    void commit();
    void checkout();
};







#endif