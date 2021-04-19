#ifndef GIT_HPP
#define GIT_HPP


struct doublyNode{
    int commitNumber;
    singlyNode* head;
    doublyNode* previous;
    doublyNode* next;
};

struct singlyNode{
    std::string fileName; // Name of local file
    std::string fileVersion; // Name of file in .minigit folder
    singlyNode* next;
};


class miniGit
{
  private:
    doublyNode* head;
    doublyNode* tail;
    singlyNode* currHead;

  public:
    miniGit(){
    	head = NULL;
        tail = NULL;
    }
    ~miniGit();
    void addFiles(string fileName);
    void removeFiles(string fileName);
    void commit();
    void checkout(int version);
    
};







#endif