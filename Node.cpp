#include <iostream>
using namespace std;

class Node()
{
private:
    int data;
    Node *right, *left, *parent;
public:
    Node() {data = 0; right = left = nullptr;}
    Node(int d) {data = d; right = left = nullptr;}
    
    virtual int getData() {return data;}
    virtual Node* getRight() {return right;}
    virtual Node* getLeft() {return left;}
    virtual Node* getPatent() {return parent;}
    virtual void setData(int d) {data = d;}
    virtual void setRight(Node* r) {right = r;}
    virtual void setLeft(Node* l) {left = l;}
};

int main()
{
    
}
